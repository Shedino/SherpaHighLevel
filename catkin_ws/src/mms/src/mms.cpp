#include "ros/ros.h"
// #include <vector>

//#include "guidance_node_amsl/Reference.h" // input
//#include "guidance_node_amsl/Position_nav.h" // input
#include "mms/Cmd.h" // input
//#include "mms/Ack_cmd.h"// output
#include "mms/Ack_mission.h"// output
#include "mms/Arm.h"// output
#include "mms/Ack_arm.h" // input
#include "mms/Sys_status.h"// input
#include <mavros/Sonar.h> // input
#include "mms/MMS_status.h"// output
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
		// subFromPosition_=n_.subscribe("/position_nav", 10, &MmsNodeClass::readPositionMessage,this);
		subFromCmd_=n_.subscribe("/command", 10, &MmsNodeClass::readCmdMessage,this);
        subFromSonar_ = n_.subscribe("/sonar", 10, &MmsNodeClass::readSonarMessage,this);
		subFromSysStatus_=n_.subscribe("/system_status", 10, &MmsNodeClass::readSysStatusMessage,this);
		// subFromReference_=n_.subscribe("/reference", 10, &MmsNodeClass::readReferenceMessage,this);
		subFromDistance_=n_.subscribe("/distance", 10, &MmsNodeClass::readDistanceMessage,this);
		
		// publishers
//		pubToAckCmd_=n_.advertise<mms::Ack_cmd>("/ack_cmd", 10);
		pubToAckMission_=n_.advertise<mms::Ack_mission>("/ack_mission", 10);
		pubToArm_=n_.advertise<mms::Arm>("/arm", 10);
		pubToMmsStatus_=n_.advertise<mms::MMS_status>("/mms_status", 10);

		//Initializing outputAckMission_
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.mav_mission_accepted = false;
		outputAckMission_.seq = 0;

//		//Initializing outputAckCmd_
//		outputAckCmd_.command = 0;
//		outputAckCmd_.mav_command_accepted = false;

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
	}

	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	/*void readPositionMessage(const guidance_node_amsl::Position_nav::ConstPtr& msg)
	{
		inputPos_.Latitude = msg->Latitude;
		inputPos_.Longitude = msg->Longitude;
		inputPos_.Altitude = msg->Altitude;
		inputPos_.YawAngle = msg->YawAngle;
		inputPos_.Timestamp = msg->Timestamp;
	}*/

	/*void readReferenceMessage(const guidance_node_amsl::Reference::ConstPtr& msg)
	{
		inputRef_.Latitude = msg->Latitude;
		inputRef_.Longitude = msg->Longitude;
		inputRef_.AltitudeRelative = msg->AltitudeRelative;
		inputRef_.Yawangle = msg->Yawangle;
		// inputRef_.Timestamp = msg->Timestamp;
		inputRef_.frame = msg->frame;
	}*/

/*	void check_cmd()
	{
		if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputCmd_.param7 > 0 && inputCmd_.param7 < 3000 && inputSonar_.distance != -1))
			{
				outputAckCmd_.mission_item_reached = false;
				outputAckCmd_.mav_mission_accepted = true;
				outputAckCmd_.mav_cmd_id = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_ACCEPTED");
				ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
				target_frame = inputCmd_.frame;
			}
			else
			{
				outputAckCmd_.mission_item_reached = false;
				outputAckCmd_.mav_mission_accepted = false;
				outputAckCmd_.mav_cmd_id = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				ROS_INFO("MMS->GCS: FRAME_NOT_ACCEPTED");
				inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
				target_frame = 6;
		    }
	}*/

	void readCmdMessage(const mms::Cmd::ConstPtr& msg)
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

		/*Target_Position_.Latitude = inputCmd_.param5;
		Target_Position_.Longitude = inputCmd_.param6;
		Target_Position_.AltitudeRelative = inputCmd_.param7;
		Target_Position_.Yawangle = inputCmd_.param4;*/

		//6 = MAV_FRAME_GLOBAL_RELATIVE_ALT_INT
		//11 = MAV_FRAME_GLOBAL_TERRAIN_ALT_INT
        // check_cmd();//inputCmd_, inputSonar_);

		switch(inputCmd_.command)
		{
		case 16:  // MAV_CMD_NAV_WAYPOINT
		{
			ROS_INFO("MMS: CMD_WAYPOINT. Params: %f - %f - %f - %f",inputCmd_.param5,inputCmd_.param6,inputCmd_.param7,inputCmd_.param4);
			/*target_.Latitude = (int)(inputCmd_.param5*10000000.0f);
			target_.Longitude = (int)(inputCmd_.param6*10000000.0f);
			target_.AltitudeRelative = (int)(inputCmd_.param7*1000.0f);
			target_.Yawangle = inputCmd_.param4;
			target_.Mode = 0;*/
			ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
			ROS_INFO("MMS: CMD_ALTITUDE = %f",inputCmd_.param7);
			ROS_INFO("MMS: SONAR DIST. =% d",inputSonar_.distance);

			if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputCmd_.param7 > 0.0f && inputCmd_.param7 < 3.0f && inputSonar_.distance != -1))
				{
					/*outputAckCmd_.mav_command_accepted = true;
					outputAckCmd_.command = inputCmd_.command;
					pubToAckCmd_.publish(outputAckCmd_);
					ROS_INFO("MMS->GCS: CMD_ACCEPTED");*/
					// ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
					/*outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = true;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
					target_frame = inputCmd_.frame;
					seq_number = inputCmd_.seq;
					WAYPOINT = true;
				}
				else
				{
					/*				outputAckCmd_.mav_command_accepted = false;
									outputAckCmd_.command = inputCmd_.command;
									pubToAckCmd_.publish(outputAckCmd_);
									ROS_INFO("MMS: FRAME_NOT_ACCEPTED");
									ROS_INFO("MMS->GCS: CMD_NOT_ACCEPTED");*/
					outputAckMission_.mission_item_reached = false;
					outputAckMission_.seq = seq_number;
					outputAckMission_.mav_mission_accepted = false;
					pubToAckMission_.publish(outputAckMission_);
					ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");

					inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
					target_frame = 6;
					WAYPOINT = false;
			    }

			//MMS_Handle();
		} break;
		case 21:  // MAV_CMD_NAV_LAND
		{
			ROS_INFO("MMS: CMD_LAND");
			ROS_INFO("MMS: CMD_FRAME = %d", inputCmd_.frame);
			if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputSonar_.distance != -1))
			{
			/* outputAckCmd_.mav_command_accepted = true;
				outputAckCmd_.command = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: CMD_ACCEPTED");
				ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);*/
				/*outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = true;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
				// ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
				target_frame = inputCmd_.frame;
				seq_number = inputCmd_.seq;
				LAND = true;
			}
			else
			{
				/*				outputAckCmd_.mav_command_accepted = false;
				//				outputAckCmd_.command = inputCmd_.command;
				//				pubToAckCmd_.publish(outputAckCmd_);
				//				ROS_INFO("MMS: FRAME_NOT_ACCEPTED");
				//				ROS_INFO("MMS->GCS: CMD_NOT_ACCEPTED");*/
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = false;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				target_frame = 6;
			}
			//MMS_Handle();
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
				/* outputAckCmd_.mav_command_accepted = true;
				outputAckCmd_.command = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: CMD_ACCEPTED");
				 ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);*/
				/*outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = true;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
				// ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
				target_frame = inputCmd_.frame;
				seq_number = inputCmd_.seq;
				TAKEOFF = true;
			}
			else
			{
/*				outputAckCmd_.mav_command_accepted = false;
//				outputAckCmd_.command = inputCmd_.command;
//				pubToAckCmd_.publish(outputAckCmd_);
//				ROS_INFO("MMS: FRAME_NOT_ACCEPTED");
//				ROS_INFO("MMS->GCS: CMD_NOT_ACCEPTED");*/
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = seq_number;
				outputAckMission_.mav_mission_accepted = false;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				target_frame = 6;
				TAKEOFF = false;
			}
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
			ROS_INFO("MMS: CMD_SET_HOME");
			SET_HOME = true;

			/*outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("MMS->GCS: CMD_ACCEPTED");*/
			// ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
			/*outputAckMission_.mission_item_reached = false;
			outputAckMission_.seq = seq_number;
			outputAckMission_.mav_mission_accepted = true;
			pubToAckMission_.publish(outputAckMission_);
			ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
			seq_number = inputCmd_.seq;
			//MMS_Handle();
		}break;
		case 300: // MAV_CMD_MISSION_START
		{
			ROS_INFO("MMS: CMD_MISSION_START");
			MISSION_START = true;

			/*outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("MMS->GCS: CMD_ACCEPTED");*/
			// ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
//			outputAckMission_.mission_item_reached = false;
//			outputAckMission_.seq = seq_number;
//			outputAckMission_.mav_mission_accepted = true;
//			pubToAckMission_.publish(outputAckMission_);
//			ROS_INFO("MMS->GCS: MISSION_ITEM_ACCEPTED");
			//MMS_Handle();
		}break;
//		default:
//		{
//			/*outputAckCmd_.mav_command_accepted = false;
//			outputAckCmd_.command = inputCmd_.command;
//			pubToAckCmd_.publish(outputAckCmd_);
//			ROS_INFO("MMS: CMD_ID_NOT_ACCEPTED");
//			ROS_INFO("MMS->GCS: CMD_NOT_ACCEPTED");*/
//			outputAckMission_.mission_item_reached = false;
//			outputAckMission_.seq = seq_number;
//			outputAckMission_.mav_mission_accepted = false;
//			pubToAckMission_.publish(outputAckMission_);
//			ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
//		}
		//MMS_Handle();
		}
	}

	/*class e_to_tartget{
	public:
		double error_pos; // linear error
		double error_ang; // angular error
	};
	e_to_tartget error_to_t;

	class ECEF{
	public:
		double Ne;
		double X;
		double Y;
		double Z;
	};

	ECEF End_Point;
	ECEF Starting_Point;

	void distance() // e_to_tartget &error_to_t, // guidance_node_amsl::Reference Target_Position, guidance_node_amsl::Position Current_Position
	{
		static double error_x;
		static double error_y;
		static double error_z;
		static double error_yaw;
		static double alt;

		alt = inputRef_.AltitudeRelative/1000.0f;// + Home.AltitudeAMSL*1e-3;
		End_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(inputRef_.Latitude/10000000.0f*PI/180.0f)*sin(inputRef_.Latitude/10000000.0f*PI/180.0f));
		End_Point.X=(End_Point.Ne+alt)*cos(inputRef_.Latitude/10000000.0f*PI/180.0f)*cos(inputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Y=(End_Point.Ne+alt)*cos(inputRef_.Latitude/10000000.0f*PI/180.0f)*sin(inputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Z=(End_Point.Ne*(1.0f-0.08181919*0.08181919)+alt)*sin(inputRef_.Latitude*1e-7f*PI/180.0f);
        //ROS_INFO("ne, %f, endp_x, %f, endp_y, %f ,endp_z, %f", End_Point.Ne,End_Point.X, End_Point.Y,End_Point.Z);
		
        alt = ((double)inputPos_.Altitude)/1000.0f;//AltitudeAMSL*1e-3;// AltitudeRelative + Home.AltitudeAMSL;
		Starting_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f));
		Starting_Point.X=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*cos(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Y=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Z=(Starting_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f);
		//ROS_INFO("ne, %f, sp_x, %f, sp_y, %f ,sp_z, %f, alt %d", Starting_Point.Ne,Starting_Point.X, Starting_Point.Y,Starting_Point.Z,inputPos_.Altitude);

		error_x = End_Point.X - Starting_Point.X; //inputRef_.Latitude - inputPos_.Latitude;
		error_y = End_Point.Y - Starting_Point.Y; //inputRef_.Longitude - inputPos_.Longitude;
		error_z = End_Point.Z - Starting_Point.Z; //inputRef_.AltitudeRelative - (inputPos_.Altitude-Home.AltitudeAMSL);
		//ROS_INFO("error_x, %f, error_y, %f ,error_z, %f", error_x,error_y, error_z);
		
		error_yaw = inputRef_.Yawangle - inputPos_.YawAngle;//*3.14/100/360;
		error_to_t.error_pos = 1000.0f*sqrt(error_x*error_x + error_y*error_y + error_z*error_z);
		error_to_t.error_ang = 180.0f/PI*sqrt(error_yaw*error_yaw);
        //ROS_INFO("DISTANCE TO TARGET")
		
		counter_print++;
		if (counter_print >= 10){
			counter_print = 0;
			ROS_INFO("DISTANCE TO TARGET: Linear [mm], %f, Angular [deg], %f", error_to_t.error_pos, error_to_t.error_ang);
		}
	}*/

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
			outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
			pubToMmsStatus_.publish(outputMmsStatus_);
			ROS_INFO_ONCE("MMS->REF: CURRENT_STATE = ON_GROUND_NO_HOME");
			/*inputRef_.Latitude = 0;
			inputRef_.Longitude = 0;
			inputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			inputRef_.Yawangle = 0;
			inputRef_.Mode = 100;
			get_current_position();
			inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
			pubToReference_.publish(inputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/
			if (ARMED)
			{
				set_events_false();
				ARMED = true;

				/*get_current_position();
				inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
				pubToReference_.publish(inputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");*/

				outputArm_.arm_disarm = false;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
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
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
				break;
			}
			break;

		case SETTING_HOME:
			/*outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;
			pubToMmsStatus_.publish(outputMmsStatus_);*/
			//ROS_INFO("MMS_CURRENT_STATE:SETTING_HOME");
			/*inputRef_.Latitude = 0;
			inputRef_.Longitude = 0;
			inputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			inputRef_.Yawangle = 0;
			inputRef_.Mode = 100;
			get_current_position();
			inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
			pubToReference_.publish(inputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			/*Home.lat = inputGlobPosInt_.lat;
			Home.lon = inputGlobPosInt_.lon;
			Home.alt = inputGlobPosInt_.alt;
			Home.relative_alt = inputGlobPosInt_.relative_alt;
			Home.hdg = inputGlobPosInt_.hdg;
			ROS_INFO("HOME POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %d",Home.lat, Home.lon, Home.relative_alt, Home.hdg);
			//ROS_INFO("Home AMSL, %d, rel, %d", Home.alt, Home.relative_alt);*/
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
			break;

		case ON_GROUND_DISARMED:
			/*outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;
			pubToMmsStatus_.publish(outputMmsStatus_);*/
			// ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_DISARMED");

			/*inputRef_.Latitude = 0;
			inputRef_.Longitude = 0;
			inputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
			inputRef_.Yawangle = 0;
			inputRef_.Mode = 100;
			get_current_position();
			inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
			pubToReference_.publish(inputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			if (TAKEOFF)
			{
				set_events_false();
				TAKEOFF = true;
				ARMED = false;

				/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = -50; // mm @ frequency: verical speed
				inputRef_.Yawangle = 0;
				inputRef_.Mode = 100;*/
				/*get_current_position();
				inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
				pubToReference_.publish(inputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");*/
				/*outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = inputCmd_.seq;
				outputAckMission_.mav_mission_accepted = true;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

				outputArm_.arm_disarm = true;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
				ROS_INFO("MMS->APM: ARMING");

				counter_ = 0;     //start timing to rearm
				currentState = ARMING;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = ARMING");
				break;
			}
			if (SET_HOME)
			{
				set_events_false();

				/*outputAckCmd_.mission_item_reached = true;
				outputAckCmd_.mav_mission_accepted = false;
				outputAckCmd_.mav_cmd_id = 300;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = inputCmd_.seq;
				outputAckMission_.mav_mission_accepted = true;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

				currentState = SETTING_HOME;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = SETTING_HOME");
				break;
			}
			break;

		case ARMING:
			/*outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;
			pubToMmsStatus_.publish(outputMmsStatus_);*/
			/* get_current_position();
			inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
			pubToReference_.publish(inputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

			if (LAND)
			{
				set_events_false();
				ARMED = true;

				/*get_current_position();
				inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
				pubToReference_.publish(inputRef_);
				ROS_INFO("MMS->NAV: REFERENCE = ON_GROUND");*/
				outputAckMission_.mission_item_reached = false;
				outputAckMission_.seq = inputCmd_.seq;
				outputAckMission_.mav_mission_accepted = true;
				pubToAckMission_.publish(outputAckMission_);
				ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

				outputArm_.arm_disarm = false;
				outputArm_.new_arm_disarm = true;
				pubToArm_.publish(outputArm_);
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
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");
				break;
			}
			if (counter_>=50)    //5 seconds
			{
				currentState = ON_GROUND_DISARMED;
				outputMmsStatus_.mms_state = currentState;
				outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
				pubToMmsStatus_.publish(outputMmsStatus_);
				ROS_INFO("MMS: ARMING FAILED");
				ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_DISARMED");
			}
		    break;

		case DISARMING:
			/*outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;
			pubToMmsStatus_.publish(outputMmsStatus_);*/
			/*get_current_position();
			inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
			pubToReference_.publish(inputRef_);
			ROS_INFO("PUSHING THE DRONE DOWN ...");*/

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
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = 6;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	/*get_current_position();
	inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
	pubToReference_.publish(inputRef_);
	ROS_INFO("PUSHING THE DRONE DOWN ...");*/

	//ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_ARMED");
	if (LAND)
	{
		set_events_false();
		ARMED = true;

		/*outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = true;
		pubToArm_.publish(outputArm_);
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

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 22;
		pubToAckCmd_.publish(outputAckCmd_);*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = ON_GROUND_READY_TO_TAKEOFF;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = 6;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_TAKEOFF");
		break;
	}
	break;

case ON_GROUND_READY_TO_TAKEOFF:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = 6;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	// ROS_INFO("MMS_CURRENT_STATE:ON_GROUND_READY_TO_TAKEOFF");
	/*get_current_position();
	inputRef_.AltitudeRelative = inputRef_.AltitudeRelative-5000;
	pubToReference_.publish(inputRef_);
	ROS_INFO("PUSHING THE DRONE DOWN ...");*/

	if (LAND)
	{
		set_events_false();
		ARMED = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = true;
		pubToArm_.publish(outputArm_);
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

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 1000;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();
		inputRef_.AltitudeRelative = inputRef_.AltitudeRelative+Dh_TO;
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = TAKEOFF");*/

		currentState = PERFORMING_TAKEOFF;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_TAKEOFF");
		break;
	}
	break;

case PERFORMING_TAKEOFF:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	//ROS_INFO("MMS_CURRENT_STATE:PERFORMING_TAKEOFF");
	//distance(); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

		break;
	}
//	if (error_to_t.error_pos < eps_TO and error_to_t.error_ang < eps_YAW)
	if (inputDist_.command == 22 && seq_number == inputDist_.seq)
	{
		ROS_INFO_ONCE("MMS: REACHING THE TAKEOFF TARGET");
		if (inputDist_.error_pos < eps_TO and inputDist_.error_ang < eps_YAW and inputDist_.error_alt < eps_alt)
		{
			set_events_false();

			/*outputAckCmd_.mission_item_reached = true;
			outputAckCmd_.mav_mission_accepted = false;
			outputAckCmd_.mav_cmd_id = 22;
			pubToAckCmd_.publish(outputAckCmd_);*/
			outputAckMission_.mission_item_reached = true;
			outputAckMission_.seq = seq_number;
			outputAckMission_.mav_mission_accepted = false;
			pubToAckMission_.publish(outputAckMission_);
			ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

			currentState = IN_FLIGHT;
			outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
			pubToMmsStatus_.publish(outputMmsStatus_);
			ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");

			// RESET THE ERROR TO TARGET
			/*inputDist_.error_pos = eps_TO+eps_WP+eps_LAND;
			inputDist_.error_ang = 3*eps_YAW;
			ROS_INFO("MMS: ERROR TO TARGET RESET");*/
			break;
		}
	}
	break;

case IN_FLIGHT:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	//ROS_INFO("MMS_CURRENT_STATE:IN_FLIGHT");
	if (LAND)
	{
		set_events_false();
		LAND = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = seq_number;
		outputAckMission_.mav_mission_accepted = true;
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS-REF: CURRENT_STATE = READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();
		//WAYPOINT = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
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
	break;

case READY_TO_GO:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	// ROS_INFO("MMS_CURRENT_STATE:READY_TO_GO");
	if (LAND)
	{
		set_events_false();
		LAND = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();  //TODO check not used
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/
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

		//get_target_position();
		/*pubToReference_.publish(target_);
		inputRef_ = target_;
		ROS_INFO("MMS->NAV: REFERENCE = TARGET WAYPOINT");*/

		currentState = PERFORMING_GO_TO;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_GO_TO");

		break;
	}
	break;

case PERFORMING_GO_TO:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	/*if (counter_print>=9){
		ROS_INFO("MMS PERFOMING_GO_TO");
	}*/
	// ROS_INFO("MMS_CURRENT_STATE:PERFOMING_GO_TO");
//	distance(); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/
		outputAckMission_.mission_item_reached = false;
		outputAckMission_.seq = inputCmd_.seq;
		outputAckMission_.mav_mission_accepted = true;
		pubToAckMission_.publish(outputAckMission_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		currentState = READY_TO_LAND;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = READY_TO_LAND");

		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();   //TODO check not used
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/
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

//	if (error_to_t.error_pos < eps_WP && error_to_t.error_ang < eps_YAW)
	if (inputDist_.command == 16  && seq_number == inputDist_.seq)
	{
		ROS_INFO_ONCE("MMS: REACHING THE WAYPOINT TARGET");
		if (inputDist_.error_pos < eps_WP and inputDist_.error_ang < eps_YAW and inputDist_.error_alt < eps_alt)
		{
			set_events_false();

			/*outputAckCmd_.mission_item_reached = true;
			outputAckCmd_.mav_mission_accepted = false;
			outputAckCmd_.mav_cmd_id = 16;*/
			outputAckMission_.mission_item_reached = true;
			outputAckMission_.seq = seq_number;
			outputAckMission_.mav_mission_accepted = false;
			pubToAckMission_.publish(outputAckMission_);
//			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");

			currentState = IN_FLIGHT;
			outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
			pubToMmsStatus_.publish(outputMmsStatus_);
			ROS_INFO("MMS->REF: CURRENT_STATE = IN_FLIGHT");

			// RESET THE ERROR TO TARGET
			/*inputDist_.error_pos = eps_TO+eps_WP+eps_LAND;
			inputDist_.error_ang = 3*eps_YAW;
			ROS_INFO("MMS: ERROR TO TARGET RESET");*/
			break;
		}
	}

	break;

case READY_TO_LAND:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	//ROS_INFO("MMS_CURRENT_STATE: READY_TO_LAND");
	if (MISSION_START)
	{
		set_events_false();
		LAND = true;

		/*inputRef_.Latitude = inputPos_.Latitude;
		inputRef_.Longitude = inputPos_.Longitude;
		inputRef_.AltitudeRelative = inputPos_.Altitude; // 5 cm @ frequencey
		inputRef_.Yawangle = inputPos_.YawAngle;//*3.14/100/360;
		inputRef_.Mode = 0;*/
		//pubToReference_.publish(inputRef_);
		//ROS_INFO("MMS->NAV: REFERENCE = VERT. LAND SPEED");

		currentState = PERFORMING_LANDING;
		outputMmsStatus_.mms_state = currentState;
		outputMmsStatus_.target_ref_frame = target_frame;//inputCmd_.frame;
		pubToMmsStatus_.publish(outputMmsStatus_);
		ROS_INFO("MMS->REF: CURRENT_STATE = PERFORMING_LANDING");
		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		/*outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");*/

		/*inputRef_.Latitude = 0;
				inputRef_.Longitude = 0;
				inputRef_.AltitudeRelative = 0;
	            inputRef_.Yawangle = 0;
	            inputRef_.Mode = 100;*/
		/*get_current_position();  //TODO check not used
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");*/
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
	break;

case PERFORMING_LANDING:
	/*outputMmsStatus_.mms_state = currentState;
	outputMmsStatus_.target_ref_frame = target_frame;
	pubToMmsStatus_.publish(outputMmsStatus_);*/
	// ROS_INFO("MMS_CURRENT_STATE:PERFORMING_LANDING");

/*		//inputRef_.Latitude = inputPos_.Latitude;
		//inputRef_.Longitude = inputPos_.Longitude;
		inputRef_.AltitudeRelative -= 80; // 5 cm @ frequencey
		//inputRef_.Yawangle = 0;
		//inputRef_.Mode = 0;
		pubToReference_.publish(inputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = VERT. LAND SPEED");*/

//	if (inputPos_.Altitude - inputRef_.AltitudeRelative > eps_LAND)
	if (inputDist_.command == 21  && seq_number == inputDist_.seq)
	{
		ROS_INFO_ONCE("MMS: REACHING THE LANDING TARGET");
		if (inputDist_.error_alt > eps_LAND) //inputDist_.error_pos > eps_LAND and
		{
			set_events_false();
			LAND = true;

			/*outputAckCmd_.mission_item_reached = true;
			outputAckCmd_.mav_mission_accepted = false;
			outputAckCmd_.mav_cmd_id = 21;*/
			/*outputAckMission_.mission_item_reached = true;
			outputAckMission_.seq = seq_number;
			outputAckMission_.mav_mission_accepted = false;
			pubToAckMission_.publish(outputAckMission_);
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("MMS->GCS: MISSION_ITEM_REACHED");*/

			currentState = ON_GROUND_ARMED;
			outputMmsStatus_.mms_state = currentState;
			outputMmsStatus_.target_ref_frame = 6;//target_frame;//inputCmd_.frame;
			pubToMmsStatus_.publish(outputMmsStatus_);
			ROS_INFO("MMS->REF: CURRENT_STATE = ON_GROUND_ARMED");

			// RESET THE ERROR TO TARGET
			/*inputDist_.error_pos = eps_TO+eps_WP+eps_LAND;
			inputDist_.error_ang = 3*eps_YAW;
			ROS_INFO("MMS: ERROR TO TARGET RESET");*/
		}
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
mms::Cmd inputCmd_;

ros::Subscriber subFromAckArm_;
mms::Ack_arm inputAckArm_;

ros::Subscriber subFromSysStatus_;
mms::Sys_status inputSysStatus_;

ros::Subscriber subFromSonar_;
mavros::Sonar inputSonar_;

/*ros::Subscriber subFromReference_;
guidance_node_amsl::Reference inputRef_;
guidance_node_amsl::Reference target_;*/

ros::Subscriber subFromDistance_;
reference::Distance inputDist_;

// Publishers
ros::Publisher pubToAckMission_;
mms::Ack_mission outputAckMission_;

ros::Publisher pubToArm_;
mms::Arm outputArm_;

//ros::Publisher pubToAckCmd_;
//mms::Ack_cmd outputAckCmd_;

ros::Publisher pubToMmsStatus_;
mms::MMS_status outputMmsStatus_;

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
