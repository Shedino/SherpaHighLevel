#include "ros/ros.h"
// #include <vector>

#include "guidance_node_amsl/Position.h"
#include "guidance_node_amsl/Reference.h"
#include "mms/Cmd.h"
#include "mms/Ack_cmd.h"
#include "mms/Arm.h"
#include "mms/Ack_arm.h"
#include "mms/Sys_status.h"


// STATES DEFINITION
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

float PI = 3.1416; // pi
float eps_LAND = 100; // distance to the target LAND position in millimeter
float eps_WP = 100; // distance to the target WAYPOINT position in millimeters
float eps_TO = 100; // distance to the target TAKEOFF position in millimeters
float eps_YAW = 1; // distance to the target YAW position in deg
float Dh_TO = 500; // takeoff height above the groung in millimeters

class MmsNodeClass {
public:
	MmsNodeClass(ros::NodeHandle& node){

		n_=node;

		//param_ = std::vector<double>(9);

		/*//frequency
		node.param("guidance_node_amsl/frequency",rate,10);

		// DEBUG param test
		node.param("guidance_node_amsl/debug/testOffset",debugParam,1.0);

		//control param
		node.param("guidance_node_amsl/param/sat_xy", param_[0], 3.0);
		node.param("guidance_node_amsl/param/sat_z", param_[1], 1.5);
		node.param("guidance_node_amsl/param/sat_yaw", param_[2], 3.14);
		node.param("guidance_node_amsl/param/gain_NE", param_[3], 0.06);
		node.param("guidance_node_amsl/param/gain_D", param_[4], 0.06);
		node.param("guidance_node_amsl/param/gain_yaw", param_[5], 0.3);
		node.param("guidance_node_amsl/param/deadzone_NE", param_[6], 1.5);
		node.param("guidance_node_amsl/param/deadzone_D", param_[7], 0.2);
		node.param("guidance_node_amsl/param/gain_integralDown", param_[8],0.025);*/

		//subscribers
		subFromPosition_=n_.subscribe("/position", 10, &MmsNodeClass::readPositionMessage,this);
		subFromCmd_=n_.subscribe("/command", 10, &MmsNodeClass::readCmdMessage,this);
		// subFromAckArm_=n_.subscribe("/acknowledge_arming", 10, &MmsNodeClass::readAckArmMessage,this);
		subFromSysStatus_=n_.subscribe("/system_status", 10, &MmsNodeClass::readSysStatusMessage,this);
		// publishers
		pubToAckCmd_=n_.advertise<mms::Ack_cmd>("/ack_cmd", 10);
		pubToArm_=n_.advertise<mms::Arm>("/arm", 10);
		pubToReference_=n_.advertise<guidance_node_amsl::Reference>("/reference",10);

		//Initializing outputRef_
		outputRef_.Latitude = 0;
		outputRef_.Longitude = 0;
		outputRef_.AltitudeRelative = 0;
		outputRef_.Yawangle = 0;
		outputRef_.Mode = 100;

		//Initializing outputAckCmd_
		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = false;

		//Initializing outputArm_
		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = false;
	}

	void get_current_position()
	{
		outputRef_.Latitude = inputPos_.Latitude;
		outputRef_.Longitude = inputPos_.Longitude;
		outputRef_.AltitudeRelative = inputPos_.AltitudeRelative;//inputPos_.AltitudeAMSL - Home.AltitudeAMSL;
		outputRef_.Yawangle = inputPos_.Yawangle;
		outputRef_.Mode = 0;
		ROS_INFO("pos lat, %d, lon, %d", inputPos_.Latitude, inputPos_.Longitude);
                ROS_INFO("pos AMSL, %d, Home AMSL, %d, rel, %d", inputPos_.AltitudeAMSL, Home.AltitudeAMSL, outputRef_.AltitudeRelative);
	}

	void get_target_position()
	{
		outputRef_.Latitude = inputCmd_.param5;
		outputRef_.Longitude = inputCmd_.param6;
		outputRef_.AltitudeRelative = inputCmd_.param7;// - Home.AltitudeAMSL;
		outputRef_.Yawangle = inputCmd_.param4;
		outputRef_.Mode =0;
	}

	void readSysStatusMessage(const mms::Sys_status::ConstPtr& msg)
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
		//MMS_Handle();
	}

	void readPositionMessage(const guidance_node_amsl::Position::ConstPtr& msg)
	{
		inputPos_.Latitude=msg->Latitude;
		inputPos_.Longitude=msg->Longitude;
		inputPos_.AltitudeRelative = msg->AltitudeRelative;
		inputPos_.AltitudeAMSL = msg->AltitudeAMSL;
		inputPos_.Yawangle=msg->Yawangle;
		inputPos_.Timestamp=msg->Timestamp;
		inputPos_.Safety = msg->Safety;

		//MMS_Handle();
	}

	void readCmdMessage(const mms::Cmd::ConstPtr& msg)
	{
		ROS_INFO("CMD_RECEIVED");
		inputCmd_.command = msg -> command;
		inputCmd_.param1  = msg -> param1;
		inputCmd_.param2  = msg -> param2;
		inputCmd_.param3  = msg -> param3;
		inputCmd_.param4  = msg -> param4;
		inputCmd_.param5  = msg -> param5;
		inputCmd_.param6  = msg -> param6;
		inputCmd_.param7  = msg -> param7;

		/*Target_Position_.Latitude = inputCmd_.param5;
		Target_Position_.Longitude = inputCmd_.param6;
		Target_Position_.AltitudeRelative = inputCmd_.param7;
		Target_Position_.Yawangle = inputCmd_.param4;*/

		switch(inputCmd_.command)
		{
		case 16:  // MAV_CMD_NAV_WAYPOINT
		{
			ROS_INFO("MAV_CMD_DO_NAV_WAYPOINT");
			WAYPOINT = true;
			//MMS_Handle();
		} break;
		case 21:  // MAV_CMD_NAV_LAND
		{
			ROS_INFO("MAV_CMD_DO_NAV_LAND");
			LAND = true;
			//MMS_Handle();
		}break;
		case 22:  // MAV_CMD_NAV_TAKEOFF
		{
			ROS_INFO("MAV_CMD_NAV_TAKEOFF");
			TAKEOFF = true;
			//MMS_Handle();
		}break;
		/*		case 115: // MAV_CMD_CONDITION_YAW
		{
			CONDITION_YAW = true;
			Target_Position_.Yawangle = inputCmd_.param1;
			MMS_Handle(Target_Position_);
		}break;*/
		case 179: // MAV_CMD_DO_SET_HOME
		{
			ROS_INFO("MAV_CMD_DO_SET_HOME");
			SET_HOME = true;
			//MMS_Handle();
		}break;
		case 300: // MAV_CMD_MISSION_START
		{
			ROS_INFO("MAV_CMD_MISSION_START");
			MISSION_START = true;
			//MMS_Handle();
		}break;
		//MMS_Handle();
		}
	}

	/*void readAckArmMessage(const mms::Ack_arm::ConstPtr& msg)
	{
		ROS_INFO("ACK_ARM_RECEIVED");
		// inputAckArm_.new_mav_result = msg -> new_mav_result;
		inputAckArm_.mav_result = msg -> mav_result;
		//if (new_mav_result)
		//{
		New_MAV_RESULT = true;
		if (inputAckArm_.mav_result)
		{
			ROS_INFO("ARM/DISARM: EXECUTED");
			MAV_RESULT = true;
			//MMS_Handle();
		}
		else
		{
			ROS_INFO("ARM/DISARM:ERROR");
			MAV_RESULT = false;
			//MMS_Handle();
		}
		//}

	}*/

	class e_to_tartget{
	public:
		float error_pos = 0; // linear error
		float error_ang = 0; // angular error
	};
	e_to_tartget error_to_t;

	class ECEF{
	public:
		float Ne;
		float X;
		float Y;
		float Z;
	};
	ECEF End_Point;
	ECEF Starting_Point;

	void distance( guidance_node_amsl::Reference Target_Position, guidance_node_amsl::Position Current_Position) // e_to_tartget &error_to_t,
	{
		float error_x;
		float error_y;
		float error_z;
		float error_yaw;
		float alt;

		alt = Target_Position.AltitudeRelative*1e-3;// + Home.AltitudeAMSL*1e-3;
		End_Point.Ne=6378137/sqrt(1-0.08181919*0.08181919*sin(Target_Position.Latitude*1e-7*PI/180.0f)*sin(Target_Position.Latitude*1e-7*PI/180.0f));
		End_Point.X=(End_Point.Ne+alt)*cos(Target_Position.Latitude*1e-7*PI/180)*cos(Target_Position.Longitude*1e-7*PI/180);
		End_Point.Y=(End_Point.Ne+alt)*cos(Target_Position.Latitude*1e-7*PI/180)*sin(Target_Position.Longitude*1e-7*PI/180);
		End_Point.Z=(End_Point.Ne*(1-0.08181919*0.08181919)+alt)*sin(Target_Position.Latitude*1e-7*PI/180);

		alt = Current_Position.AltitudeRelative*1e-3;//AltitudeAMSL*1e-3;// AltitudeRelative + Home.AltitudeAMSL;
		Starting_Point.Ne=6378137/sqrt(1-0.08181919*0.08181919*sin(Current_Position.Latitude*1e-7*PI/180)*sin(Current_Position.Latitude*1e-7*PI/180));
		Starting_Point.X=(Starting_Point.Ne+alt)*cos(Current_Position.Latitude*1e-7*PI/180)*cos(Current_Position.Longitude*1e-7*PI/180);
		Starting_Point.Y=(Starting_Point.Ne+alt)*cos(Current_Position.Latitude*1e-7*PI/180)*sin(Current_Position.Longitude*1e-7*PI/180);
		Starting_Point.Z=(Starting_Point.Ne*(1-0.08181919*0.08181919)+alt)*sin(Current_Position.Latitude*1e-7*PI/180);

		error_x = End_Point.X - Starting_Point.X; //Target_Position.Latitude - Current_Position.Latitude;
		error_y = End_Point.Y - Starting_Point.Y; //Target_Position.Longitude - Current_Position.Longitude;
		error_z = End_Point.Z - Starting_Point.Z; //Target_Position.AltitudeRelative - (Current_Position.AltitudeAMSL-Home.AltitudeAMSL);
		error_yaw = Target_Position.Yawangle - Current_Position.Yawangle;
		error_to_t.error_pos = 1000*sqrt(error_x*error_x + error_y*error_y + error_z*error_z);
		error_to_t.error_ang = 180/PI*sqrt(error_yaw*error_yaw);
                
                ROS_INFO("lin dist [mm], %f, ang dist [deg], %f", error_to_t.error_pos, error_to_t.error_ang);
	}

	void set_events_false()
	{
		// INPUTS GCS -> MMS
		SET_HOME = false;
		TAKEOFF = false;
		LAND = false;
		MISSION_START = false;
		WAYPOINT = false;
		// static bool CONDITION_YAW = false;
		// MAV_RESULT  = false;
		//ARMED = false;
		//DISARMED = false;
	}

	void MMS_Handle()
	{
		switch(currentState)

		{

		case ON_GROUND_NO_HOME:
			//ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_NO_HOME");
			/*outputRef_.Latitude = 0;
			outputRef_.Longitude = 0;
			outputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			outputRef_.Yawangle = 0;
			outputRef_.Mode = 100;
			get_current_position();
			outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
			pubToReference_.publish(outputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/
			if (ARMED)
			{
				set_events_false();
				ARMED = true;

				get_current_position();
				outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
				pubToReference_.publish(outputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");

				outputArm_.arm_disarm = false;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
				ROS_INFO("MMS->APM: DISARMING");

				currentState = ON_GROUND_NO_HOME;
				ROS_INFO("MMS_CURRENT_STATE: ON_GROUND_NO_HOME");
				break;
			}
			if (SET_HOME)
			{
				set_events_false();
				currentState = SETTING_HOME;
				ROS_INFO("MMS_CURRENT_STATE: SETTING_HOME");
				break;
			}
			//}
			break;

		case SETTING_HOME:
			//ROS_INFO("MMS_CURRENT_STATE:SETTING_HOME");
			/*outputRef_.Latitude = 0;
			outputRef_.Longitude = 0;
			outputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			outputRef_.Yawangle = 0;
			outputRef_.Mode = 100;
			get_current_position();
			outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
			pubToReference_.publish(outputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			Home.Latitude = inputPos_.Latitude;
			Home.Longitude = inputPos_.Longitude;
			Home.AltitudeAMSL = inputPos_.AltitudeAMSL;
			Home.AltitudeRelative = inputPos_.AltitudeRelative;
			Home.Yawangle = inputPos_.Yawangle;
			ROS_INFO("Home AMSL, %d, rel, %d", Home.AltitudeAMSL, Home.AltitudeRelative);
			currentState = ON_GROUND_DISARMED;
			ROS_INFO("MMS_CURRENT_STATE: ON_GROUND_DISARMED");
			break;

		case ON_GROUND_DISARMED:
			// ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_DISARMED");

			/*outputRef_.Latitude = 0;
			outputRef_.Longitude = 0;
			outputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			outputRef_.Yawangle = 0;
			outputRef_.Mode = 100;
			get_current_position();
			outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
			pubToReference_.publish(outputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			if (TAKEOFF)
			{
				set_events_false();
				TAKEOFF = true;
				ARMED = false;

				/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
				outputRef_.Yawangle = 0;
				outputRef_.Mode = 100;*/
				get_current_position();
				outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
				pubToReference_.publish(outputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");

				outputArm_.arm_disarm = true;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
				ROS_INFO("MMS->APM: ARMING");

				currentState = ARMING;
				ROS_INFO("MMS_CURRENT_STATE: ARMING");
				break;
			}
			if (SET_HOME)
			{
				set_events_false();

				outputAckCmd_.mission_item_reached = true;
				outputAckCmd_.mav_mission_accepted = false;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

				currentState = SETTING_HOME;
				ROS_INFO("MMS_CURRENT_STATE: SETTING_HOME");
				break;
			}
			break;

		case ARMING:
			/* get_current_position();
			outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
			pubToReference_.publish(outputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			if (LAND)
			{
				set_events_false();
				ARMED = true;

				get_current_position();
				outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
				pubToReference_.publish(outputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");

				outputArm_.arm_disarm = false;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
				ROS_INFO("MMS->APM: DISARMING");

				currentState = DISARMING;
				ROS_INFO("MMS_CURRENT_STATE: DISARMING");
				break;
			}
			if (ARMED)
			{
				ROS_INFO("!!! DRONE ARMED !!!");
				set_events_false();
				TAKEOFF = true; // to complete automatic TO

				currentState = ON_GROUND_ARMED;
				ROS_INFO("MMS_CURRENT_STATE: ON_GROUND_ARMED");
				break;
			}
		
		        break;

		case DISARMING:
			/*get_current_position();
			outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
			pubToReference_.publish(outputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			if (ARMED == false)
			{
				ROS_INFO("DRONE DISARMED!");
				set_events_false();

				currentState = ON_GROUND_DISARMED;
				ROS_INFO("MMS_CURRENT_STATE: ON_GROUND_DISARMED");
			}
	        
	                break;

case ON_GROUND_ARMED:
	/*get_current_position();
	outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
	pubToReference_.publish(outputRef_);
	ROS_INFO("PUSHING THE DRONE DOWN ...");*/

	//ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_ARMED");
	if (LAND)
	{
		set_events_false();
		ARMED = true;

		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = true;
		pubToArm_.publish(outputArm_);
		ROS_INFO("MMS->APM: DISARMING");

		currentState = DISARMING;
		ROS_INFO("MMS_CURRENT_STATE: DISARMING");
		break;
	}
	if (TAKEOFF)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = ON_GROUND_READY_TO_TAKEOFF;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_TAKEOFF");
		break;
	}

	break;

case ON_GROUND_READY_TO_TAKEOFF:
	// ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_READY_TO_TAKEOFF");
	/*get_current_position();
	outputRef_.AltitudeRelative = outputRef_.AltitudeRelative-5000;
	pubToReference_.publish(outputRef_);
	ROS_INFO("PUSHING THE DRONE DOWN ...");*/

	if (LAND)
	{
		set_events_false();
		ARMED = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = true;
		pubToArm_.publish(outputArm_);
		ROS_INFO("MMS->APM: DISARMING");

		currentState = DISARMING;
		ROS_INFO("MMS_CURRENT_STATE: DISARMING");
		break;
	}
	if (MISSION_START)
	{
		set_events_false();

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 1000;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		outputRef_.AltitudeRelative = outputRef_.AltitudeRelative+Dh_TO;
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = TAKEOFF");

		currentState = PERFORMING_TAKEOFF;
		ROS_INFO("MMS_CURRENT_STATE:PERFORMING_TAKEOFF");
		break;
	}
	break;

case PERFORMING_TAKEOFF:
	//ROS_INFO("MMS_CURRENT_STATE:PERFORMING_TAKEOFF");
	distance( outputRef_, inputPos_); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_LAND;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");

		break;
	}
	if (error_to_t.error_pos < eps_TO and error_to_t.error_ang < eps_YAW)
		//if (abs(inputPos_.AltitudeRelative - Dh_TO) < eps_TO and error_to_t.error_ang < eps_YAW)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = true;
		outputAckCmd_.mav_mission_accepted = false;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

		currentState = IN_FLIGHT;
		ROS_INFO("MMS_CURRENT_STATE: IN_FLIGHT");
		break;
	}
	break;

case IN_FLIGHT:
	//ROS_INFO("MMS_CURRENT_STATE:IN_FLIGHT");
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_LAND;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_GO;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_GO");
		break;
	}
	break;

case READY_TO_GO:
	// ROS_INFO("MMS_CURRENT_STATE:READY_TO_GO");
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_LAND;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_GO;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_GO");

		break;
	}
	if (MISSION_START)
	{
		set_events_false();

		get_target_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = TARGET WAYPOINT");

		currentState = PERFORMING_GO_TO;
		ROS_INFO("MMS_CURRENT_STATE: PERFORMING_GO_TO");

		break;
	}
	break;

case PERFORMING_GO_TO:
	// ROS_INFO("MMS_CURRENT_STATE:PERFOMING_GO_TO");
	distance( outputRef_, inputPos_); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_LAND;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_GO;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_GO");

		break;
	}
	if (error_to_t.error_pos < eps_WP and error_to_t.error_ang < eps_YAW)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = true;
		outputAckCmd_.mav_mission_accepted = false;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

		currentState = IN_FLIGHT;
		ROS_INFO("MMS_CURRENT_STATE: IN_FLIGHT");
		break;
	}

	break;

case READY_TO_LAND:
	//ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");
	if (MISSION_START)
	{
		set_events_false();
		LAND = true;

		outputRef_.Latitude = 0;
		outputRef_.Longitude = 0;
		outputRef_.AltitudeRelative = -50; // 5 cm @ frequencey
		outputRef_.Yawangle = 0;
		outputRef_.Mode = 100;
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = VERT. LAND SPEED");

		currentState = PERFORMING_LANDING;
		ROS_INFO("MMS_CURRENT_STATE: PERFORMING_LANDING");
	}
	break;

case PERFORMING_LANDING:
	// ROS_INFO("MMS_CURRENT_STATE:PERFORMING_LANDING");

	if (inputPos_.AltitudeRelative - outputRef_.AltitudeRelative > 3000)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = true;
		outputAckCmd_.mav_mission_accepted = false;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

		currentState = ON_GROUND_ARMED;
		ROS_INFO("MMS_CURRENT_STATE: ON_GROUND_ARMED");
	}
	break;
}

}

void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		// ROS_INFO("MMS running");

		MMS_Handle();
		ros::spinOnce();

		loop_rate.sleep();
	}
}

protected:
/*state here*/
ros::NodeHandle n_;

ros::Subscriber subFromPosition_;
ros::Subscriber subFromCmd_;
ros::Subscriber subFromAckArm_;
ros::Subscriber subFromSysStatus_;

guidance_node_amsl::Position inputPos_;
mms::Cmd inputCmd_;
mms::Ack_arm inputAckArm_;
mms::Sys_status inputSysStatus_;

ros::Publisher pubToAckCmd_;
ros::Publisher pubToArm_;
ros::Publisher pubToReference_;

guidance_node_amsl::Reference outputRef_;
// guidance_node_amsl::Reference LVP_;
mms::Arm outputArm_;
mms::Ack_cmd outputAckCmd_;

// guidance_node_amsl::Reference Target_Position_;
guidance_node_amsl::Position Home;

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

// ERRORS DEFINITION
// float err; // current distance to the target position in millimeters
/*int eps_LAND = 100; // target distance to the LAND position in millimeter
	int eps_WP = 100; // target distance to the WAYPOINT position in millimeters
	int eps_TO = 100; // target distance to the TAKEOFF position in millimeters*/

int rate = 10;

//std::vector<double> param_;
//double debugParam;

private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mms");
	ros::NodeHandle node;

	MmsNodeClass mmsNode(node);

	mmsNode.run();
	return 0;
}
