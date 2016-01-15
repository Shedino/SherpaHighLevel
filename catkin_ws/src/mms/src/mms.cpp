#include "ros/ros.h"

#include "mms_msgs/Cmd.h" // input
#include "mms_msgs/Ack_mission.h"// output
#include "mms_msgs/Arm.h"// output
#include "mms_msgs/Ack_arm.h" // input
#include "mms_msgs/Sys_status.h"// input
#include "mms_msgs/Grid_ack.h"  //input
#include <mavros/Sonar.h> // input
#include "mms_msgs/MMS_status.h"// output
#include <reference/Distance.h>// input
#include <mavros/Safety.h>// input
#include <reference/LeashingStatus.h>// input
#include <qos_sensors_autopilot/Qos_sensors.h>// input

// STATES DEFINITION -> CREATE A DEDICATED LIBRARY = TODO
//TODO make this static const int
#define ON_GROUND_NO_HOME 10
#define SETTING_HOME 20
#define ON_GROUND_DISARMED 30
#define ARMING 40
#define DISARMING 45
#define ON_GROUND_ARMED 50
// #define ON_GROUND_READY_TO_TAKEOFF 60
#define PERFORMING_TAKEOFF 70
#define IN_FLIGHT 80
#define GRID 90
#define PERFORMING_GO_TO 100
// #define READY_TO_LAND 110
#define PERFORMING_LANDING 120
#define LEASHING 140
#define PAUSED 150
#define MANUAL_FLIGHT 1000
#define FRAME_BARO 6
#define FRAME_SONAR 11


double PI = 3.1416; // pi
double eps_LAND = 10000.0; // distance to the target LAND position in millimeter
double eps_WP = 400.0; // distance to the target WAYPOINT position in millimeters           //TODO no hardcoded
double eps_TO = 400.0; // distance to the target TAKEOFF position in millimeters
double eps_alt = 400.0; // distance to the target altitude in millimeters
double eps_YAW = 10.0; // distance to the target YAW position in deg

class MmsNodeClass {
public:
	MmsNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromCmd_=n_.subscribe("/sent_command", 10, &MmsNodeClass::readCmdMessage,this); //subscribe to "sent_command" to exclude the "cmd_verifier" node
		subFromSonar_ = n_.subscribe("/sonar", 10, &MmsNodeClass::readSonarMessage,this);
		subFromSysStatus_=n_.subscribe("/system_status", 10, &MmsNodeClass::readSysStatusMessage,this);
		subFromDistance_=n_.subscribe("/distance", 10, &MmsNodeClass::readDistanceMessage,this);
		subFromGridAck_ = n_.subscribe("/grid_ack", 10, &MmsNodeClass::readGridAckMessage,this);
		subSafety_ = n_.subscribe("/safety_odroid", 2, &MmsNodeClass::readSafetyMessage,this);
		subLeashingStatus_ = n_.subscribe("/leashing_status", 10, &MmsNodeClass::readLeashingStatusMessage,this);
		subQos_sensors_ = n_.subscribe("/qos_sensors", 10, &MmsNodeClass::readQosSensorsMessage,this);
		
		// publishers
		pubToAckMission_=n_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);
		pubToArm_=n_.advertise<mms_msgs::Arm>("/arm", 10);
		pubToMmsStatus_=n_.advertise<mms_msgs::MMS_status>("/mms_status", 10);
		pubCmd_ = n_.advertise<mms_msgs::Cmd>("/cmd_from_mms", 10);


		//Initializing outputAckMission_
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.mav_mission_accepted = false;
		outputAckMission_.seq = 0;

		//Initializing outputArm_
		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = false;

		//Initializing outputMmsStatus_
		outputMmsStatus_.mms_state = ON_GROUND_NO_HOME;
		outputMmsStatus_.target_ref_frame = FRAME_BARO;
		
		//Initializing states
		SET_HOME = false;
		TAKEOFF = false;
		LAND = false;
		MISSION_START = false;
		WAYPOINT = false;
		ARMED = false;
		GRID_ENDED = false;
		GRID_EVENT = false;
		SAFETY_ON = false;
		SAFETY_OFF = false;
		LEASHING_START = false;
		LEASHING_END = false;
		LEASHING_FAILURE = false;
		PAUSE = false;
		CONTINUE = false;

		//Init something
		currentState = ON_GROUND_NO_HOME;
		previousState = ON_GROUND_NO_HOME;
		target_frame = FRAME_BARO;
		rate = 10;
		uint16_t counter_ = 0;
		counter_print = 0;
		seq_number = 0;
		Dh_TO = 0;
	}

	void readDistanceMessage(const reference::Distance::ConstPtr& msg)
	{
		inputDist_.error_pos=msg->error_pos;
		inputDist_.error_ang=msg->error_ang;
		inputDist_.error_alt=msg->error_alt;
		inputDist_.command=msg->command;
		inputDist_.seq=msg->seq;
	}
	
	void readSafetyMessage(const mavros::Safety::ConstPtr& msg)
	{
		Safety_.safety = msg->safety;
		if (Safety_.safety && !SAFETY_ON){
			SAFETY_ON = true;
			SAFETY_OFF = false;
			ROS_INFO("MMS: safety on");
		} else if (!Safety_.safety && !SAFETY_OFF) {
			SAFETY_OFF = true;
			SAFETY_ON = false;
			ROS_INFO("MMS: safety off");
		}
	}

	void readSysStatusMessage(const mms_msgs::Sys_status::ConstPtr& msg)
	{
		inputSysStatus_.armed=msg->armed;
		inputSysStatus_.voltage_battery=msg->voltage_battery;
		//ROS_INFO("Sys_status received");
		if (inputSysStatus_.armed)
		{
			ARMED = true;
                //ROS_INFO("ARMED");
		}
		else
		{
			ARMED = false;
                //ROS_INFO("DISARMED");
		}
	}

	void readGridAckMessage(const mms_msgs::Grid_ack::ConstPtr& msg){
		Grid_ack_.grid_completed = msg->grid_completed;
		Grid_ack_.completion_type = msg->completion_type;
		if (Grid_ack_.grid_completed) GRID_ENDED = true;
		else GRID_ENDED = false;
	}

	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	void readLeashingStatusMessage(const reference::LeashingStatus::ConstPtr& msg){
		if (msg->failure > 0) LEASHING_FAILURE = true;
	}

	void readQosSensorsMessage(const qos_sensors_autopilot::Qos_sensors::ConstPtr& msg){
		Qos_sensors_ = *msg;
	}

	void readCmdMessage(const mms_msgs::Cmd::ConstPtr& msg)
	{
		//TODO sostituire con inputCmd_ = *msg  ??
		inputCmd_.command = msg -> command;
		inputCmd_.param1  = msg -> param1;
		inputCmd_.param2  = msg -> param2;
		inputCmd_.param3  = msg -> param3;
		inputCmd_.param4  = msg -> param4;
		inputCmd_.param5  = msg -> param5;
		inputCmd_.param6  = msg -> param6;
		inputCmd_.param7  = msg -> param7;
		inputCmd_.frame  = msg -> frame;
		inputCmd_.seq  = msg -> seq;

        ROS_INFO("MMS: CMD_RECEIVED %d. Sequence: %d", inputCmd_.command, inputCmd_.seq);

		switch(inputCmd_.command)
		{
			case 16:  // MAV_CMD_NAV_WAYPOINT
			{
				ROS_INFO("MMS: CMD_WAYPOINT. Params: %f - %f - %f - %f",inputCmd_.param5,inputCmd_.param6,inputCmd_.param7,inputCmd_.param4);
				ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
				ROS_INFO("MMS: CMD_ALTITUDE = %f",inputCmd_.param7);
				ROS_INFO("MMS: SONAR DIST. =% d",inputSonar_.distance);

				double temp_max_speed;
				nodeHandle.getParam("/guidance_node_amsl/param/sat_xy", temp_max_speed);
				temp_max_speed = sqrt(pow(temp_max_speed,2));
				//conditions to accept waypoint: frame baro && speed (param1) less than max speed OR frame sonar and sonar working and altitude less than sonar range
				if (((inputCmd_.frame == FRAME_BARO) || (inputCmd_.frame == FRAME_SONAR && inputCmd_.param7 > 0.3f && inputCmd_.param7 < 3.0f && Qos_sensors_.sonar_present && Qos_sensors_.sonar_working)) && inputCmd_.param1 < temp_max_speed)
					{
						target_frame = inputCmd_.frame;
						seq_number = inputCmd_.seq;
						WAYPOINT = true;
						pubCmd_.publish(inputCmd_);  //cmd passed to reference
					}
					else
					{
						outputAckMission_.mission_item_reached = false;
						outputAckMission_.seq = seq_number;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");

						inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
						target_frame = FRAME_BARO;
						WAYPOINT = false;
					}

			} break;

			case 160:  // GRID
			{
				ROS_INFO("MMS: CMD_GRID. Params: %f - %f - %f - %f",inputCmd_.param1,inputCmd_.param2,inputCmd_.param3,inputCmd_.param4);
				ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);

				if ((inputCmd_.frame == FRAME_BARO) || (inputCmd_.frame == FRAME_SONAR))       //TODO add more sanity check maybe
					{
						target_frame = inputCmd_.frame;
						seq_number = inputCmd_.seq;
						GRID_EVENT = true;
						pubCmd_.publish(inputCmd_);  //cmd passed to reference
					}
					else
					{
						outputAckMission_.mission_item_reached = false;
						outputAckMission_.seq = seq_number;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");

						inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
						target_frame = FRAME_BARO;
						GRID_EVENT = false;
					}

			} break;

			case 161:  // GRID VERTEX
			{
				pubCmd_.publish(inputCmd_);  //cmd passed to reference

			} break;

			case 21:  // MAV_CMD_NAV_LAND
			{
				ROS_INFO("MMS: CMD_LAND");
				ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
				if ((inputCmd_.frame == FRAME_BARO) || (inputCmd_.frame == FRAME_SONAR && inputSonar_.distance != -1))
				{
					target_frame = inputCmd_.frame;
					seq_number = inputCmd_.seq;
					LAND = true;
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				}
				else
				{
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
					target_frame = FRAME_BARO;
					LAND = false;
				}
			}break;

			case 22:  // MAV_CMD_NAV_TAKEOFF
			{
				ROS_INFO("MMS: CMD_TAKEOFF");
				Dh_TO = (int)(inputCmd_.param7*1000.0f);
				ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
				ROS_INFO("MMS: CMD_DH_TO = %d",Dh_TO);
				ROS_INFO("MMS: SONAR DIST. =% d",inputSonar_.distance);

				if ((inputCmd_.frame == FRAME_BARO) || (inputCmd_.frame == FRAME_SONAR && Dh_TO > 300 && Dh_TO <= 3000 && inputSonar_.distance > 0))
				{
					target_frame = inputCmd_.frame;
					seq_number = inputCmd_.seq;
					TAKEOFF = true;
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				} else {
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
					target_frame = FRAME_BARO;
					TAKEOFF = false;
				}
			}break;

			case 179: // MAV_CMD_DO_SET_HOME
			{
				ROS_INFO("MMS: CMD_SET_HOME");
				SET_HOME = true;
				pubCmd_.publish(inputCmd_);  //cmd passed to reference
				seq_number = inputCmd_.seq;
			}break;

			case 300: // MAV_CMD_MISSION_START
			{
				ROS_INFO("MMS: CMD_MISSION_START");
				MISSION_START = true;
				pubCmd_.publish(inputCmd_);  //cmd passed to reference
			}break;

			case 25:  // MAV_CMD_NAV_FOLLOW (LEASHING)
			{
				//seq_number = inputCmd_.seq;
				if (inputCmd_.param1 == 1){
					LEASHING_START = true;
					ROS_INFO("MMS: CMD_LEASHING_START");
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				} else if (inputCmd_.param1 == 0){
					LEASHING_END = true;
					ROS_INFO("MMS: CMD_LEASHING_STOP");
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				} else {
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED (LEASHING). WRONG PARAM1");
				}
				
			}break;

			case 252:  // MAV_CMD_OVERRIDE_GOTO (PAUSE/CONTINUE)
			{
				//seq_number = inputCmd_.seq;
				if (inputCmd_.param1 == 0){          //PAUSE
					PAUSE = true;
					ROS_INFO("MMS: CMD_PAUSE");
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				} else if (inputCmd_.param1 == 1){		//CONTINUE
					CONTINUE = true;
					ROS_INFO("MMS: CMD_CONTINUE");
					pubCmd_.publish(inputCmd_);  //cmd passed to reference
				} else {
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED (PAUSE/CONTINUE). WRONG PARAM1");
				}
				
			}break;
		}
	}

	void set_events_false()
	{
		SET_HOME = false;
		TAKEOFF = false;
		LAND = false;
		MISSION_START = false;
		WAYPOINT = false;
		GRID_EVENT = false;
		GRID_ENDED = false;
		SAFETY_ON = false;
		SAFETY_OFF = false;
		LEASHING_START = false;
		LEASHING_END = false;
		LEASHING_FAILURE = false;
		PAUSE = false;
		CONTINUE = false;
	}

	void MMS_Handle()
	{
		switch(currentState)

		{

			case ON_GROUND_NO_HOME:
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO_ONCE("MMS->REF: CURRENT_STATE = ON_GROUND_NO_HOME");
				
				if (ARMED)
				{
					set_events_false();
					ARMED = true;

					outputArm_.arm_disarm = false;
					outputArm_.new_arm_disarm = true;
					// pubToArm_.publish(outputArm_); // TODO automatic disarming
					ROS_INFO("MMS->APM: DISARMING");

					currentState = ON_GROUND_NO_HOME;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_NO_HOME");
					break;
				}
				if (SET_HOME)
				{
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");
					set_events_false();
					currentState = SETTING_HOME;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
					break;
				}
				// Use these lines to introduce the mission_not_accepted for those commands which cannot be executed in this state
				if (TAKEOFF || LAND || WAYPOINT)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}

				break;

			case SETTING_HOME:

				outputAckMission_.mission_item_reached = true;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = false;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

				currentState = ON_GROUND_DISARMED;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = FRAME_BARO;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
				break;

			case ON_GROUND_DISARMED:
				
				if (TAKEOFF)
				{
					set_events_false();
					TAKEOFF = true;
					ARMED = false;

					outputArm_.arm_disarm = true;
					outputArm_.new_arm_disarm = true;
					pubToArm_.publish(outputArm_);
					ROS_INFO("MMS->APM: ARMING");

					counter_ = 0;     //start timing to rearm
					currentState = ARMING;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = ARMING");
					break;
				}
				if (SET_HOME)
				{
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = SETTING_HOME;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
					break;
				}
				// Use these lines to introduce the mission_not_accepted for those commands which cannot be executed in this state
				if (LAND || WAYPOINT)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}
				break;

			case ARMING:

				if (LAND)
				{
					set_events_false();
					ARMED = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					outputArm_.arm_disarm = false;
					outputArm_.new_arm_disarm = true;
					// pubToArm_.publish(outputArm_); // TODO automatic disarming
					ROS_INFO("MMS->APM: DISARMING");

					currentState = DISARMING;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = DISARMING");
					counter_ = 0;     //start timing to rearm
					break;
				}
				if (ARMED)
				{
					ROS_INFO("MMS: !!! DRONE ARMED !!!");
					set_events_false();
					TAKEOFF = true; // to complete automatic TO

					currentState = ON_GROUND_ARMED;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
					break;
				}
				if (counter_>=50)    //5 seconds
				{
					currentState = ON_GROUND_DISARMED;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS: ARMING FAILED");
					ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
				}
				if (SET_HOME || WAYPOINT)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}
				break;

			case DISARMING:  // TODO automatic disarming

				if (ARMED == false)
				{
					ROS_INFO("MMS: DRONE DISARMED!");
					set_events_false();

					outputAckMission_.mission_item_reached = true;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

					currentState = ON_GROUND_DISARMED;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
					break;
				}
				if (counter_>=50)    //5 seconds
				{
					currentState = ON_GROUND_ARMED;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					LAND = true;
					ROS_INFO("MMS: DISARMING FAILED");
					ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
				}
				if (TAKEOFF || LAND || SET_HOME || WAYPOINT)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}
				break;

			case ON_GROUND_ARMED:

				if (LAND) // TODO automatic disarming
				{
					set_events_false();
					ARMED = true;
			
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					outputArm_.arm_disarm = false;
					outputArm_.new_arm_disarm = true;
					// pubToArm_.publish(outputArm_); // TODO automatic disarming
					ROS_INFO("MMS->APM: DISARMING");

					currentState = DISARMING;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;//inputCmd_.frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = DISARMING");
					counter_ = 0;     //start timing to rearm
					break;
				}
				if (TAKEOFF)
				{
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_TAKEOFF;// ON_GROUND_READY_TO_TAKEOFF; // with this modification the state that needs the MISSION_START is excluded
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = FRAME_BARO;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_TAKEOFF");//READY_TO_TAKEOFF");
					break;
				}
						if (SET_HOME || WAYPOINT)
						{
							set_events_false();
							outputAckMission_.mission_item_reached = false;
							outputAckMission_.seq = inputCmd_.seq;
							outputAckMission_.mav_mission_accepted = false;
							pubToAckMission_.publish(outputAckMission_);
							ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
							break;
						}
				break;
		

			case PERFORMING_TAKEOFF:

				if (SAFETY_ON){                    //PUT THIS FOR ROLLING BACK FROM MANUAL_FLIGHT
					set_events_false();
					previousState = currentState;   //save last state in previousState
					currentState = MANUAL_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = MANUAL_FLIGHT");
					break;
				}
				
				if (LAND)
				{
					set_events_false();
					LAND = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_LANDING;//READY_TO_LAND;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");//READY_TO_LAND");

					break;
				}
				
				//ROS_INFO("MMS DEBUG: Command: %d - Seq: %d - Seq_req: %d", inputDist_.command, inputDist_.seq, seq_number);
				if (inputDist_.command == 22 && seq_number == inputDist_.seq)
				{
					ROS_INFO_ONCE("MMS: REACHING THE TAKEOFF TARGET");
					//ROS_INFO("MMS: Distances: %.3f - %.3f - %.3f", inputDist_.error_pos, inputDist_.error_ang, inputDist_.error_alt);
					if (inputDist_.error_pos < eps_TO && inputDist_.error_ang < eps_YAW && inputDist_.error_alt < eps_alt)
					{
						set_events_false();

						outputAckMission_.mission_item_reached = true;
						outputAckMission_.seq = seq_number;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

						currentState = IN_FLIGHT;
						outputMmsStatus_.mms_state = currentState;
						outputMmsStatus_.target_ref_frame = target_frame;
						pubToMmsStatus_.publish(outputMmsStatus_);
						ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");

						// break; // comment this line to execute also the following if()
					}
				}
				
				if (TAKEOFF || SET_HOME || WAYPOINT)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}
				break;

			case IN_FLIGHT:

				if (SAFETY_ON){                    //PUT THIS FOR ROLLING BACK FROM MANUAL_FLIGHT
					set_events_false();
					previousState = currentState;   //save last state in previousState
					currentState = MANUAL_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = MANUAL_FLIGHT");
					break;
				}
				
				if (LAND)
				{
					set_events_false();
					LAND = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_LANDING;// READY_TO_LAND; // with this modification the state that needs the MISSION_START is excluded
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS-REF: CURRENT_STATE = PERFORMING_LANDING"); // READY_TO_LAND");

					break;
				}
				if (WAYPOINT)
				{
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_GO_TO; // READY_TO_GO; // with this modification the state that needs the MISSION_START is excluded
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);

					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_GO_TO"); // READY_TO_GO");
					break;
				}
				if (GRID_EVENT)           //TODO add this event in other states too (in waypoint and ??)
				{
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = GRID; //
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);

					ROS_INFO("MMS->REF: CURRENT_STATE = GRID"); // READY_TO_GO");
					break;
				}
				if (TAKEOFF || SET_HOME)
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
					break;
				}
				if (LEASHING_START)             //TODO maybe add LEASHING_START in other states
				{
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED (LEASHING)");
					
					currentState = LEASHING;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = LEASHING");
				}
				break;
			

			case GRID:
				
				if (SAFETY_ON){                    //PUT THIS FOR ROLLING BACK FROM MANUAL_FLIGHT
					set_events_false();
					previousState = currentState;   //save last state in previousState
					currentState = MANUAL_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = MANUAL_FLIGHT");
					break;
				}
				
				if (LAND){
					set_events_false();
					LAND = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_LANDING; // READY_TO_LAND;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");//READY_TO_LAND");

					break;
				}
				if (WAYPOINT){
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_GO_TO;//READY_TO_GO;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_GO_TO");//READY_TO_GO");

					break;
				}
				if (GRID_ENDED){
					set_events_false();
					currentState = IN_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");

					if (Grid_ack_.completion_type == 1){       //success
						outputAckMission_.mission_item_reached = true;
						outputAckMission_.seq = seq_number;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: GRID FINISHED SUCCESFULLY");
					} else {                               //failure
						outputAckMission_.mission_item_reached = false;
						outputAckMission_.seq = inputCmd_.seq;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: GRID FINISHED NOT SUCCESFULLY");
					}
				}
				if (PAUSE){
					set_events_false();
					previousState = currentState;   //save last state in previousState
					currentState = PAUSED;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PAUSED");
				}
				break;

			case PERFORMING_GO_TO:

				if (SAFETY_ON){                    //PUT THIS FOR ROLLING BACK FROM MANUAL_FLIGHT
					set_events_false();
					previousState = currentState;   //save last state in previousState
					currentState = MANUAL_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = MANUAL_FLIGHT");
					break;
				}
				
				if (LAND)
				{
					set_events_false();
					LAND = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_LANDING; // READY_TO_LAND;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");//READY_TO_LAND");

					break;
				}
				if (WAYPOINT)
				{
					set_events_false();

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_GO_TO;//READY_TO_GO;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_GO_TO");//READY_TO_GO");

					break;
				}

				//ROS_INFO("-------------- command %d --- seq %d ---  seq2: %d  ------", inputDist_.command, seq_number, inputDist_.seq);
				if (inputDist_.command == 16  && seq_number == inputDist_.seq)
				{
					ROS_INFO_ONCE("MMS: REACHING THE WAYPOINT TARGET");
					//ROS_INFO("----------MMS: Distances: %.3f - %.3f - %.3f", inputDist_.error_pos, inputDist_.error_ang, inputDist_.error_alt);
					if (inputDist_.error_pos < eps_WP && inputDist_.error_ang < eps_YAW && inputDist_.error_alt < eps_alt)
					{
						set_events_false();

						outputAckMission_.mission_item_reached = true;
						outputAckMission_.seq = seq_number;
						outputAckMission_.mav_mission_accepted = false;
						pubToAckMission_.publish(outputAckMission_);
						ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

						currentState = IN_FLIGHT;
						outputMmsStatus_.mms_state = currentState;
						outputMmsStatus_.target_ref_frame = target_frame;
						pubToMmsStatus_.publish(outputMmsStatus_);
						ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");

						// break;
					}
				}
						if (TAKEOFF || SET_HOME)
						{
							set_events_false();
							outputAckMission_.mission_item_reached = false;
							outputAckMission_.seq = inputCmd_.seq;
							outputAckMission_.mav_mission_accepted = false;
							pubToAckMission_.publish(outputAckMission_);
							ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
							break;
						}
				break;

			case PERFORMING_LANDING:

				set_events_false();  // TODO automatic disarming (eliminate)

				//if (inputDist_.command == 21  && seq_number == inputDist_.seq)
				//{
					ROS_INFO_ONCE("MMS: REACHING THE LANDING TARGET");
					/*if (inputDist_.error_alt > eps_LAND)
					{
						set_events_false();
						LAND = true;

						currentState = ON_GROUND_ARMED;
						outputMmsStatus_.mms_state = currentState;
						outputMmsStatus_.target_ref_frame = FRAME_BARO;
						pubToMmsStatus_.publish(outputMmsStatus_);
						ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
					}*/
				//}
				break;
				
			case MANUAL_FLIGHT:

				if (SAFETY_OFF)
				{
					set_events_false();
					currentState = previousState;   //rolling back to last state
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS: CURRENT_STATE = BACK TO OLD ONE: %d",currentState);
				}
				break;
				
			case PAUSED:
				if (CONTINUE){
					set_events_false();
					currentState = previousState;   //rolling back to last state
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS: CURRENT_STATE = BACK TO OLD ONE: %d",currentState);
				}
				break;
			
			case LEASHING:
				if (LAND){
					set_events_false();
					LAND = true;

					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

					currentState = PERFORMING_LANDING;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");

					break;
				}
				if (LEASHING_END){
					set_events_false();
					outputAckMission_.mission_item_reached = true;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED (LEASHING)");
					
					currentState = IN_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");
				}
				if (LEASHING_FAILURE){
					set_events_false();
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = inputCmd_.seq;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_FAILED (LEASHING)");

					currentState = IN_FLIGHT;
					outputMmsStatus_.mms_state = currentState;
					outputMmsStatus_.target_ref_frame = target_frame;
					pubToMmsStatus_.publish(outputMmsStatus_);
					ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");
				}
				break;
		}
}

void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("MMS: RUNNING");

		MMS_Handle();
		counter_++;
		ros::spinOnce();

		loop_rate.sleep();
	}
}

protected:
/*state here*/
ros::NodeHandle n_;

// Subscribers
/*ros::Subscriber subFromPosition_;
guidance_node_amsl::Position_nav inputPos_;*/

ros::Subscriber subFromCmd_;
mms_msgs::Cmd inputCmd_;

ros::Subscriber subFromAckArm_;
mms_msgs::Ack_arm inputAckArm_;

ros::Subscriber subFromGridAck_;
mms_msgs::Grid_ack Grid_ack_;

ros::Subscriber subSafety_;
mavros::Safety Safety_;

ros::Subscriber subFromSysStatus_;
mms_msgs::Sys_status inputSysStatus_;

ros::Subscriber subFromSonar_;
mavros::Sonar inputSonar_;

ros::Subscriber subLeashingStatus_;

ros::Subscriber subQos_sensors_;
qos_sensors_autopilot::Qos_sensors Qos_sensors_;

/*ros::Subscriber subFromReference_;
guidance_node_amsl::Reference inputRef_;
guidance_node_amsl::Reference target_;*/

ros::Subscriber subFromDistance_;
reference::Distance inputDist_;

// Publishers
ros::Publisher pubToAckMission_;
mms_msgs::Ack_mission outputAckMission_;

ros::Publisher pubCmd_;

ros::Publisher pubToArm_;
mms_msgs::Arm outputArm_;

//ros::Publisher pubToAckCmd_;
//mms_msgs::Ack_cmd outputAckCmd_;

ros::Publisher pubToMmsStatus_;
mms_msgs::MMS_status outputMmsStatus_;

// INPUTS GCS -> MMS
bool SET_HOME;
bool TAKEOFF;
bool LAND;
bool MISSION_START;
bool WAYPOINT;
bool GRID_EVENT;
bool ARMED;
bool GRID_ENDED;
bool SAFETY_ON;
bool SAFETY_OFF;
bool LEASHING_START;
bool LEASHING_END;
bool LEASHING_FAILURE;
bool PAUSE;
bool CONTINUE;
// static bool CONDITION_YAW = false;

// INPUTS APM -> MMS
// bool MAV_RESULT  = false;
// bool New_MAV_RESULT= false;


// OUTPUTS MMS -> GCS
// bool MISSION_ITEM_REACHED = false;
// bool MAV_MISSION_ACCEPTED = false;

// OUTPUTS MMS -> APM
// bool arm_disarm = false; // true = arm, false = disarm
// bool new_arm_disarm = false; // true = armed, false = none

// STATE INITIALIZATION
int currentState;
int previousState;      //used when rolling back to previous state from MANUAL_FLIGHT
// int lastARMState = ON_GROUND_DISARMED;
int target_frame;

// ERRORS DEFINITION
// float err; // current distance to the target position in millimeters
/*int eps_LAND = 100; // target distance to the LAND position in millimeter
int eps_WP = 100; // target distance to the WAYPOINT position in millimeters
int eps_TO = 100; // target distance to the TAKEOFF position in millimeters*/

int rate;

//std::vector<double> param_;
//double debugParam;

private:

uint16_t counter_;
uint16_t counter_print;
uint16_t seq_number;
int Dh_TO;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mms");
	ros::NodeHandle node;

	MmsNodeClass mmsNode(node);

	mmsNode.run();
	return 0;
}
