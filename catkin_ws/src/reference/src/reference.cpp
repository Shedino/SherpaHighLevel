#include "ros/ros.h"

#include "mavros/Global_position_int.h"
#include "guidance_node_amsl/Reference.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mms_msgs/Cmd.h"
#include "mms_msgs/MMS_status.h"
#include <mavros/Sonar.h>
#include <frame/Ref_system.h>
#include "reference/Distance.h"
#include "WP_grid.h"         //GRID



double PI = 3.1416; // pi

class ReferenceNodeClass {
public:
	ReferenceNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromPosition_=n_.subscribe("/position_nav", 10, &ReferenceNodeClass::readPositionMessage,this);
		subFromGlobPosInt_=n_.subscribe("/global_position_int", 10, &ReferenceNodeClass::readGlobalPosIntMessage,this);
		subFromCmd_=n_.subscribe("/sent_command", 10, &ReferenceNodeClass::readCmdMessage,this);     //excluding command verifier
        subFromSonar_ = n_.subscribe("/sonar", 10, &ReferenceNodeClass::readSonarMessage,this);
		subFromMmsStatus_=n_.subscribe("/mms_status", 10, &ReferenceNodeClass::readMmsStatusMessage,this);
		subFromFrame_=n_.subscribe("/ref_system", 10, &ReferenceNodeClass::readFrameMessage,this);
		subFromHome_=n_.subscribe("/home", 10, &ReferenceNodeClass::readHomeMessage,this);
		
		// publishers
		pubToReference_=n_.advertise<guidance_node_amsl::Reference>("/reference",10);
		pubToDistance_=n_.advertise<reference::Distance>("/distance",10);

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

		inputHome_.lat = 0;
		inputHome_.lon = 0;
		inputHome_.relative_alt = 0;
		inputHome_.alt = 0;
		inputHome_.hdg = 0;
		inputHome_.time_boot_ms = 0;
		
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
		//max_wp_grid = 150;
		//WP_data_grid = new real_T[max_wp_grid*2];
		//success_grid = 0;
		//WP_out_grid = emxCreate_real_T(max_wp_grid, 2);
		//WP_out_grid = new real_T [max_wp_grid*2];
		received_grid_cmd = false;
		waiting_for_vertex_grid = false;
		vertex_grid_n = 0;
		received_vertexes_grid = 0;
		speed_grid = 0;
		height_grid = 0;
		//int_point [0] = 0;
		//int_point [1] = 0;
		//point1 [0] = 0;
		//point1 [1] = 0;
		//point2 [0] = 4;
		//point2 [1] = 4;
		//inters = 0;
		N_vertex = 8;
		points = new float *[N_vertex];           
		for(int i = 0; i <N_vertex; i++){
	    	points[i] = new float[2];
		}
		/*for(int i = 0; i <5; i++){
			convex_vertex[i] = new float[2];
		}*/
		points [0][0] = 5;
		points [0][1] = 0;
		points [1][0] = 3;
		points [1][1] = 4;
		points [2][0] = 2;
		points [2][1] = 2;
		points [3][0] = 1;
		points [3][1] = 5;
		points [4][0] = -3;
		points [4][1] = 3;
		points [5][0] = -1;
		points [5][1] = 2;
		points [6][0] = 0;
		points [6][1] = 2;
		points [7][0] = -2;
		points [7][1] = -2;

		convex = false;
		
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

		alt = (double)outputRef_.AltitudeRelative/1000.0f;// + Home.AltitudeAMSL*1e-3;
		End_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Latitude/10000000.0f*PI/180.0f));
		End_Point.X=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*cos(outputRef_.Longitude/10000000.0f*PI/180.0f);
		End_Point.Y=(End_Point.Ne+alt)*cos(outputRef_.Latitude/10000000.0f*PI/180.0f)*sin(outputRef_.Longitude/10000000.0f*PI/180.0f);
		//End_Point.Z=(End_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(outputRef_.Latitude*1e-7f*PI/180.0f);
		End_Point.Z = (double)outputRef_.AltitudeRelative;     //MICHELE BRUTAL!! buahahaha
        //ROS_INFO("ne, %f, endp_x, %f, endp_y, %f ,endp_z, %f", End_Point.Ne,End_Point.X, End_Point.Y,End_Point.Z);

        	alt = (double)inputPos_.Altitude/1000.0f;//AltitudeAMSL*1e-3;// AltitudeRelative + Home.AltitudeAMSL;
		Starting_Point.Ne=6378137.0f;///sqrt(1.0f-0.08181919f*0.08181919f*sin(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f));
		Starting_Point.X=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*cos(inputPos_.Longitude/10000000.0f*PI/180);
		Starting_Point.Y=(Starting_Point.Ne+alt)*cos(inputPos_.Latitude/10000000.0f*PI/180.0f)*sin(inputPos_.Longitude/10000000.0f*PI/180);
		//Starting_Point.Z=(Starting_Point.Ne*(1.0f-0.08181919f*0.08181919f)+alt)*sin(inputPos_.Latitude/10000000.0f*PI/180.0f);
		Starting_Point.Z = (double)inputPos_.Altitude;		//MIC brutal! buahaha
		//ROS_INFO("ne, %f, sp_x, %f, sp_y, %f ,sp_z, %f, alt %d", Starting_Point.Ne,Starting_Point.X, Starting_Point.Y,Starting_Point.Z,inputPos_.Altitude);

		error_x = End_Point.X - Starting_Point.X; //outputRef_.Latitude - inputPos_.Latitude;
		error_y = End_Point.Y - Starting_Point.Y; //outputRef_.Longitude - inputPos_.Longitude;
		error_z = End_Point.Z - Starting_Point.Z; //outputRef_.AltitudeRelative - (inputPos_.Altitude-Home.AltitudeAMSL);
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

	void readHomeMessage(const mavros::Global_position_int::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: POSITION_RECEIVED");
		inputHome_.lat = msg->lat;
		inputHome_.lon = msg->lon;
		inputHome_.relative_alt = msg->relative_alt;
		inputHome_.alt = msg->alt;
		inputHome_.hdg = msg->hdg;
		inputHome_.time_boot_ms = msg->time_boot_ms;
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

		//6 = MAV_FRAME_GLOBAL_RELATIVE_ALT_INT
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
				//ROS_INFO("Seqence 1: %d", outputDist_.seq);	//TODO remove
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
						tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
				
				/*ROS_INFO("REF: SETTING_HOME");

				Home.lat = inputGlobPosInt_.lat;
				Home.lon = inputGlobPosInt_.lon;
				Home.alt = inputGlobPosInt_.alt;
				Home.relative_alt = inputGlobPosInt_.relative_alt;
				Home.hdg = inputGlobPosInt_.hdg;
				ROS_INFO("HOME POSITION: Lat, %d, Lon, %d, AltRel, %d, Yaw, %d",Home.lat, Home.lon, Home.relative_alt, Home.hdg);*/
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
						tempRelAlt = inputGlobPosInt_.relative_alt - inputHome_.relative_alt;
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
									tempRelAlt = inputGlobPosInt_.relative_alt;// - Home.relative_alt;
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
									tempRelAlt = inputGlobPosInt_.relative_alt;// - Home.relative_alt;
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
									tempRelAlt = inputGlobPosInt_.relative_alt - inputHome_.relative_alt;
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
									tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
					tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
					ROS_INFO("REF->NAV: REFERENCE = TAKEOFF");
				}
				if (new_frame == true)
				{
					new_frame = false;

					if (actual_frame == 6 && target_frame == 6) // 6 = barometer
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRef_.AltitudeRelative+Dh_TO+1;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: TAKEOFF BARO");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 11 && target_frame == 11) // 11 = sonar
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = Dh_TO+2;
						outputRef_.frame = actual_frame;
						ROS_INFO("REF->NAV: TAKEOFF SONAR");
						pubToReference_.publish(outputRef_);
					}
					if (actual_frame == 6 && target_frame == 11)
					{
						outputRef_ = tempRef_;
						outputRef_.AltitudeRelative = tempRelAlt+Dh_TO+3;
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
					tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;*/
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
					//tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
				//-------TEST GRID-------   TODO change position
				if (received_grid_cmd && !waiting_for_vertex_grid){
					/*real_T data_arr[8] = {0, 6, 6, 0, 0, 0, 4, 4};  //COLOUMN MAJOR!!!-->(0,0)-(6,0)-(6,4)-(0,4)
					real_T *data = data_arr;
					emxArray_real_T* vertex_test = emxCreateWrapper_real_T(data, 4, 2);
					initial_pos_grid[0] = 0;
					initial_pos_grid[1] = 0;
					d_grid = 1;
					//WP_grid(vertex_test, initial_pos_grid, d_grid, max_wp_grid, WP_out_grid, &success_grid, &number_WP_grid);
					WP_grid(vertex_test, initial_pos_grid, d_grid, WP_out_grid, &success_grid, &number_WP_grid);
					ROS_INFO("GRID! Success: %d - N. WP: %d", success_grid, number_WP_grid);
					for (int i = 0; i < number_WP_grid; i++){
						//ROS_INFO("GRID! WP %d: %.2f - %.2f", i, WP_out_grid->data[i], WP_out_grid->data[max_wp_grid+i]);    //with emxArray
						ROS_INFO("GRID! WP %d: %.2f - %.2f", i, WP_out_grid[i], WP_out_grid[max_wp_grid+i]);	//with array
					}*/
					//intersection_line_segment(point1, point2, 0, 0, &inters, int_point);	
					//ROS_INFO("GRID! Intersection: %d - Point x: %f - Point y: %f", inters, int_point[0], int_point[1]);
					//find_closest_2D(points, point1, closest_point, &index, int(5));
					//ROS_INFO("GRID! Closest point: %f - %f - INDEX: %d", closest_point[0], closest_point[1], index);
					is_convex(points, true, &convex, &N_vertex);
					ROS_INFO("GRID! Convex: %s - N_vertex: %d", convex ? "true" : "false", N_vertex);
					for (int i = 0 ; i<N_vertex; i++){
						ROS_INFO("GRID! Vertex %d: %f - %f", i+1, points[i][0], points[i][1]);
					}
				}
				//-------TEST GRID------------------------------------
				/*if (received_grid_cmd && !waiting_for_vertex_grid){   //have received all vertexes
					ROS_INFO("REF: GRID. Starting GRID alg. N. vertex: %d - Distance: %f", vertex_grid_n, d_grid);
					real_T *data_arr = new real_T[vertex_grid_n*2]; //COLOUMN MAJOR!!!
					for (int i = 0; i < vertex_grid_n; i++){
						ROS_INFO("REF: GRID. Vertex %d: %f - %f", i+1, vertex_grid [i][0], vertex_grid [i][1]);
						data_arr [i] = vertex_grid [i][0];			//COLOUMN MAJOR!!		//TODO add conversion WGS84-->NED
						data_arr [i+vertex_grid_n] = vertex_grid [i][1];					//TODO add conversion WGS84-->NED
					}
					ROS_INFO("REF: GRID. Vertex: %f %f %f %f %f %f %f %f", data_arr [0], data_arr [1], data_arr [2], data_arr [3], data_arr [4], data_arr [5], data_arr [6], data_arr [7]);
					emxArray_real_T* vertex = emxCreateWrapper_real_T(data_arr, vertex_grid_n, 2);
					initial_pos_grid[0] = outputRef_.Latitude;    //latest reference as initial point    //TODO add conversion WGS84-->NED
					initial_pos_grid[1] = outputRef_.Longitude;	//latest reference as initial point		 //TODO add conversion WGS84-->NED
					WP_grid(vertex, initial_pos_grid, d_grid, max_wp_grid, WP_out_grid, &success_grid, &number_WP_grid);
					ROS_INFO("REF: GRID! Success: %d - N. WP: %d - speed: %f - Height: %f", success_grid, number_WP_grid, speed_grid, height_grid);
					for (int i = 0; i < number_WP_grid; i++){
						ROS_INFO("REF: GRID! WP %d: %.2f - %.2f", i, WP_out_grid->data[i], WP_out_grid->data[max_wp_grid+i]);
					}
				}*/
				received_grid_cmd = false;     //WP calculated. Now they need to be sent as reference
				//TODO send WP as reference
				//TODO add conversion NED-->WGS84
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
					// tempRelAlt = inputGlobPosInt_.relative_alt;// - Home.relative_alt;
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
						outputRef_.AltitudeRelative = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
// this state is useless if MISSION_START is not implemented
/*case READY_TO_LAND:
	if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
	{
		if (new_state == true)
		{
			ROS_INFO("REF: READY_TO_LAND");
			new_state = false;
			// comment these lines to avoid the drone accumulates position and yaw errors
			// comment these lines to make the published reference equal to the last valid one
			//get_current_position();
			//outputRef_.frame = actual_frame;
			//tempRef_ = outputRef_;
			//tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
			ROS_INFO("REF->NAV: REFERENCE = READY_TO_LAND");
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

		case PERFORMING_LANDING:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
						ROS_INFO("REF: PERFORMING_LANDING");
						new_state = false;
						get_current_position();
						outputRef_.frame = actual_frame;
						tempRef_ = outputRef_;
						tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
//ros::Subscriber subFromAckArm_;
ros::Subscriber subFromMmsStatus_;
ros::Subscriber subFromSonar_;

ros::Subscriber subFromHome_;
mavros::Global_position_int inputHome_;

ros::Subscriber subFromFrame_;

guidance_node_amsl::Position_nav inputPos_;

ros::Subscriber subFromGlobPosInt_;
mavros::Global_position_int inputGlobPosInt_;

mavros::Sonar inputSonar_;
mms_msgs::Cmd inputCmd_;
//mms_msgs::Ack_arm inputAckArm_;
mms_msgs::MMS_status inputMmsStatus_;
frame::Ref_system inputFrame_;
frame::Ref_system oldFrame_;

//ros::Publisher pubToAckCmd_;
//ros::Publisher pubToArm_;
ros::Publisher pubToReference_;
guidance_node_amsl::Reference outputRef_;     
guidance_node_amsl::Reference target_;    

// ros::Publisher pubToSysStatus_;
ros::Publisher pubToDistance_;
reference::Distance outputDist_;

// guidance_node_amsl::Reference last_valid_ref;
guidance_node_amsl::Reference tempRef_;
guidance_node_amsl::Reference waypointRef_;
// guidance_node_amsl::Reference target_; //TODO HERE
// guidance_node_amsl::Reference LVP_;
// mms::Arm outputArm_;
// mms::Ack_cmd outputAckCmd_;
// mms::Sys_status outputSysStatus_;
// guidance_node_amsl::Reference Target_Position_;
// mavros::Global_position_int Home;



// STATES DEFINITION
static const int ON_GROUND_NO_HOME = 10;
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
//real_T initial_pos_grid[2];
float d_grid;
//emxArray_real_T *WP_out_grid;
//real_T *WP_out_grid;
//int16_T success_grid;
//int16_T max_wp_grid;
//int16_T number_WP_grid;
//float int_point [2];
//int inters;
//float point1 [2];
//float point2 [2];
float **points;
//int index;
bool convex;
float **convex_vertex;
int N_vertex;

bool received_grid_cmd;
bool waiting_for_vertex_grid;
uint16_t vertex_grid_n; 
uint16_t received_vertexes_grid;
float speed_grid;
float height_grid;
float vertex_grid [MAX_VERTEX_GRID][2];

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
