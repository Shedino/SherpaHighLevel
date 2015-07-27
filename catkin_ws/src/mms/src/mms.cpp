#include "ros/ros.h"
// #include <vector>

//#include "guidance_node_amsl/Position.h"
#include "mavros/Global_position_int.h"
#include "guidance_node_amsl/Reference.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mms/Cmd.h"
#include "mms/Ack_cmd.h"
#include "mms/Arm.h"
#include "mms/Ack_arm.h"
#include "mms/Sys_status.h"
//#include <mavros/Sonar.h>


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

double PI = 3.1416; // pi
double eps_LAND = 10000.0; // distance to the target LAND position in millimeter
double eps_WP = 1500.0; // distance to the target WAYPOINT position in millimeters
double eps_TO = 1500.0; // distance to the target TAKEOFF position in millimeters
double eps_YAW = 20.0; // distance to the target YAW position in deg
double Dh_TO = 2000.0; // takeoff height above the groung in millimeters

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
		//subFromPosition_=n_.subscribe("/global_position_int", 10, &MmsNodeClass::readPositionMessage,this);
		subFromPosition_=n_.subscribe("/position_nav", 10, &MmsNodeClass::readPositionMessage,this);
		subFromGlobPosInt_=n_.subscribe("/global_position_int", 10, &MmsNodeClass::readGlobalPosIntMessage,this);
		subFromCmd_=n_.subscribe("/command", 10, &MmsNodeClass::readCmdMessage,this);
        //subFromSonar_ = n_.subscribe("/sonar", 10, &MmsNodeClass::readSonarMessage,this);
		// subFromAckArm_=n_.subscribe("/acknowledge_arming", 10, &MmsNodeClass::readAckArmMessage,this);
		subFromSysStatus_=n_.subscribe("/system_status", 10, &MmsNodeClass::readSysStatusMessage,this);
		
		// publishers
		pubToAckCmd_=n_.advertise<mms::Ack_cmd>("/ack_cmd", 10);
		//pubToSysStatus_=n_.advertise<mms::Sys_status>("/system_status", 10);
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
		outputAckCmd_.mav_cmd_id = 0;

		//Initializing outputArm_
		outputArm_.arm_disarm = false;
		outputArm_.new_arm_disarm = false;
	}

	/*void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		//ROS_INFO("MMS: SONAR_MSG_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}*/

	void get_current_position()
	{
		outputRef_.Latitude = inputPos_.Latitude;
		outputRef_.Longitude = inputPos_.Longitude;
		outputRef_.AltitudeRelative = inputPos_.Altitude;//inputPos_.Altitude - Home.AltitudeAMSL;
		outputRef_.Yawangle = inputPos_.YawAngle;//*3.14/100/360;
		outputRef_.Mode = 0;
		//ROS_INFO("CURRENT POSITION")
		ROS_INFO("CURRENT POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %f", inputPos_.Latitude, inputPos_.Longitude, inputPos_.Altitude, inputPos_.YawAngle);
        // ROS_INFO("pos AMSL, %d, Home AMSL, %d, rel, %d", inputPos_.Altitude, Home.alt, outputRef_.AltitudeRelative);
	}

	void get_target_position()
	{
		outputRef_.Latitude = (int)inputCmd_.param5*10000000.0f;
		outputRef_.Longitude = (int)inputCmd_.param6*10000000.0f;
		outputRef_.AltitudeRelative = (int)inputCmd_.param7*1000.0f;// - Home.AltitudeAMSL;
		outputRef_.Yawangle = inputCmd_.param4;
		outputRef_.Mode =0;
		//ROS_INFO("TARGET POSITION")
		ROS_INFO("TARGET POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %f", outputRef_.Latitude, outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle);
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

	//void readPositionMessage(const mavros::Global_position_int::ConstPtr& msg)
	void readPositionMessage(const guidance_node_amsl::Position_nav::ConstPtr& msg)
	{
		inputPos_.Latitude = msg->Latitude;
		inputPos_.Longitude = msg->Longitude;
		// inputPos_.Altitude = msg->Altitude;
		inputPos_.Altitude = msg->Altitude;
		inputPos_.YawAngle = msg->YawAngle;
		inputPos_.Timestamp = msg->Timestamp;

		//MMS_Handle();
	}
	
	void readGlobalPosIntMessage(const mavros::Global_position_int::ConstPtr& msg)
	{
		inputGlobPosInt_.lat = msg->lat;
		inputGlobPosInt_.lon = msg->lon;
		inputGlobPosInt_.alt = msg->alt;
		inputGlobPosInt_.relative_alt = msg->relative_alt;
		inputGlobPosInt_.hdg = msg->hdg;
		inputGlobPosInt_.time_boot_ms = msg->time_boot_ms;

		//MMS_Handle();
	}
	
	/*void check_frame (mms::Cmd inputCmd_,mavros::Sonar inputSonar_){
		if ((inputCmd_.frame == 6) || (inputCmd_.frame == 11 && inputSonar_.distance > 0))     
			{
				outputAckCmd_.mission_item_reached = false;
				outputAckCmd_.mav_mission_accepted = true;
				outputAckCmd_.mav_cmd_id = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_ACCEPTED");
				outputSysStatus_.valid_ref_frame = inputCmd_.frame;
				pubToSysStatus_.publish(outputSysStatus_);
				ROS_INFO("MMS: VALID_MAV_FRAME = %d", inputCmd_.frame);
			}
			else
			{
				outputAckCmd_.mission_item_reached = false;
				outputAckCmd_.mav_mission_accepted = false;
				outputAckCmd_.mav_cmd_id = inputCmd_.command;
				pubToAckCmd_.publish(outputAckCmd_);
				ROS_INFO("MMS->GCS: MISSION_ITEM_NOT_ACCEPTED");
				inputCmd_.command = 0; // 0 = NOT USED it's used to disable the switch-case structure
		}
	}*/

	void readCmdMessage(const mms::Cmd::ConstPtr& msg)
	{
		ROS_INFO("MMS: CMD_RECEIVED");
		inputCmd_.command = msg -> command;
		inputCmd_.param1  = msg -> param1;
		inputCmd_.param2  = msg -> param2;
		inputCmd_.param3  = msg -> param3;
		inputCmd_.param4  = msg -> param4;
		inputCmd_.param5  = msg -> param5;
		inputCmd_.param6  = msg -> param6;
		inputCmd_.param7  = msg -> param7;
        inputCmd_.frame  = msg -> frame;

		/*Target_Position_.Latitude = inputCmd_.param5;
		Target_Position_.Longitude = inputCmd_.param6;
		Target_Position_.AltitudeRelative = inputCmd_.param7;
		Target_Position_.Yawangle = inputCmd_.param4;*/

		//6 = MAV_FRAME_GLOBAL_RELATIVE_ALT_INT
		//11 = MAV_FRAME_GLOBAL_TERRAIN_ALT_INT
		

		switch(inputCmd_.command)
		{
		case 16:  // MAV_CMD_NAV_WAYPOINT
		{
			ROS_INFO("MAV_CMD_DO_NAV_WAYPOINT. Params: %f - %f - %f - %f",inputCmd_.param5,inputCmd_.param6,inputCmd_.param7,inputCmd_.param4);
			target_.Latitude = (int)(inputCmd_.param5*10000000.0f);
			target_.Longitude = (int)(inputCmd_.param6*10000000.0f);
			target_.AltitudeRelative = (int)(inputCmd_.param7*1000.0f);
			target_.Yawangle = inputCmd_.param4;
			target_.Mode = 0;
			//check_frame(inputCmd_, inputSonar_);

            		/*if (inputCmd_.frame == 11)     //11 = MAV_FRAME_GLOBAL_TERRAIN_ALT_INT
			{
				if (sonar_ok)
 				{
					WAYPOINT = true;
				}
				else
				{
					outputAckCmd_.mission_item_reached = false;
					outputAckCmd_.mav_mission_accepted = false;
					outputAckCmd_.mav_cmd_id = inputCmd_.command;
					pubToAckCmd_.publish(outputAckCmd_);
					ROS_INFO("MMS->GCS: MISSION_NOT_ACCEPTED");
                                        WAYPOINT = false;
				}
			} 
                        else // any of the remaining frames
			{				
			        WAYPOINT = true;
			}*/			
			//MMS_Handle();
		} break;
		case 21:  // MAV_CMD_NAV_LAND
		{
			ROS_INFO("MAV_CMD_DO_NAV_LAND");
			LAND = true;
			//check_frame(inputCmd_, inputSonar_);
			//MMS_Handle();
		}break;
		case 22:  // MAV_CMD_NAV_TAKEOFF
		{
			ROS_INFO("MAV_CMD_NAV_TAKEOFF");
			TAKEOFF = true;
			//check_frame(inputCmd_, inputSonar_);
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
	guidance_node_amsl::Reference target_;

	void distance() // e_to_tartget &error_to_t, // guidance_node_amsl::Reference Target_Position, guidance_node_amsl::Position Current_Position
	{
		static double error_x;
		static double error_y;
		static double error_z;
		static double error_yaw;
		static double alt;

		alt = outputRef_.AltitudeRelative/1000.0f;// + Home.AltitudeAMSL*1e-3;
		End_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Latitude/10000000.0f*PI/180.0f));
		End_Point.X=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*cos(outputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Y=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Z=(End_Point.Ne*(1.0f-0.08181919*0.08181919)+alt)*sin(outputRef_.Latitude*1e-7f*PI/180.0f);
        //ROS_INFO("ne, %f, endp_x, %f, endp_y, %f ,endp_z, %f", End_Point.Ne,End_Point.X, End_Point.Y,End_Point.Z);
		
        alt = ((double)inputPos_.Altitude)/1000.0f;//AltitudeAMSL*1e-3;// AltitudeRelative + Home.AltitudeAMSL;
		Starting_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f));
		Starting_Point.X=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*cos(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Y=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Z=(Starting_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f);
		//ROS_INFO("ne, %f, sp_x, %f, sp_y, %f ,sp_z, %f, alt %d", Starting_Point.Ne,Starting_Point.X, Starting_Point.Y,Starting_Point.Z,inputPos_.Altitude);

		error_x = End_Point.X - Starting_Point.X; //outputRef_.Latitude - inputPos_.Latitude;
		error_y = End_Point.Y - Starting_Point.Y; //outputRef_.Longitude - inputPos_.Longitude;
		error_z = End_Point.Z - Starting_Point.Z; //outputRef_.AltitudeRelative - (inputPos_.Altitude-Home.AltitudeAMSL);
		//ROS_INFO("error_x, %f, error_y, %f ,error_z, %f", error_x,error_y, error_z);
		
		error_yaw = outputRef_.Yawangle - inputPos_.YawAngle;//*3.14/100/360;
		error_to_t.error_pos = 1000.0f*sqrt(error_x*error_x + error_y*error_y + error_z*error_z);
		error_to_t.error_ang = 180.0f/PI*sqrt(error_yaw*error_yaw);
        //ROS_INFO("DISTANCE TO TARGET")
		
		counter_print++;
		if (counter_print >= 10){
			counter_print = 0;
			ROS_INFO("DISTANCE TO TARGET: Linear [mm], %f, Angular [deg], %f", error_to_t.error_pos, error_to_t.error_ang);
		}
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

			Home.lat = inputGlobPosInt_.lat;
			Home.lon = inputGlobPosInt_.lon;
			Home.alt = inputGlobPosInt_.alt;
			Home.relative_alt = inputGlobPosInt_.relative_alt;
			Home.hdg = inputGlobPosInt_.hdg;
			ROS_INFO("HOME POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %f",Home.lat, Home.lon, Home.relative_alt, Home.hdg);
			//ROS_INFO("Home AMSL, %d, rel, %d", Home.alt, Home.relative_alt);
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

				counter_ = 0;     //start timing to rearm
				currentState = ARMING;
				ROS_INFO("MMS_CURRENT_STATE: ARMING");
				break;
			}
			if (SET_HOME)
			{
				set_events_false();

				outputAckCmd_.mission_item_reached = true;
				outputAckCmd_.mav_mission_accepted = false;
				outputAckCmd_.mav_cmd_id = 300;
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
			if (counter_>=50)    //5 seconds
			{
				currentState = ON_GROUND_DISARMED;
				ROS_INFO("MMS_ARMING FAILED. BACK TO STATE: ON_GROUND_DISARMED");
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
		outputAckCmd_.mav_cmd_id = 22;
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
		outputAckCmd_.mav_cmd_id = 21;
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
	distance(); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
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
		//if (abs(inputPos_.Altitude - Dh_TO) < eps_TO and error_to_t.error_ang < eps_YAW)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = true;
		outputAckCmd_.mav_mission_accepted = false;
		outputAckCmd_.mav_cmd_id = 22;
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
		outputAckCmd_.mav_cmd_id = 21;
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
		outputAckCmd_.mav_cmd_id = 16;
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
		outputAckCmd_.mav_cmd_id = 21;
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
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();  //TODO check not used
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_GO;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_GO");

		break;
	}
	if (MISSION_START)
	{
		set_events_false();

		//get_target_position();
		pubToReference_.publish(target_);
		outputRef_ = target_;
		ROS_INFO("MMS->NAV: REFERENCE = TARGET WAYPOINT");

		currentState = PERFORMING_GO_TO;
		ROS_INFO("MMS_CURRENT_STATE: PERFORMING_GO_TO");

		break;
	}
	break;

case PERFORMING_GO_TO:
	if (counter_print>=9){
		ROS_INFO("MMS PERFOMING_GO_TO");
	}
	// ROS_INFO("MMS_CURRENT_STATE:PERFOMING_GO_TO");
	distance(); // error_to_t,
	if (LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 21;
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
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();   //TODO check not used
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
		outputAckCmd_.mav_cmd_id = 16;
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

		outputRef_.Latitude = inputPos_.Latitude;
		outputRef_.Longitude = inputPos_.Longitude;
		outputRef_.AltitudeRelative = inputPos_.Altitude; // 5 cm @ frequencey
		outputRef_.Yawangle = inputPos_.YawAngle;//*3.14/100/360;
		outputRef_.Mode = 0;
		//pubToReference_.publish(outputRef_);
		//ROS_INFO("MMS->NAV: REFERENCE = VERT. LAND SPEED");

		currentState = PERFORMING_LANDING;
		ROS_INFO("MMS_CURRENT_STATE: PERFORMING_LANDING");
		break;
	}
	if (WAYPOINT)
	{
		set_events_false();

		outputAckCmd_.mission_item_reached = false;
		outputAckCmd_.mav_mission_accepted = true;
		outputAckCmd_.mav_cmd_id = 16;
		pubToAckCmd_.publish(outputAckCmd_);
		ROS_INFO("MMS->GCS: MISSION_ACCEPTED");

		/*outputRef_.Latitude = 0;
				outputRef_.Longitude = 0;
				outputRef_.AltitudeRelative = 0;
	            outputRef_.Yawangle = 0;
	            outputRef_.Mode = 100;*/
		get_current_position();  //TODO check not used
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = CURRENT POSITION");

		currentState = READY_TO_GO;
		ROS_INFO("MMS_CURRENT_STATE: READY_TO_GO");

		break;
	}
	break;

case PERFORMING_LANDING:
	// ROS_INFO("MMS_CURRENT_STATE:PERFORMING_LANDING");

		//outputRef_.Latitude = inputPos_.Latitude;
		//outputRef_.Longitude = inputPos_.Longitude;
		outputRef_.AltitudeRelative -= 80; // 5 cm @ frequencey
		//outputRef_.Yawangle = 0;
		//outputRef_.Mode = 0;
		pubToReference_.publish(outputRef_);
		ROS_INFO("MMS->NAV: REFERENCE = VERT. LAND SPEED");

	if (inputPos_.Altitude - outputRef_.AltitudeRelative > eps_LAND)
	{
		set_events_false();
		LAND = true;

		outputAckCmd_.mission_item_reached = true;
		outputAckCmd_.mav_mission_accepted = false;
		outputAckCmd_.mav_cmd_id = 21;
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
		counter_++;
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
ros::Subscriber subFromSonar_;
ros::Subscriber subFromGlobPosInt_;

guidance_node_amsl::Position_nav inputPos_;
mavros::Global_position_int inputGlobPosInt_;

// mavros::Sonar inputSonar_;
mms::Cmd inputCmd_;
mms::Ack_arm inputAckArm_;
mms::Sys_status inputSysStatus_;

ros::Publisher pubToAckCmd_;
ros::Publisher pubToArm_;
ros::Publisher pubToReference_;
// ros::Publisher pubToSysStatus_;

guidance_node_amsl::Reference outputRef_;
// guidance_node_amsl::Reference LVP_;
mms::Arm outputArm_;
mms::Ack_cmd outputAckCmd_;
// mms::Sys_status outputSysStatus_;
// guidance_node_amsl::Reference Target_Position_;
mavros::Global_position_int Home;                              //TODO HERE

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

bool sonar_ok;

//std::vector<double> param_;
//double debugParam;

private:

uint16_t counter_ = 0;
uint16_t counter_print = 0;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mms");
	ros::NodeHandle node;

	MmsNodeClass mmsNode(node);

	mmsNode.run();
	return 0;
}
