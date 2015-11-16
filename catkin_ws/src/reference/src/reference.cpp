#include "ros/ros.h"

#include "mavros/Global_position_int.h"
#include "guidance_node_amsl/Reference.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mms_msgs/Cmd.h"
#include "mms_msgs/Grid_ack.h"
#include "mms_msgs/MMS_status.h"
#include <mavros/Sonar.h>
#include <frame/Ref_system.h>
#include "reference/Distance.h"	
#include "reference/Grid_info.h"	//GRID
#include "reference/Grid_info.h"	//GRID
#include "WP_grid.h"         //GRID
#include "reference/LeashingCommand.h"   //leashing
#include "reference/LeashingStatus.h"    //leashing
#include "geographic_msgs/GeoPose.h"	 //leashing
#include <wgs84_ned_lib/wgs84_ned_lib.h>       

double eps_WP = 1500.0; // distance to the target WAYPOINT position in millimeters      //TODO not hardcoded and it is in both mms and here
double eps_alt = 500.0; // distance to the target altitude in millimeters
double eps_YAW = 20.0; // distance to the target YAW position in deg

double PI = 3.1416; // pi

class leashing_target_ned{
	public:
		double x;
		double y;
		double z;
};
class leashing_offset_ned{
	public:
		double x_offset;
		double y_offset;
		double z_offset;
		double rho_offset;
		double psi_offset;
};

class ReferenceNodeClass {
public:
	ReferenceNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromPosition_ = n_.subscribe("/position_nav", 10, &ReferenceNodeClass::readPositionMessage,this);
		subFromGlobPosInt_ = n_.subscribe("/global_position_int", 10, &ReferenceNodeClass::readGlobalPosIntMessage,this);
		subFromCmd_ = n_.subscribe("/sent_command", 10, &ReferenceNodeClass::readCmdMessage,this);     //excluding command verifier
        subFromSonar_ = n_.subscribe("/sonar", 10, &ReferenceNodeClass::readSonarMessage,this);
		subFromMmsStatus_ = n_.subscribe("/mms_status", 10, &ReferenceNodeClass::readMmsStatusMessage,this);
		subFromFrame_ = n_.subscribe("/ref_system", 10, &ReferenceNodeClass::readFrameMessage,this);
		subLeashingTargetPosition_ = n_.subscribe("/leashing_target_position", 10, &ReferenceNodeClass::readLeashingTarget,this);
		subLeashingCommand_ = n_.subscribe("/leashing_command", 10, &ReferenceNodeClass::readLeashingCommand,this);
		
		// publishers
		pubToReference_ = n_.advertise<guidance_node_amsl::Reference>("/reference",10);
		pubToDistance_ = n_.advertise<reference::Distance>("/distance",10);
		pubGridAck_ = n_.advertise<mms_msgs::Grid_ack>("/grid_ack",10);
		pubHome_ = n_.advertise<mavros::Global_position_int>("/home",5);
		pubGridInfo_ = n_.advertise<reference::Grid_info>("/grid_info",2);
		pubLeashingStatus_ = n_.advertise<reference::LeashingStatus>("/leashing_status",5);

		//Initializing outputRef_
		outputRef_.Latitude = 0;
		outputRef_.Longitude = 0;
		outputRef_.AltitudeRelative = 0;
		outputRef_.Yawangle = 0;
		outputRef_.Mode = 100;
		outputRef_.frame = 6;

		// Initializing the
		oldFrame_.actual_ref_frame = 6;
		oldFrame_.target_ref_frame = 6;

		inputPos_.frame =6;
		actual_frame = 6;
		inputMmsStatus_.target_ref_frame = 6;
		inputFrame_.target_ref_frame = 6;
		
		// STATE INITIALIZATION
		currentState = ON_GROUND_NO_HOME;
		oldState = 0;//ON_GROUND_NO_HOME;
		// int lastARMState = ON_GROUND_DISARMED;
		target_frame = 6;
		actual_frame = 6;
		tempRelAlt = 0;

		rate = 10;
		new_state = true;
		new_frame = true;
		land = false;
		counter_print = 0;
		Dh_TO = 5000;
		
		//GRID
		d_grid = 0;
		received_grid_cmd = false;
		waiting_for_vertex_grid = false;
		repeat_flag = false;
		vertex_grid_n = 0;
		received_vertexes_grid = 0;
		speed_grid = 0;
		height_grid = 0;
		vertex_grid = new float *[MAX_VERTEX_GRID];
		for(int i = 0; i<MAX_VERTEX_GRID; i++){
	    	vertex_grid[i] = new float[2];
		}
		success_grid = false;   //OUTPUT
		WP = new float *[150];        //OUTPUT    //TODO check hardcoded 150
		for(int i = 0; i<150; i++){
	    	WP[i] = new float[2];
		}
		N_WP = 0;      //OUTPUT
		WP_completed_grid = 0;
		waiting_for_WP_execution_grid = false;

		//LEASHING
		yaw_leashing = 0;
		leashing_status_.horizontal_control_mode = 0;  //none
		leashing_status_.horizontal_distance = 0;
		leashing_status_.horizontal_heading = 0;
		leashing_status_.distance_north = 0;
		leashing_status_.distance_east = 0;
		leashing_status_.vertical_control_mode = 0;  //none
		leashing_status_.vertical_distance = 0;
		leashing_status_.yaw_control_mode = 0;  //none
		leashing_status_.yaw = 0;
		//leashing_status_.yawpoint = ;   //TODO initialize better
	}

	class e_to_tartget{
	public:
		double error_pos; // linear error
		double error_ang; // angular error
		double error_alt; // altitude error
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
		static double error_alt;
		static double error_yaw;
		static double alt;

		alt = (double)outputRef_.AltitudeRelative/1000.0f;// 
		End_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Latitude/10000000.0f*PI/180.0f));
		End_Point.X=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*cos(outputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Y=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Longitude/10000000.0f*PI/180.0f);
		//End_Point.Z=(End_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(outputRef_.Latitude*1e-7f*PI/180.0f);
		End_Point.Z = (double)outputRef_.AltitudeRelative;     //MICHELE BRUTAL!! buahahaha
        //ROS_INFO("ne, %f, endp_x, %f, endp_y, %f ,endp_z, %f", End_Point.Ne,End_Point.X, End_Point.Y,End_Point.Z);

		alt = (double)inputPos_.Altitude/1000.0f;//AltitudeAMSL*1e-3;
		Starting_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f));
		Starting_Point.X=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*cos(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Y=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Longitude/10000000.0f*PI/180);
		//Starting_Point.Z=(Starting_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f);
		Starting_Point.Z = (double)inputPos_.Altitude;		//MIC brutal! buahaha
		//ROS_INFO("ne, %f, sp_x, %f, sp_y, %f ,sp_z, %f, alt %d", Starting_Point.Ne,Starting_Point.X, Starting_Point.Y,Starting_Point.Z,inputPos_.Altitude);

		error_x = End_Point.X - Starting_Point.X; //outputRef_.Latitude - inputPos_.Latitude;
		error_y = End_Point.Y - Starting_Point.Y; //outputRef_.Longitude - inputPos_.Longitude;
		error_z = End_Point.Z - Starting_Point.Z; //outputRef_.AltitudeRelative - (inputPos_.Altitude);
		//ROS_INFO("error_x, %f, error_y, %f ,error_z, %f", error_x,error_y, error_z);

		error_yaw = outputRef_.Yawangle - inputPos_.YawAngle;//*3.14/100/360;
		error_to_t.error_pos = 1000.0f*sqrt(error_x*error_x + error_y*error_y);
		error_to_t.error_ang = 180.0f/PI*sqrt(error_yaw*error_yaw);
		//error_to_t.error_alt = 1000.0f*sqrt(error_z*error_z);
		error_to_t.error_alt = sqrt(error_z*error_z);      //MIC brutal! buahaha
        //ROS_INFO("DISTANCE TO TARGET")

		counter_print++;
		if (counter_print >= 30){
			counter_print = 0;
			ROS_INFO("DISTANCE TO TARGET: Linear [mm], %f, Angular [deg], %f", error_to_t.error_pos, error_to_t.error_ang);
		}
	}

	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		//ROS_INFO("MMS: SONAR_MSG_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	void get_current_position()
	{
		outputRef_.Latitude = inputPos_.Latitude;
		outputRef_.Longitude = inputPos_.Longitude;
		outputRef_.AltitudeRelative = inputPos_.Altitude;
		outputRef_.Yawangle = inputPos_.YawAngle;//*3.14/100/360;
		outputRef_.Mode = 0;
		ROS_INFO("REF: CURRENT POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %f", inputPos_.Latitude, inputPos_.Longitude, inputPos_.Altitude, inputPos_.YawAngle);
	}

	void readMmsStatusMessage(const mms_msgs::MMS_status::ConstPtr& msg)
	{
		inputMmsStatus_.mms_state=msg->mms_state;
		inputMmsStatus_.target_ref_frame=msg->target_ref_frame;
		//ROS_INFO("REF: MMS_status received %d", inputMmsStatus_.mms_state);
		currentState = inputMmsStatus_.mms_state;
		//new_state = true;
	}

	void readFrameMessage(const frame::Ref_system::ConstPtr& msg)
	{
		inputFrame_.actual_ref_frame=msg->actual_ref_frame;
		inputFrame_.target_ref_frame=msg->target_ref_frame;
		// ROS_INFO("REFERENCE: Ref_system received");
		actual_frame = inputFrame_.actual_ref_frame;
		target_frame = inputFrame_.target_ref_frame;
        //new_frame = true;
	}

	//void readPositionMessage(const mavros::Global_position_int::ConstPtr& msg)
	void readPositionMessage(const guidance_node_amsl::Position_nav::ConstPtr& msg)
	{
		inputPos_.Latitude = msg->Latitude;
		inputPos_.Longitude = msg->Longitude;
		inputPos_.Altitude = msg->Altitude;
		inputPos_.YawAngle = msg->YawAngle;
		inputPos_.Timestamp = msg->Timestamp;
		inputPos_.frame = msg->frame;
		//new_pos = true;
	}
	
	void readGlobalPosIntMessage(const mavros::Global_position_int::ConstPtr& msg)
	{
		inputGlobPosInt_.lat = msg->lat;
		inputGlobPosInt_.lon = msg->lon;
		inputGlobPosInt_.alt = msg->alt;
		inputGlobPosInt_.relative_alt = msg->relative_alt;
		inputGlobPosInt_.hdg = msg->hdg;
		inputGlobPosInt_.time_boot_ms = msg->time_boot_ms;
	}
	
	void readLeashingTarget(const geographic_msgs::GeoPose::ConstPtr& msg){
		leashing_target_ = *msg;
		double temp_x, temp_y;
		get_pos_NED_from_WGS84 (&temp_x, &temp_y, leashing_target_.position.latitude, leashing_target_.position.longitude, Home_.lat/10000000.0f, Home_.lon/10000000.0f);
		leashing_target_ned_.x = temp_x;
		leashing_target_ned_.y = temp_y;
		leashing_target_ned_.z = leashing_target_.position.altitude;
	}
	
	void readLeashingCommand(const reference::LeashingCommand::ConstPtr& msg){
		if (currentState == LEASHING) {
			leashing_command_ = *msg;
			
			if (leashing_command_.horizontal_control_mode != 0) leashing_status_.horizontal_control_mode = leashing_command_.horizontal_control_mode;
			if (leashing_command_.vertical_control_mode != 0) leashing_status_.vertical_control_mode = leashing_command_.vertical_control_mode;
			if (leashing_command_.yaw_control_mode != 0) leashing_status_.yaw_control_mode = leashing_command_.yaw_control_mode;
			leashing_status_.yawpoint = leashing_command_.yawpoint;
		}
	}
	
	/*void readLeashingStatus(const reference::LeashingStatus::ConstPtr& msg){
		if (currentState == LEASHING) leashing_status_ = *msg;
	}*/

	void readCmdMessage(const mms_msgs::Cmd::ConstPtr& msg)
	{
		ROS_INFO("REF: CMD_RECEIVED");
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

        // new_cmd = true;

		/*Target_Position_.Latitude = inputCmd_.param5;
		Target_Position_.Longitude = inputCmd_.param6;
		Target_Position_.AltitudeRelative = inputCmd_.param7;
		Target_Position_.Yawangle = inputCmd_.param4;*/

		//6 = MAV_FRAME_GLOBAL_ALT_INT
		//11 = MAV_FRAME_GLOBAL_TERRAIN_ALT_INT

		switch(inputCmd_.command)
		{
			case 16:  // MAV_CMD_NAV_WAYPOINT
			{
				// ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT");
				// ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %f - %f - %f - %f",inputCmd_.param5,inputCmd_.param6,inputCmd_.param7,inputCmd_.param4);
				target_.Latitude = (int)(inputCmd_.param5*10000000.0f);
				target_.Longitude = (int)(inputCmd_.param6*10000000.0f);
				target_.AltitudeRelative = (int)(inputCmd_.param7*1000.0f);
				target_.Yawangle = inputCmd_.param4;
				target_.frame = inputCmd_.frame;
				outputDist_.seq = inputCmd_.seq;
				ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %f - %d",target_.Latitude,target_.Longitude,target_.AltitudeRelative,target_.Yawangle,target_.frame);
				// target_.Mode = 0;
			} break;
			case 160:    //GRID_START
			{
				received_grid_cmd = true;
				waiting_for_vertex_grid = true;
				speed_grid = inputCmd_.param1;
				d_grid = inputCmd_.param2;
				height_grid = inputCmd_.param3;
				vertex_grid_n = inputCmd_.param4;
				repeat_flag = inputCmd_.param7 == 1 ? true : false; 
				//TODO add sanity checks (max vertexes, min vertexes, positive speed and height, ...)
			}break;
			case 161:    //GRID_VERTEX
			{
				for (int i = 0; i<3; i++){
					if (i == 0){
						vertex_grid[received_vertexes_grid][0] = inputCmd_.param1;       //LAT
						vertex_grid[received_vertexes_grid][1] = inputCmd_.param2;		 //LON
						ROS_INFO("REF: Vertex Received: %f - %f", inputCmd_.param1, inputCmd_.param2);
						received_vertexes_grid++;
					} else if (i == 1){
						vertex_grid[received_vertexes_grid][0] = inputCmd_.param3;
						vertex_grid[received_vertexes_grid][1] = inputCmd_.param4;
						ROS_INFO("REF: Vertex Received: %f - %f", inputCmd_.param3, inputCmd_.param4);
						received_vertexes_grid++;
					} else if (i == 2){
						vertex_grid[received_vertexes_grid][0] = inputCmd_.param5;
						vertex_grid[received_vertexes_grid][1] = inputCmd_.param6;          //MAX 3 vertex per command
						ROS_INFO("REF: Vertex Received: %f - %f", inputCmd_.param5, inputCmd_.param6);
						received_vertexes_grid++;
					}
					if (received_vertexes_grid == vertex_grid_n){
						waiting_for_vertex_grid = false;       //received all the vertexes
						break;                      //exit for cycle, we have all vertexes
					}
				}
			}break;
			case 21:  // MAV_CMD_NAV_LAND
			{
				ROS_INFO("REF: MAV_CMD_DO_NAV_LAND");
				outputRef_.frame = inputCmd_.frame;
				outputDist_.seq = inputCmd_.seq;
			}break;
			case 22:  // MAV_CMD_NAV_TAKEOFF
			{
				ROS_INFO("REF: MAV_CMD_NAV_TAKEOFF");
				outputRef_.Latitude = (int)(inputCmd_.param5*10000000.0f);
				outputRef_.Longitude = (int)(inputCmd_.param6*10000000.0f);
				outputRef_.AltitudeRelative = (int)(inputCmd_.param7*1000.0f);
				outputRef_.Yawangle = inputCmd_.param4;
				outputRef_.frame = inputCmd_.frame;
				outputDist_.seq = inputCmd_.seq;
				Dh_TO = outputRef_.AltitudeRelative;
			}break;
			/*		case 115: // MAV_CMD_CONDITION_YAW
			{
				CONDITION_YAW = true;
			}break;*/
			case 179: // MAV_CMD_DO_SET_HOME
			{
				ROS_INFO("REF: MAV_CMD_DO_SET_HOME");
			}break;
			case 300: // MAV_CMD_MISSION_START
			{
				ROS_INFO("REF: MAV_CMD_MISSION_START");
			}break;
			case 25: // MAV_CMD_NAV_FOLLOW (LEASHING)
			{
				if (inputCmd_.param1 == 1){
					//initial offset
					//we should have already the target, but better add sanity check
					double temp_reference_x, temp_reference_y;
					get_pos_NED_from_WGS84 (&temp_reference_x, &temp_reference_y, outputRef_.Latitude/10000000.0f, outputRef_.Longitude/10000000.0f, Home_.lat/10000000.0f, Home_.lon/10000000.0f);
					double temp_target_x, temp_target_y;
					get_pos_NED_from_WGS84 (&temp_target_x, &temp_target_y, leashing_target_.position.latitude, leashing_target_.position.longitude, Home_.lat/10000000.0f, Home_.lon/10000000.0f);
					leashing_offset_ned_.x_offset = temp_reference_x - temp_target_x;          //initial offset
					leashing_offset_ned_.y_offset = temp_reference_y - temp_target_y;
					leashing_offset_ned_.z_offset = outputRef_.AltitudeRelative/1000.0f - leashing_target_.position.altitude;
					if (leashing_offset_ned_.x_offset > 10 || leashing_offset_ned_.y_offset > 10 || leashing_offset_ned_.z_offset > 15){   //target-wasp too far
						//TODO maybe it is better to abort leashing
						ROS_INFO("REF: LEASHING target-wasp too far");
						leashing_offset_ned_.x_offset = 0;          //initial offset
						leashing_offset_ned_.y_offset = 0;
						leashing_offset_ned_.z_offset =	3;
					}
					leashing_offset_ned_.rho_offset = sqrt(pow(leashing_offset_ned_.x_offset,2)+pow(leashing_offset_ned_.y_offset,2));
					if (leashing_offset_ned_.rho_offset > 0.1){
						leashing_offset_ned_.psi_offset = atan2(leashing_offset_ned_.x_offset,leashing_offset_ned_.y_offset);
					} else {
						leashing_offset_ned_.psi_offset = 0;
					}
					yaw_leashing = outputRef_.Yawangle;    //take actual yaw
					
					/*leashing_offset_ned_.x_offset = 0;
					leashing_offset_ned_.y_offset = 0;
					leashing_offset_ned_.z_offset = 2;
					leashing_offset_ned_.rho_offset = 0;
					leashing_offset_ned_.psi_offset = 0;*/
				}
			}break;
		}
	}

	void Reference_Handle()
	{
		if (currentState != oldState)
		{
			new_state = true;
			new_frame = true;
			oldState = currentState;
			ROS_INFO("REF: NEW MMS STATE");
		}
		if (inputFrame_.actual_ref_frame != oldFrame_.actual_ref_frame || inputFrame_.target_ref_frame != oldFrame_.target_ref_frame)
		{
			new_frame = true;
			oldFrame_ = inputFrame_;
			ROS_INFO("REF: NEW FRAME");
		}

		switch(currentState)
		{
		case ON_GROUND_NO_HOME:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				//ROS_INFO("REF: FRAME COHERENCE OK");
				if (new_frame == true)
				{
					//ROS_INFO("REF: NEW_FRAME");
					if (new_state == true)
					{
						ROS_INFO("REF: ON_GROUND_NO_HOME");
						new_state = false;

						get_current_position();
						outputRef_.frame = actual_frame;
						tempRef_ = outputRef_;
						tempRelAlt = inputGlobPosInt_.alt;
						//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
						//pubToReference_.publish(outputRef_);
						ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
					}

					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: ON_GROUND BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: ON_GROUND SONAR");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRelAlt-5000-3;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
					}
				}
			}
			else
			{
				ROS_INFO("REF: FRAME COHERENCE NOT OK");
			}
			break;

		case SETTING_HOME:
			if (new_state == true)
			{
				new_state = false;
				
				ROS_INFO("REF: SETTING_HOME");

				Home_.lat = inputGlobPosInt_.lat;
				Home_.lon = inputGlobPosInt_.lon;
				Home_.alt = inputGlobPosInt_.alt;
				Home_.relative_alt = inputGlobPosInt_.relative_alt;
				Home_.hdg = inputGlobPosInt_.hdg;
				pubHome_.publish(Home_);
			}
			if (new_frame == true)
			{
				new_frame = false;
			}
			/*if (new_pos == true)
			{
				new_pos = false;
			}*/
			break;

		case ON_GROUND_DISARMED:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				//ROS_INFO("REF: FRAME COHERENCE OK");
				if (new_frame == true)
				{
					//ROS_INFO("REF: NEW_FRAME");
					if (new_state == true)
					{
						ROS_INFO("REF: ON_GROUND_DISARMED");
						new_state = false;

						get_current_position();
						outputRef_.frame = actual_frame;
						tempRef_ = outputRef_;
						tempRelAlt = inputGlobPosInt_.alt;
						//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
						//pubToReference_.publish(outputRef_);
						ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
					}

					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: ON_GROUND BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: ON_GROUND SONAR");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRelAlt-5000-3;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
					}
				}
			}
			else
			{
				ROS_INFO("REF: FRAME COHERENCE NOT OK");
			}
			break;

		case ARMING:
			if (new_state == true)
			{
				ROS_INFO("REF: ARMING");
				new_state = false;
			}
			/*if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
						{
							if (new_frame == true)
							{
								if (new_state == true)
								{
									new_state = false;

									get_current_position();
									outputRef_.frame = actual_frame;
									tempRef_ = outputRef_;
									tempRelAlt = inputGlobPosInt_.alt;  // 
									//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
									//pubToReference_.publish(outputRef_);
									ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
								}

								new_frame = false;

								if (actual_frame == 6 && target_frame == 6) // 6 = barometer
								{
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 11) // 11 = sonar
								{
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND SONAR");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 6 && target_frame == 11)
								{
									outputRef_.AltitudeRelative = tempRelAlt-5000-3;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 6)
								{
									ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
									ROS_INFO("REF: NO REFERENCE PUBLISHED");
								}
							}
						}*/
		    break;

		case DISARMING:
			if (new_state == true)
			{
				ROS_INFO("REF: DISARMING");
				new_state = false;
			}

			/*if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
						{
							if (new_frame == true)
							{
								if (new_state == true)
								{
									new_state = false;

									get_current_position();
									outputRef_.frame = actual_frame;
									tempRef_ = outputRef_;
									tempRelAlt = inputGlobPosInt_.alt;//
									//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
									//pubToReference_.publish(outputRef_);
									ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
								}

								new_frame = false;

								if (actual_frame == 6 && target_frame == 6) // 6 = barometer
								{
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 11) // 11 = sonar
								{
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND SONAR");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 6 && target_frame == 11)
								{
									outputRef_.AltitudeRelative = tempRelAlt-5000-3;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 6)
								{
									ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
									ROS_INFO("REF: NO REFERENCE PUBLISHED");
								}
							}
						}*/
			break;

		case ON_GROUND_ARMED:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
						{
							if (new_frame == true)
							{
								if (new_state == true)
								{
									ROS_INFO("REF: ON_GROUND_ARMED");
									new_state = false;

									get_current_position();
									outputRef_.frame = actual_frame;
									tempRef_ = outputRef_;
									tempRelAlt = inputGlobPosInt_.alt;
									//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
									//pubToReference_.publish(outputRef_);
									ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
								}

								new_frame = false;

								if (actual_frame == 6 && target_frame == 6) // 6 = barometer
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 11) // 11 = sonar
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND SONAR");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 6 && target_frame == 11)
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRelAlt-5000-3;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 6)
								{
									ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
									ROS_INFO("REF: NO REFERENCE PUBLISHED");
								}
							}
						}
			break;
		// this state is useless if MISSION_START is not implemented
		/*case ON_GROUND_READY_TO_TAKEOFF:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
						{
							if (new_frame == true)
							{
								if (new_state == true)
								{
									ROS_INFO("REF: ON_GROUND_READY_TO_TAKEOFF");
									new_state = false;

									get_current_position();
									outputRef_.frame = actual_frame;
									tempRef_ = outputRef_;
									tempRelAlt = inputGlobPosInt_.alt;
									//outputRef_.AltitudeRelative -= 5000;//outputRef_.AltitudeRelative-5000;
									//pubToReference_.publish(outputRef_);
									ROS_INFO("REF->NAV: REFERENCE = ON_GROUND");
								}

								new_frame = false;

								if (actual_frame == 6 && target_frame == 6) // 6 = barometer
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-1;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 11) // 11 = sonar
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRef_.AltitudeRelative-5000-2;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: ON_GROUND SONAR");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 6 && target_frame == 11)
								{
									outputRef_ = tempRef_;
									outputRef_.AltitudeRelative = tempRelAlt-5000-3;
									outputRef_.frame = actual_frame;
									ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
									pubToReference_.publish(outputRef_);
								}
								if (actual_frame == 11 && target_frame == 6)
								{
									ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
									ROS_INFO("REF: NO REFERENCE PUBLISHED");
								}
							}
						}
			break;*/

		case PERFORMING_TAKEOFF:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
					ROS_INFO("REF: PERFORMING_TAKEOFF");
					new_state = false;
					get_current_position();
					outputRef_.frame = actual_frame;
					tempRef_ = outputRef_;
					tempRelAlt = inputGlobPosInt_.alt;
					ROS_INFO("REF->NAV: REFERENCE = TAKEOFF");
				}
				if (new_frame == true)
				{
					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative+Dh_TO+1;
						tempRef_ = outputRef_;       //MIC ADDED
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: TAKEOFF BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = Dh_TO+2;
						tempRef_ = outputRef_;		//MIC ADDED
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: TAKEOFF SONAR");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRelAlt+Dh_TO+3;
						tempRef_ = outputRef_;		//MIC ADDED
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
					}
				}
				distance();
				outputDist_.error_pos = error_to_t.error_pos;
				outputDist_.error_ang = error_to_t.error_ang;
				outputDist_.error_alt = error_to_t.error_alt;
				outputDist_.command = 22; // TAKEOFF
				pubToDistance_.publish(outputDist_);
			}
			break;

		case IN_FLIGHT:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
					ROS_INFO("REF: IN_FLIGHT");
					new_state = false;
					// comment these lines to avoid the drone stops when the distance is inside the allowed error sphere
					// comment these lines to make the published reference equal to the last valid one
					/* get_current_position(); 
					outputRef_.frame = actual_frame;
					tempRef_ = outputRef_;
					tempRelAlt = inputGlobPosInt_.alt;*/
					ROS_INFO("REF->NAV: REFERENCE = IN_FLIGHT");
				}

				if (new_frame == true)
				{
					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is     
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is		
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						outputRef_.AltitudeRelative = tempRelAlt;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);				
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
					}
				}
			}
			break;
		// this state is useless if MISSION_START is not implemented
		/*case READY_TO_GO:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
					ROS_INFO("REF: READY_TO_GO");
					new_state = false;
					// comment these lines to avoid the drone accumulates position and yaw errors
					// comment these lines to make the published reference equal to the last valid one
					//get_current_position();
					//outputRef_.frame = actual_frame;
					//tempRef_ = outputRef_;
					//tempRelAlt = inputGlobPosInt_.alt;
					ROS_INFO("REF->NAV: REFERENCE = READY_TO_GO");
				}

				if (new_frame == true)
				{
					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						outputRef_.AltitudeRelative = tempRelAlt;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
					}
				}
			}
			break;*/

		case GRID:
			{
				if (received_grid_cmd && !waiting_for_vertex_grid){   //have received all vertexes
					ROS_INFO("REF: GRID. Starting GRID alg. N. vertex: %d - Distance: %f", vertex_grid_n, d_grid);
					for (int i = 0; i < vertex_grid_n; i++){
						double temp_x = 0;
						double temp_y = 0;
						get_pos_NED_from_WGS84 (&temp_x, &temp_y, vertex_grid [i][0], vertex_grid [i][1], Home_.lat/10000000.0f, Home_.lon/10000000.0f);
						vertex_grid [i][0] = temp_x;
						vertex_grid [i][1] = temp_y;
						//ROS_INFO("REF: GRID. Vertex %d: %f - %f", i+1, vertex_grid [i][0], vertex_grid [i][1]);
					}
					double temp_x_init = 0;
					double temp_y_init = 0;
					get_pos_NED_from_WGS84 (&temp_x_init, &temp_y_init, outputRef_.Latitude/10000000.0f, outputRef_.Longitude/10000000.0f, Home_.lat/10000000.0f, Home_.lon/10000000.0f); //latest reference as initial point
					initial_pos_grid[0] = temp_x_init;
					initial_pos_grid[1] = temp_y_init;
					WP_grid(vertex_grid, &vertex_grid_n, initial_pos_grid, d_grid, WP, &success_grid, &N_WP);      //CORE ALGORITHM
					
					double tot_distance = 0;
					tot_distance += sqrt(pow(initial_pos_grid[0]-WP[1][0],2)+pow(initial_pos_grid[1]-WP[1][1],2));  //distance from intitial position to first WP
					for (int i = 0; i<N_WP-1; i++){
						tot_distance += sqrt(pow(WP[i][0]-WP[i+1][0],2)+pow(WP[i][1]-WP[i+1][1],2));     //sum of all distances between WPs
					}
					double grid_exec_time = tot_distance / speed_grid;                  
					reference::Grid_info gridInfo;
					gridInfo.success = success_grid;
					gridInfo.N_WP = N_WP;
					gridInfo.exec_time = grid_exec_time;
					pubGridInfo_.publish(gridInfo);
					ROS_INFO("REF: GRID! Success: %d - N. WP: %d - speed: %f - Height: %f - Exec time: %f - Distance: %f", success_grid, N_WP, speed_grid, height_grid, grid_exec_time, tot_distance);
					/*ROS_INFO("REF: GRID! New vertex: %d", vertex_grid_n);
					for (int i = 0; i < N_WP; i++){
						ROS_INFO("REF: GRID! WP %d: %.2f - %.2f", i, WP[i][0], WP[i][1]);
					}*/
					received_grid_cmd = false;     //WP calculated. Now they need to be sent as reference
				}
				if (WP_completed_grid<N_WP && !waiting_for_WP_execution_grid && success_grid){           
					waiting_for_WP_execution_grid = true;
					double temp_ref_latitude;
					double temp_ref_longitude;
					get_pos_WGS84_from_NED (&temp_ref_latitude, &temp_ref_longitude, WP[WP_completed_grid][0], WP[WP_completed_grid][1], Home_.lat/10000000.0f, Home_.lon/10000000.0f);
					outputRef_.Latitude = (int)(temp_ref_latitude * 10000000.0f);
					outputRef_.Longitude = (int)(temp_ref_longitude * 10000000.0f);
					outputRef_.AltitudeRelative = height_grid * 1000.0f;  //yaw should be already the last target  //TODO maybe we can set yaw from mission, for example pointing in the direciton of flight
					outputRef_.frame = actual_frame;                 //TODO check this with Nicola
					ROS_INFO("REF->GRID: Sent a WP");
					pubToReference_.publish(outputRef_);
				} else if (waiting_for_WP_execution_grid && success_grid){
					//Waiting for the execution of the WP
					distance();
					outputDist_.error_pos = error_to_t.error_pos;
					outputDist_.error_ang = error_to_t.error_ang;
					outputDist_.error_alt = error_to_t.error_alt;
					outputDist_.command = 160; // GRID
					pubToDistance_.publish(outputDist_);
					if (outputDist_.error_pos < eps_WP && outputDist_.error_ang < eps_YAW && outputDist_.error_alt < eps_alt){
						//WP reached
						waiting_for_WP_execution_grid = false;
						WP_completed_grid++;
					}
				} else if (WP_completed_grid==N_WP && success_grid && !repeat_flag){ //completed grid and repeat_flag is off
					//GRID execution ended. Return EVENT to MMS
					grid_ack_.grid_completed = true;
					grid_ack_.completion_type = 1;      //success
					pubGridAck_.publish(grid_ack_);
					tempRef_ = outputRef_;
				} else if (WP_completed_grid==N_WP && success_grid && repeat_flag){  //completed grid but repeat_flag is on
					//START GRID again until termination command
					WP_completed_grid = 0;   //reset to first WP to start over
					ROS_INFO("REF->GRID: Restarting GRID because repeat_flag");
				} else if (!success_grid){
					//FAIL
					grid_ack_.grid_completed = true;
					grid_ack_.completion_type = 0;      //generic failure
					pubGridAck_.publish(grid_ack_);
					tempRef_ = outputRef_;
				}
			}
		break;

		case PERFORMING_GO_TO:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
					ROS_INFO("REF: PERFORMING_GO_TO");
					new_state = false;

					/*outputRef_.Latitude = 123; // ONLY FOR TEST
					outputRef_.Longitude = 123;// ONLY FOR TEST
					outputRef_.AltitudeRelative = 1230;// ONLY FOR TEST
					outputRef_.Yawangle = 123;// ONLY FOR TEST
					outputRef_.Mode = 123;// ONLY FOR TEST
					outputRef_.frame = target_frame;// ONLY FOR TEST*/

					// get_current_position();
					tempRef_ = target_;
					// tempRelAlt = inputGlobPosInt_.alt;//
					ROS_INFO("REF->NAV: REFERENCE = TARGET WAYPOINT");
				}
				if (new_frame == true)
				{
					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						// tempRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is
						ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %f - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
						land = false;
						ROS_INFO("REF->NAV: WAYPOINT BARO");
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						// tempRef_.frame = actual_frame;
						pubToReference_.publish(outputRef_);// as it is
						ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %f - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
						land = false;
						ROS_INFO("REF->NAV: WAYPOINT SONAR");
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.frame = actual_frame;
						//tempRef_.frame = actual_frame;
						outputRef_.AltitudeRelative = inputGlobPosInt_.alt;
						ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						pubToReference_.publish(outputRef_);
						ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %f - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
						land = true;
					}
					if (actual_frame == 11 && target_frame == 6)
					{
						ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
						ROS_INFO("REF: NO REFERENCE PUBLISHED");
						land = false;
					}
				}
				if (land == true)
				{
					outputRef_.AltitudeRelative -= 80;
					pubToReference_.publish(outputRef_);
				}
				distance();
				outputDist_.error_pos = error_to_t.error_pos;
				outputDist_.error_ang = error_to_t.error_ang;
				outputDist_.error_alt = error_to_t.error_alt;
				outputDist_.command = 16; // WAYPOINT
				pubToDistance_.publish(outputDist_);
			}
		break;

		case LEASHING:
			if (new_state == true){
				ROS_INFO("REF: LEASHING");
				new_state = false;
			}
			if (new_frame == true){
				new_frame = false;
			}
			switch (leashing_status_.horizontal_control_mode){
				case 0:	//HORIZONTAL_CONTROL_MODE_NONE
					//NEUTRAL MODE. DO NOTHING
					break;
				case 1:	//HORIZONTAL_CONTROL_MODE_KEEP_DISTANCE
					
					break;
				case 2:	//HORIZONTAL_CONTROL_MODE_DISTANCE_HEADING_ABSOLUTE
					//ROS_INFO("REF: LEASHING HORIZONTAL ABSOLUTE");
					leashing_offset_ned_.rho_offset = leashing_command_.horizontal_distance;
					leashing_offset_ned_.psi_offset = leashing_command_.horizontal_heading;
					leashing_offset_ned_.x_offset = leashing_offset_ned_.rho_offset * cos(leashing_offset_ned_.psi_offset);
					leashing_offset_ned_.y_offset = leashing_offset_ned_.rho_offset * sin(leashing_offset_ned_.psi_offset);
					break;
				case 3:	//HORIZONTAL_CONTROL_MODE_NORTH_EAST_ABSOLUTE
					//ROS_INFO("REF: LEASHING HORIZONTAL NE ABSOLUTE");
					leashing_offset_ned_.x_offset = leashing_command_.distance_north;
					leashing_offset_ned_.y_offset = leashing_command_.distance_east;
					leashing_offset_ned_.rho_offset = sqrt(pow(leashing_offset_ned_.x_offset,2)+pow(leashing_offset_ned_.y_offset,2));
					if (leashing_offset_ned_.rho_offset > 0.1){
						leashing_offset_ned_.psi_offset = atan2(leashing_offset_ned_.y_offset,leashing_offset_ned_.y_offset);
					} else {
						leashing_offset_ned_.psi_offset = 0;
					}
					break;
				case 4:	//HORIZONTAL_CONTROL_MODE_DISTANCE_HEADING_VEL
					//ROS_INFO("REF: LEASHING DISTANCE HEADING: %f - %f", leashing_command_.horizontal_distance_vel, leashing_command_.horizontal_heading_vel);
					leashing_offset_ned_.rho_offset += leashing_command_.horizontal_distance_vel * 0.2;  //max speed 2 m/s  //TODO check hardcoded
					if (leashing_offset_ned_.rho_offset < 0) leashing_offset_ned_.rho_offset = 0;   //cannot become negative
					if (leashing_offset_ned_.rho_offset > 0.1){
						leashing_offset_ned_.psi_offset += leashing_command_.horizontal_heading_vel * 0.2 / leashing_offset_ned_.rho_offset; //max tangential velocity 2 m/s -->normalized with rho //TODO check hardcoded
					} else {
						leashing_offset_ned_.psi_offset += 0;
					}
					if (leashing_offset_ned_.psi_offset >= 2*M_PI) leashing_offset_ned_.psi_offset -= 2*M_PI;    //reset every 2pi
					if (leashing_offset_ned_.psi_offset <= -2*M_PI) leashing_offset_ned_.psi_offset += 2*M_PI;    //reset every 2pi
					//ROS_INFO("REF: LEASHING offset: %f - %f", leashing_offset_ned_.rho_offset, leashing_offset_ned_.psi_offset);
					leashing_offset_ned_.x_offset = leashing_offset_ned_.rho_offset * cos(leashing_offset_ned_.psi_offset);
					leashing_offset_ned_.y_offset = leashing_offset_ned_.rho_offset * sin(leashing_offset_ned_.psi_offset);
					break;
				case 5:		//HORIZONTAL_CONTROL_MODE_NORTH_EAST_VEL
					//ROS_INFO("REF: LEASHING HORIZONTAL VEL NE");
					//TODO add yaw of the command issuer (rescuer)
					leashing_offset_ned_.x_offset += leashing_command_.distance_north_vel * 0.2;		//max speed 2 m/s  //TODO check hardcoded
					leashing_offset_ned_.y_offset += leashing_command_.distance_east_vel * 0.2;
					leashing_offset_ned_.rho_offset = sqrt(pow(leashing_offset_ned_.x_offset,2)+pow(leashing_offset_ned_.y_offset,2));
					if (leashing_offset_ned_.rho_offset > 0.1){
						leashing_offset_ned_.psi_offset = atan2(leashing_offset_ned_.y_offset,leashing_offset_ned_.x_offset);
					} else {
						leashing_offset_ned_.psi_offset = 0;
					}
					break;
			}
			
			switch (leashing_status_.vertical_control_mode){
				case 0:		//VERTICAL_CONTROL_MODE_NONE

					break;
				case 1:		//VERTICAL_CONTROL_MODE_KEEP
					//WHAT HERE?? TODO
					break;
				case 2:		//VERTICAL_CONTROL_MODE_ABSOLUTE
					leashing_offset_ned_.z_offset = leashing_command_.vertical_distance;
					break;
				case 3:		//VERTICAL_CONTROL_MODE_VEL
					leashing_offset_ned_.z_offset += leashing_command_.vertical_distance_vel * 0.1; //max speed 1 m/s  //TODO check hardcoded   //TODO add check with terrain altitude
					break;
			}
			
			switch (leashing_status_.yaw_control_mode){
				case 0:	//YAW_CONTROL_MODE_NONE
					yaw_leashing = outputRef_.Yawangle;    //take actual yaw
					break;
				case 1:	//YAW_CONTROL_MODE_ABSOLUTE
					yaw_leashing = leashing_command_.yaw;
					break;
				case 2:	//YAW_CONTROL_MODE_VEL
					yaw_leashing += leashing_command_.yaw_vel * 0.3;   //3 rad/s as maximum rotational velocity //TODO check hardcoded
					break;
				case 3:		//YAW_CONTROL_MODE_TOWARDS_POINT
					double temp_point_x, temp_point_y;
					get_pos_NED_from_WGS84 (&temp_point_x, &temp_point_y, leashing_command_.yawpoint.latitude, leashing_command_.yawpoint.longitude, Home_.lat/10000000.0f, Home_.lon/10000000.0f);
					yaw_leashing = atan2(temp_point_y-(leashing_target_ned_.y+leashing_offset_ned_.y_offset), temp_point_x-(leashing_target_ned_.x+leashing_offset_ned_.x_offset));   //atan of vector point-wasp_reference, where wasp_reference is target+offset for leashing
					break;
				case 4:		//YAW_CONTROL_MODE_TOWARDS_ANCHOR
					if (leashing_offset_ned_.rho_offset > 0.1){
						yaw_leashing = atan2(leashing_offset_ned_.y_offset, leashing_offset_ned_.x_offset) + M_PI;
					} else {
						//do nothing. Do not change yaw when leashing distance is too small because atan can be close to undefined
					}
					break;
				case 5:		//YAW_CONTROL_MODE_AWAY_FROM_ANCHOR
					if (leashing_offset_ned_.rho_offset > 0.1){
						yaw_leashing = atan2(leashing_offset_ned_.y_offset, leashing_offset_ned_.x_offset);
					} else {
						//do nothing. Do not change yaw when leashing distance is too small because atan can be close to undefined
					}
					break;
			}
			//Publish references in WGS84
			double temp_lat, temp_lon;
			get_pos_WGS84_from_NED (&temp_lat, &temp_lon, (leashing_target_ned_.x+leashing_offset_ned_.x_offset), (leashing_target_ned_.y+leashing_offset_ned_.y_offset), Home_.lat/10000000.0f, Home_.lon/10000000.0f);
			outputRef_.Latitude = temp_lat*10000000.0f;
			outputRef_.Longitude = temp_lon*10000000.0f;
			outputRef_.AltitudeRelative = (leashing_target_ned_.z + leashing_offset_ned_.z_offset)*1000.0f;
			outputRef_.Yawangle = yaw_leashing;
			pubToReference_.publish(outputRef_);
			tempRef_ = outputRef_;
			//ROS_INFO("REF: LEASHING: %d - %f - %f - %f - %f", leashing_status_.horizontal_control_mode, leashing_offset_ned_.x_offset, leashing_offset_ned_.y_offset, leashing_offset_ned_.rho_offset, leashing_offset_ned_.psi_offset);
			leashing_status_.horizontal_distance = leashing_offset_ned_.rho_offset;
			leashing_status_.horizontal_heading = leashing_offset_ned_.psi_offset;
			leashing_status_.distance_north = leashing_offset_ned_.x_offset;
			leashing_status_.distance_east = leashing_offset_ned_.y_offset;
			leashing_status_.vertical_distance = leashing_offset_ned_.z_offset;
			leashing_status_.yaw = yaw_leashing;
			pubLeashingStatus_.publish(leashing_status_);  //TODO maybe modify this to include failures
			break;
		
		case PERFORMING_LANDING:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
						ROS_INFO("REF: PERFORMING_LANDING");
						new_state = false;
						get_current_position();                      //this is needed because I can send the land command before last WP or target is reached
						outputRef_.frame = actual_frame;
						tempRef_ = outputRef_;
						tempRelAlt = inputGlobPosInt_.alt;
						ROS_INFO("REF->NAV: REFERENCE = VERT. LAND SPEED");
				}
				if (new_frame == true)
				{
						new_frame = false;

						if (actual_frame == 6 && target_frame == 6) // 6 = barometer
						{
							outputRef_.frame = actual_frame;// as it is
						}
						if (actual_frame == 11 && target_frame == 11) // 11 = sonar
						{
							outputRef_.frame = actual_frame;// as it is
						}
						if (actual_frame == 6 && target_frame == 11)
						{
							outputRef_.frame = actual_frame;
							outputRef_.AltitudeRelative = tempRelAlt;
							ROS_INFO("REF->NAV: WARNING! REF CONVERTED TO BARO");
						}
						if (actual_frame == 11 && target_frame == 6)
						{
							ROS_INFO("REF: !!! SYSTEM ERROR !!! actual = 11; target = 6");
							ROS_INFO("REF: NO REFERENCE PUBLISHED");
						}
				}
				outputRef_.AltitudeRelative -= 80; // 8 cm @ frequency --> with rate=10 it means 80cm/s
				tempRelAlt -= 80;
				pubToReference_.publish(outputRef_);
				distance();
				outputDist_.error_pos = error_to_t.error_pos;
				outputDist_.error_ang = error_to_t.error_ang;
				outputDist_.error_alt = error_to_t.error_alt;
				outputDist_.command = 21; // LAND
				pubToDistance_.publish(outputDist_);
			}
			break;
		
		case MANUAL_FLIGHT:
			if (new_state == true)
				{
					ROS_INFO("REF: MANUAL_FLIGHT");
					new_state = false;
				}
				if (new_frame == true)
				{
					new_frame = false;
				}
			break;
		}
}

void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("REF: RUNNING");

		Reference_Handle();
		ros::spinOnce();

		loop_rate.sleep();
	}
}

protected:
/*state here*/
ros::NodeHandle n_;

ros::Subscriber subFromPosition_;
ros::Subscriber subFromCmd_;
ros::Subscriber subFromMmsStatus_;
ros::Subscriber subFromSonar_;
ros::Subscriber subFromFrame_;
ros::Subscriber subFromGlobPosInt_;
ros::Subscriber subLeashingTargetPosition_;
ros::Subscriber subLeashingCommand_;

ros::Publisher pubToReference_;
ros::Publisher pubGridAck_;
ros::Publisher pubToDistance_;
ros::Publisher pubHome_;
ros::Publisher pubGridInfo_;
ros::Publisher pubLeashingStatus_;

guidance_node_amsl::Position_nav inputPos_;
mavros::Global_position_int inputGlobPosInt_;

mavros::Sonar inputSonar_;
mms_msgs::Cmd inputCmd_;
mms_msgs::MMS_status inputMmsStatus_;
frame::Ref_system inputFrame_;
frame::Ref_system oldFrame_;

guidance_node_amsl::Reference outputRef_;
guidance_node_amsl::Reference target_;     

reference::Distance outputDist_;

guidance_node_amsl::Reference tempRef_;
guidance_node_amsl::Reference waypointRef_;
mavros::Global_position_int Home_;

reference::LeashingCommand leashing_command_;   //leashing
reference::LeashingStatus leashing_status_;    //leashing
geographic_msgs::GeoPose leashing_target_;	 //leashing

// STATES DEFINITION
static const int ON_GROUND_NO_HOME = 10;         //TODO make a .h to include in both reference and mms
static const int SETTING_HOME = 20;
static const int  ON_GROUND_DISARMED = 30;
static const int  ARMING = 40;
static const int  DISARMING = 45;
static const int  ON_GROUND_ARMED = 50;
//static const int  ON_GROUND_READY_TO_TAKEOFF = 60;
static const int  PERFORMING_TAKEOFF = 70;
static const int  IN_FLIGHT = 80;
static const int  GRID = 90;
//static const int  READY_TO_GO = 90;
static const int  PERFORMING_GO_TO = 100;
//static const int  READY_TO_LAND = 110;
static const int  PERFORMING_LANDING = 120;
static const int LEASHING = 140;
static const int  MANUAL_FLIGHT = 1000;

static const int  MAX_VERTEX_GRID = 10;

// STATE INITIALIZATION
int currentState;
int oldState;
// int lastARMState = ON_GROUND_DISARMED;
int target_frame;
int actual_frame;
int tempRelAlt;

int rate;
bool new_state;
bool new_frame;
bool land;
uint16_t counter_print;

//GRID RELATED
float d_grid;
bool received_grid_cmd;
bool waiting_for_vertex_grid;
bool repeat_flag;
int vertex_grid_n; 
int received_vertexes_grid;
float speed_grid;
float height_grid;
float **vertex_grid;
float initial_pos_grid [2];
bool success_grid;   //OUTPUT
float **WP;        //OUTPUT
int N_WP;      //OUTPUT
int WP_completed_grid;
bool waiting_for_WP_execution_grid;
mms_msgs::Grid_ack grid_ack_;

//LEASHING RELATED
leashing_target_ned leashing_target_ned_;
leashing_offset_ned leashing_offset_ned_;
double yaw_leashing;


private:

int Dh_TO;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "reference");
	ros::NodeHandle node;

	ReferenceNodeClass referenceNode(node);

	referenceNode.run();
	return 0;
}
