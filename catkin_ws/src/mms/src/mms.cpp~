#include "ros/ros.h"

#include "mms_msgs/Cmd.h" // input
#include "mms_msgs/Ack_mission.h"// output
#include "mms_msgs/Arm.h"// output
#include "mms_msgs/Ack_arm.h" // input
#include "mms_msgs/Sys_status.h"// input
#include <mavros/Sonar.h> // input
#include "mms_msgs/MMS_status.h"// output
#include <reference/Distance.h>// input

// STATES DEFINITION -> CREATE A DEDICATED LIBRARY = TODO
#define ON_GROUND_NO_HOME 10
#define SETTING_HOME 20
#define ON_GROUND_DISARMED 30
#define ARMING 40
#define DISARMING 45
#define ON_GROUND_ARMED 50
#define ON_GROUND_READY_TO_TAKEOFF 60
#define PERFORMING_TAKEOFF 70
#define IN_FLIGHT 80
#define READY_TO_GO 90
#define PERFORMING_GO_TO 100
#define READY_TO_LAND 110
#define PERFORMING_LANDING 120

double PI = 3.1416; // pi
double eps_LAND = 10000.0; // distance to the target LAND position in millimeter
double eps_WP = 1500.0; // distance to the target WAYPOINT position in millimeters
double eps_TO = 1500.0; // distance to the target TAKEOFF position in millimeters
double eps_alt = 500.0; // distance to the target altitude in millimeters
double eps_YAW = 20.0; // distance to the target YAW position in deg

class MmsNodeClass {
public:
	MmsNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromCmd_=n_.subscribe("/command", 10, &MmsNodeClass::readCmdMessage,this);
        	subFromSonar_ = n_.subscribe("/sonar", 10, &MmsNodeClass::readSonarMessage,this);
		subFromSysStatus_=n_.subscribe("/system_status", 10, &MmsNodeClass::readSysStatusMessage,this);
		subFromDistance_=n_.subscribe("/distance", 10, &MmsNodeClass::readDistanceMessage,this);
		
		// publishers
		pubToAckMission_=n_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);
		pubToArm_=n_.advertise<mms_msgs::Arm>("/arm", 10);
		pubToMmsStatus_=n_.advertise<mms_msgs::MMS_status>("/mms_status", 10);

		//Initializing outputAckMission_
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.mav_mission_accepted = false;
		outputAckMission_.seq = 0;

		//Initializing outputArm_
		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = false;

		//Initializing outputMmsStatus_
		outputMmsStatus_.mms_state = ON_GROUND_NO_HOME;
		outputMmsStatus_.target_ref_frame = 6;
	}

	void readDistanceMessage(const reference::Distance::ConstPtr& msg)
	{
		inputDist_.error_pos=msg->error_pos;
		inputDist_.error_ang=msg->error_ang;
		inputDist_.error_alt=msg->error_alt;
		inputDist_.command=msg->command;
		inputDist_.seq=msg->seq;
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

	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	void readCmdMessage(const mms_msgs::Cmd::ConstPtr& msg)
	{
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

        ROS_INFO("MMS: CMD_RECEIVED %d", inputCmd_.command);

		switch(inputCmd_.command)
		{
		case 16:  // MAV_CMD_NAV_WAYPOINT
		{
			ROS_INFO("MMS: CMD_WAYPOINT. Params: %f - %f - %f - %f",inputCmd_.param5,inputCmd_.param6,inputCmd_.param7,inputCmd_.param4);
			ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
			ROS_INFO("MMS: CMD_ALTITUDE = %f",inputCmd_.param7);
			ROS_INFO("MMS: SONAR DIST. =% d",inputSonar_.distance);

			if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputCmd_.param7 > 0.0f && inputCmd_.param7 < 3.0f && inputSonar_.distance != -1))
				{
					target_frame = inputCmd_.frame;
					seq_number = inputCmd_.seq;
					WAYPOINT = true;
				}
				else
				{
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");

					inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
					target_frame = 6;
					WAYPOINT = false;
			    }

		} break;
		case 21:  // MAV_CMD_NAV_LAND
		{
			ROS_INFO("MMS: CMD_LAND");
			ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
			if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputSonar_.distance != -1))
			{
				target_frame = inputCmd_.frame;
				seq_number = inputCmd_.seq;
				LAND = true;
			}
			else
			{
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = false;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				target_frame = 6;
			}
		}break;
		case 22:  // MAV_CMD_NAV_TAKEOFF
		{
			ROS_INFO("MMS: CMD_TAKEOFF");
			Dh_TO = (int)(inputCmd_.param7*1000.0f);
			ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
			ROS_INFO("MMS: CMD_DH_TO = %d",Dh_TO);
			ROS_INFO("MMS: SONAR DIST. =% d",inputSonar_.distance);

			if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && Dh_TO > 0 && Dh_TO < 3000 && inputSonar_.distance != -1))
			{
				target_frame = inputCmd_.frame;
				seq_number = inputCmd_.seq;
				TAKEOFF = true;
			}
			else
			{
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = false;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				target_frame = 6;
				TAKEOFF = false;
			}
		}break;

		case 179: // MAV_CMD_DO_SET_HOME
		{
			ROS_INFO("MMS: CMD_SET_HOME");
			SET_HOME = true;

			seq_number = inputCmd_.seq;
		}break;
		case 300: // MAV_CMD_MISSION_START
		{
			ROS_INFO("MMS: CMD_MISSION_START");
			MISSION_START = true;
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
	}

	void MMS_Handle()
	{
		switch(currentState)

		{

		case ON_GROUND_NO_HOME:
			outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
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
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
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
				outputMmsStatus_.target_ref_frame = 6;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
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
			outputMmsStatus_.target_ref_frame = 6;
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
				outputMmsStatus_.target_ref_frame = 6;
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
				outputMmsStatus_.target_ref_frame = 6;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
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
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
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
				outputMmsStatus_.target_ref_frame = 6;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
				break;
			}
			if (counter_>=50)    //5 seconds
			{
				currentState = ON_GROUND_DISARMED;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = 6;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS: ARMING FAILED");
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
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
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
			}
			if (counter_>=50)    //5 seconds
			{
				currentState = ON_GROUND_ARMED;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				LAND = true;
				ROS_INFO("MMS: DISARMING FAILED");
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
			}
			break;

case ON_GROUND_ARMED:

	if (LAND) // TODO automatic disarming
	{
		set_events_false();
		ARMED = true;

		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = true;
		// pubToArm_.publish(outputArm_); // TODO automatic disarming
		ROS_INFO("MMS->APM: DISARMING");

		currentState = DISARMING;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
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

		currentState = ON_GROUND_READY_TO_TAKEOFF;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = 6;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_TAKEOFF");
		break;
	}
	break;

case ON_GROUND_READY_TO_TAKEOFF:

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
		outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = DISARMING");
		counter_ = 0;     //start timing to rearm
		break;
	}
	if (MISSION_START)
	{
		set_events_false();

		currentState = PERFORMING_TAKEOFF;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_TAKEOFF");
		break;
	}
	break;

case PERFORMING_TAKEOFF:

	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

		break;
	}

	if (inputDist_.command == 22 && seq_number == inputDist_.seq)
	{
		ROS_INFO_ONCE("MMS: REACHING THE TAKEOFF TARGET");
		if (inputDist_.error_pos < eps_TO and inputDist_.error_ang < eps_YAW and inputDist_.error_alt < eps_alt)
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

			break;
		}
	}
	break;

case IN_FLIGHT:

	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS-REF: CURRENT_STATE = READY_TO_LAND");

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

		currentState = READY_TO_GO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);

		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_GO");
		break;
	}
	break;

case READY_TO_GO:

	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

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

		currentState = READY_TO_GO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_GO");

		break;
	}
	if (MISSION_START)
	{
		set_events_false();

		currentState = PERFORMING_GO_TO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_GO_TO");

		break;
	}
	break;

case PERFORMING_GO_TO:

	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

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

		currentState = READY_TO_GO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_GO");

		break;
	}

	if (inputDist_.command == 16  && seq_number == inputDist_.seq)
	{
		ROS_INFO_ONCE("MMS: REACHING THE WAYPOINT TARGET");
		//ROS_INFO("MMS: Distances: %.3f - %.3f - %.3f", inputDist_.error_pos, inputDist_.error_ang, inputDist_.error_alt);
		if (inputDist_.error_pos < eps_WP and inputDist_.error_ang < eps_YAW and inputDist_.error_alt < eps_alt)
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

			break;
		}
	}

	break;

case READY_TO_LAND:

	if (MISSION_START)
	{
		set_events_false();
		LAND = true;

		currentState = PERFORMING_LANDING;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");
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

		currentState = READY_TO_GO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_GO");

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
			outputMmsStatus_.target_ref_frame = 6;
			pubToMmsStatus_.publish(outputMmsStatus_);
			ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
		}*/
	//}
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

ros::Subscriber subFromSysStatus_;
mms_msgs::Sys_status inputSysStatus_;

ros::Subscriber subFromSonar_;
mavros::Sonar inputSonar_;

/*ros::Subscriber subFromReference_;
guidance_node_amsl::Reference inputRef_;
guidance_node_amsl::Reference target_;*/

ros::Subscriber subFromDistance_;
reference::Distance inputDist_;

// Publishers
ros::Publisher pubToAckMission_;
mms_msgs::Ack_mission outputAckMission_;

ros::Publisher pubToArm_;
mms_msgs::Arm outputArm_;

//ros::Publisher pubToAckCmd_;
//mms_msgs::Ack_cmd outputAckCmd_;

ros::Publisher pubToMmsStatus_;
mms_msgs::MMS_status outputMmsStatus_;

// INPUTS GCS -> MMS
bool SET_HOME = false;
bool TAKEOFF = false;
bool LAND = false;
bool MISSION_START = false;
bool WAYPOINT = false;
// static bool CONDITION_YAW = false;

// INPUTS APM -> MMS
// bool MAV_RESULT  = false;
// bool New_MAV_RESULT= false;
bool ARMED = false;

// OUTPUTS MMS -> GCS
// bool MISSION_ITEM_REACHED = false;
// bool MAV_MISSION_ACCEPTED = false;

// OUTPUTS MMS -> APM
// bool arm_disarm = false; // true = arm, false = disarm
// bool new_arm_disarm = false; // true = armed, false = none

// STATE INITIALIZATION
int currentState = ON_GROUND_NO_HOME;
// int lastARMState = ON_GROUND_DISARMED;
int target_frame = 6;

// ERRORS DEFINITION
// float err; // current distance to the target position in millimeters
/*int eps_LAND = 100; // target distance to the LAND position in millimeter
	int eps_WP = 100; // target distance to the WAYPOINT position in millimeters
	int eps_TO = 100; // target distance to the TAKEOFF position in millimeters*/

int rate = 10;

//std::vector<double> param_;
//double debugParam;

private:

uint16_t counter_ = 0;
uint16_t counter_print = 0;
uint16_t seq_number = 0;
int Dh_TO = 0;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mms");
	ros::NodeHandle node;

	MmsNodeClass mmsNode(node);

	mmsNode.run();
	return 0;
}
