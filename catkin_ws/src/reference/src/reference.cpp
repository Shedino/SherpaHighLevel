#include "ros/ros.h"

#include "mavros/Global_position_int.h"
#include "guidance_node_amsl/Reference.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mms_msgs/Cmd.h"
#include "mms_msgs/MMS_status.h"
#include <mavros/Sonar.h>
#include <frame/Ref_system.h>
#include "reference/Distance.h"


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

class ReferenceNodeClass {
public:
	ReferenceNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromPosition_=n_.subscribe("/position_nav", 10, &ReferenceNodeClass::readPositionMessage,this);
		subFromGlobPosInt_=n_.subscribe("/global_position_int", 10, &ReferenceNodeClass::readGlobalPosIntMessage,this);
		subFromCmd_=n_.subscribe("/command", 10, &ReferenceNodeClass::readCmdMessage,this);
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
		End_Point.Z=(End_Point.Ne*(1.0f-0.08181919*0.08181919)+alt)*sin(outputRef_.Latitude*1e-7f*PI/180.0f);
        //ROS_INFO("ne, %f, endp_x, %f, endp_y, %f ,endp_z, %f", End_Point.Ne,End_Point.X, End_Point.Y,End_Point.Z);

        alt = (double)inputPos_.Altitude/1000.0f;//AltitudeAMSL*1e-3;// AltitudeRelative + Home.AltitudeAMSL;
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
		error_to_t.error_pos = 1000.0f*sqrt(error_x*error_x + error_y*error_y);
		error_to_t.error_ang = 180.0f/PI*sqrt(error_yaw*error_yaw);
		error_to_t.error_alt = 1000.0f*sqrt(error_z*error_z);
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
			ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %d - %d",target_.Latitude,target_.Longitude,target_.AltitudeRelative,target_.Yawangle,target_.frame);
			// target_.Mode = 0;
		} break;
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

		case ON_GROUND_READY_TO_TAKEOFF:
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
			break;

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
					get_current_position();
					outputRef_.frame = actual_frame;
					tempRef_ = outputRef_;
					tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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

		case READY_TO_GO:
			if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
			{
				if (new_state == true)
				{
					ROS_INFO("REF: READY_TO_GO");
					new_state = false;
					get_current_position();
					outputRef_.frame = actual_frame;
					tempRef_ = outputRef_;
					tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
					ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %d - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
					land = false;
					ROS_INFO("REF->NAV: WAYPOINT BARO");
				}
				if (actual_frame == 11 && target_frame == 11) // 11 = sonar
				{
					outputRef_ = tempRef_;
					outputRef_.frame = actual_frame;
					// tempRef_.frame = actual_frame;
					pubToReference_.publish(outputRef_);// as it is
					ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %d - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
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
					ROS_INFO("REF: MAV_CMD_DO_NAV_WAYPOINT. Params: %d - %d - %d - %d - %d",outputRef_.Latitude,outputRef_.Longitude,outputRef_.AltitudeRelative,outputRef_.Yawangle,outputRef_.frame);
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

case READY_TO_LAND:
	if (inputPos_.frame == actual_frame && inputMmsStatus_.target_ref_frame == inputFrame_.target_ref_frame) // CHOERENCE CHECK
	{
		if (new_state == true)
		{
			ROS_INFO("REF: READY_TO_LAND");
			new_state = false;
			get_current_position();
			outputRef_.frame = actual_frame;
			tempRef_ = outputRef_;
			tempRelAlt = inputGlobPosInt_.relative_alt-inputHome_.relative_alt;
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
	break;

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
		outputRef_.AltitudeRelative -= 80; // 5 cm @ frequencey
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


// STATE INITIALIZATION
int currentState = ON_GROUND_NO_HOME;
int oldState = 0;//ON_GROUND_NO_HOME;
// int lastARMState = ON_GROUND_DISARMED;
int target_frame = 6;
int actual_frame = 6;
int tempRelAlt = 0;

// ERRORS DEFINITION
// float err; // current distance to the target position in millimeters
/*int eps_LAND = 100; // target distance to the LAND position in millimeter
	int eps_WP = 100; // target distance to the WAYPOINT position in millimeters
	int eps_TO = 100; // target distance to the TAKEOFF position in millimeters*/

int rate = 10;
bool new_state = true;
// bool new_pos = false;
bool new_frame = true;
bool land = false;

uint16_t counter_print = 0;

private:

int Dh_TO = 5000;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "reference");
	ros::NodeHandle node;

	ReferenceNodeClass referenceNode(node);

	referenceNode.run();
	return 0;
}
