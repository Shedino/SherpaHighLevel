//#include <mavros/RCIn.h>
#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include "mavros/OverrideRCIn.h"
#include "mavros/Sonar.h"
#include "mavros/Attitude.h"
#include "mavros/ArtvaRead.h"
#include "mavros/Global_position_int.h"
#include "mavros/Safety.h"

#include <guidance_node_amsl/Directive.h>
//#include <guidance_node_amsl/Position.h>
#include <mms/Ack_arm.h>
#include <mms/Arm.h>
#include <mms/Sys_status.h>

#define SONAR_THRESHOLD 300          //maximum centimetres of a reliable sonar reading
namespace mavplugin {
/**
 * @brief Mavros plugin
 */
class UniboControllerAMSLPlugin : public MavRosPlugin {
public:
	UniboControllerAMSLPlugin() :
		nodeHandle(),
		uas(nullptr),
		safetyOn(true),
		v_xy_max(3.0),
		v_z_max(1.5),
		v_psi_max(3.14),
		RC1_trim_(1),
		RC2_trim_(1),
		RC3_trim_(1),
		RC4_trim_(1)
	{};

	void initialize(UAS &uas_)
	{
		uas = &uas_;

		/* --- SUBSCRIPTIONS --- */
		directive_sub = nodeHandle.subscribe("/directive", 10, &UniboControllerAMSLPlugin::directive_cb, this);
		arm_sub = nodeHandle.subscribe("/arm", 10, &UniboControllerAMSLPlugin::arming, this);

		/* --- PUBLISHERS --- */
		position_pub = nodeHandle.advertise<mavros::Global_position_int>("/global_position_int", 10);               //TODO this should become a mavros topic and removed from amsl and splitted from attitude
		arm_ack_pub = nodeHandle.advertise<mms::Ack_arm>("acknowledge_arming", 10);
		sys_status_pub = nodeHandle.advertise<mms::Sys_status>("/system_status", 10);
		distance_sensor_pub = nodeHandle.advertise<mavros::Sonar>("/sonar", 10);
		attitude_pub = nodeHandle.advertise<mavros::Attitude>("/attitude", 10);
		ROS_INFO("reading ARTVA message from Arduino!");
		artva_pub = nodeHandle.advertise<mavros::ArtvaRead>("/artva_read", 10);
		safety_pub = nodeHandle.advertise<mavros::Safety>("/safety_odroid", 10);



		nodeHandle.param("guidance_node_amsl/param/sat_xy", v_xy_max, 3.0);
		nodeHandle.param("guidance_node_amsl/param/sat_z", v_z_max, 1.5);
		nodeHandle.param("guidance_node_amsl/param/sat_yaw", v_psi_max, 3.14);
		
		/*nodeHandle.param("mavros/unibo_controller/trim_RC1", RC1_trim_, 1000.0);
		nodeHandle.param("mavros/unibo_controller/trim_RC2", RC2_trim_, 1000.0);
		nodeHandle.param("mavros/unibo_controller/trim_RC3", RC3_trim_, 1000.0);
		nodeHandle.param("mavros/unibo_controller/trim_RC4", RC4_trim_, 1000.0);*/

		mavros::Sonar temp_sonar;
		temp_sonar.distance = -1;      //if there is no sonar, the distance is initialized to -1
		distance_sensor_pub.publish(temp_sonar);
	}

	//should be logic mapping between id number and message type
	const message_map get_rx_handlers() {
		return {
			MESSAGE_HANDLER(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, &UniboControllerAMSLPlugin::handle_global_position_int),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_ATTITUDE, &UniboControllerAMSLPlugin::handle_attitude),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_RC_CHANNELS_RAW, &UniboControllerAMSLPlugin::handle_rc_channels_raw),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_COMMAND_ACK, &UniboControllerAMSLPlugin::handle_arm_ack),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_HEARTBEAT, &UniboControllerAMSLPlugin::handle_heartbeat),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_SYS_STATUS, &UniboControllerAMSLPlugin::handle_status),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_DISTANCE_SENSOR, &UniboControllerAMSLPlugin::handle_distance_sensor),
//			MESSAGE_HANDLER(MAVLINK_MSG_ID_VFR_HUD, &UniboControllerAMSLPlugin::handle_vfr_hud),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_DEBUG_VECT, &UniboControllerAMSLPlugin::handle_ARTVA_read)
		};
	}

private:
	ros::NodeHandle nodeHandle;
	UAS *uas;
	double RC1_trim_;
	double RC2_trim_;
	double RC3_trim_;
	double RC4_trim_;

	ros::Publisher position_pub;
	ros::Subscriber directive_sub;
	ros::Subscriber arm_sub;
	ros::Publisher arm_ack_pub;
	ros::Publisher sys_status_pub;
	ros::Publisher distance_sensor_pub;
	ros::Publisher attitude_pub;
	ros::Publisher safety_pub;
	ros::Publisher artva_pub;
	
	mms::Sys_status _system_status;

	mavros::ArtvaRead ARTVAread_msg;
	mavros::Attitude attitude_msg;  //private and accessed by many handlers
	mavros::Safety safety_;
	mavros::Global_position_int global_pos_;

	//message to move the quadcopter
	mavros::OverrideRCIn velocity_;

	//safety flag
	bool safetyOn;

	//saturation parameters
	/*
	 * Questi tipi di dato sono matlab per specifiche ros del param server.
	 * Nel nodo "guidance_node" sono convertiti a float perchè la legge di guida matlab ha come input
	 * dei float. Adesso non sono convertiti perchè tanto possono essere usati così
	 */
	double v_xy_max, v_z_max, v_psi_max;

	// void test_ARTVA_mavlink_message(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {
		
		// ROS_INFO("Mavlink message busy! :-(");
	// }

	void handle_ARTVA_read(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		//ROS_INFO("ARTVA message received! :-)");
		mavlink_debug_vect_t ARTVAread;
		mavlink_msg_debug_vect_decode(msg, &ARTVAread);

		if(ARTVAread.name[0]=='a'){
			//ROS_INFO("GREAT");
			ARTVAread_msg.rec1_distance = (int)ARTVAread.time_usec;
			ARTVAread_msg.rec1_direction =(int)ARTVAread.x;
			if(ARTVAread_msg.rec1_distance>0){
				ROS_INFO("ARTVA 1 read: distance %dm, heading %d DEG",ARTVAread_msg.rec1_distance,ARTVAread_msg.rec1_direction);
			}
			ARTVAread_msg.rec2_distance = (int)ARTVAread.y;
			ARTVAread_msg.rec2_direction = (int)ARTVAread.z;
			if(ARTVAread_msg.rec2_distance>0){
				ROS_INFO("ARTVA 2 read: distance %dm, heading %d DEG",ARTVAread_msg.rec2_distance,ARTVAread_msg.rec2_direction);
			}
		}else if(ARTVAread.name[0]=='b'){
			ARTVAread_msg.rec3_distance = (int)ARTVAread.time_usec;
			ARTVAread_msg.rec3_direction = (int)ARTVAread.x;
			if(ARTVAread_msg.rec3_distance>0){
				ROS_INFO("ARTVA 3 read: distance %dm, heading %d DEG",ARTVAread_msg.rec3_distance,ARTVAread_msg.rec3_direction);
			}
			ARTVAread_msg.rec4_distance = (int)ARTVAread.y;
			ARTVAread_msg.rec4_direction = (int)ARTVAread.z;
			if(ARTVAread_msg.rec4_distance>0){
				ROS_INFO("ARTVA 4 read: distance %dm, heading %d DEG",ARTVAread_msg.rec4_distance,ARTVAread_msg.rec4_direction);
			}
		}
		artva_pub.publish(ARTVAread_msg);
	}

	//check if safety is on
	void handle_rc_channels_raw(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_rc_channels_raw_t channels_raw;
		mavlink_msg_rc_channels_raw_decode(msg,&channels_raw);

		/*
		 * If safety is on, chan5_raw must be less than 1200
		 */
		//DEBUG
		//ROS_INFO("CHANNEL 6 IS: %u",channels_raw.chan6_raw);
		if(channels_raw.chan6_raw > 1700 && channels_raw.chan5_raw > 1300 && channels_raw.chan5_raw < 1700){
		//            ODROID_ON                 HIGHER THAN STABILIZE     &&            LOWER THAN RTL        -->      LOITER
			safetyOn = false;
			safety_.safety = false;
		}
		else {
			safetyOn=true;
			safety_.safety = true;
		}
		safety_pub.publish(safety_);

		//DEBUG
//		ROS_INFO("HANDLE RC RAW");

	}

	void handle_attitude(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_attitude_t attitude;
		mavlink_msg_attitude_decode(msg, &attitude);

		attitude_msg.roll = attitude.roll;
		attitude_msg.pitch = attitude.pitch;
		attitude_msg.yaw = attitude.yaw;
		attitude_msg.rollspeed = attitude.rollspeed;
		attitude_msg.pitchspeed = attitude.pitchspeed;
		attitude_msg.yawspeed = attitude.yawspeed;
		attitude_msg.time_boot_ms = attitude.time_boot_ms;
		
		if (attitude.yaw >= 0){
			global_pos_.hdg = (int)((attitude.yaw)*180/3.14*100);   //attitude comes in +-pi but hdg is 0..359.99 deg.
		} else {
			global_pos_.hdg = (int)((attitude.yaw+6.28)*180/3.14*100);   //attitude comes in +-pi but hdg is 0..359.99 deg. Adding 2*pi to attitude if negative.
		}
		

		//position_pub.publish(position_msg);       //already published when received position_int
		attitude_pub.publish(attitude_msg);
	}

	void handle_heartbeat(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_heartbeat_t heart;
		mavlink_msg_heartbeat_decode(msg, &heart);

		//auto position_msg = boost::make_shared<guidance_node_amsl::Position>();
		ROS_INFO("Heatbeat. Base Mode: %d - Sys Status: %d ",heart.base_mode, heart.system_status);
		if (heart.base_mode >= 128)	_system_status.armed = true;
		else	_system_status.armed = false;
		sys_status_pub.publish(_system_status);
	}

	void handle_status(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {
		mavlink_sys_status_t status;
		mavlink_msg_sys_status_decode(msg, &status);
		//ROS_INFO("Received sys status mavlink. Battery: %d", status.voltage_battery);

		_system_status.voltage_battery = status.voltage_battery;
		sys_status_pub.publish(_system_status);
	}

	void handle_arm_ack(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_command_ack_t cmd_ack;
		mavlink_msg_command_ack_decode(msg, &cmd_ack);

		auto arm_ack_msg = boost::make_shared<mms::Ack_arm>();

		if (cmd_ack.command == 400){      //ARM_DISARM command
			//ROS_INFO("Received arm_disarm");
			if (cmd_ack.result == MAV_RESULT_ACCEPTED){
				arm_ack_msg->mav_result = true;
				//ROS_INFO("Arm-disarm: succsessful");
			} else {
				arm_ack_msg->mav_result = false;
				//ROS_INFO("Arm-disarm: fail");
			}
			arm_ack_pub.publish(arm_ack_msg);
		}
	}

	void handle_global_position_int(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_global_position_int_t global_position;
		mavlink_msg_global_position_int_decode(msg, &global_position);

		//auto position_msg = boost::make_shared<guidance_node_amsl::Position>();

		//getting new data
		global_pos_.time_boot_ms = global_position.time_boot_ms;
		global_pos_.relative_alt = global_position.relative_alt;
		global_pos_.alt = global_position.alt;
		global_pos_.lat = global_position.lat;
		global_pos_.lon = global_position.lon;
		global_pos_.vx = 0;            //NOT USED NOW
		global_pos_.vy = 0;            //NOT USED NOW
		global_pos_.vz = 0;            //NOT USED NOW

		//DEBUG
//		ROS_INFO("HANDLE POSITION");


		position_pub.publish(global_pos_);
	}

	void handle_distance_sensor(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_distance_sensor_t distance_sensor;
		mavlink_msg_distance_sensor_decode(msg, &distance_sensor);

		auto distance_msg = boost::make_shared<mavros::Sonar>();
		if (distance_sensor.current_distance > SONAR_THRESHOLD){    //more than 300cm-->not reliable
			distance_msg->distance = 0;         //if the distance is over threshold-->put zero
		} else {
			distance_msg->distance = (int)(distance_sensor.current_distance*10*cos(attitude_msg.roll)*cos(attitude_msg.pitch));   //from mavlink received in centimiters, converted in millimiters and corrected with attitude
		}
		//ROS_INFO("Sonar received. Distance: %d - Type: %d - MaxDist: %d",distance_msg->distance, distance_sensor.type, distance_sensor.max_distance);
		distance_sensor_pub.publish(distance_msg);
	}
	/*
	 * From directive to RC
	 */
	void directive_cb(const guidance_node_amsl::Directive::ConstPtr msg){

		/*
		 * Initializing values, if safety is on, these values will remain
		 */
			velocity_.channels[0]=0;
			velocity_.channels[1]=0;
			velocity_.channels[2]=0;
			velocity_.channels[3]=0;
			velocity_.channels[4]=0;
			velocity_.channels[5]=0;
			velocity_.channels[6]=0;
			velocity_.channels[7]=0;
		
		

		if(!safetyOn){
			
		    if (nodeHandle.getParam("mavros/unibo_controller/trim_RC1", RC1_trim_)){
			//ROS_INFO("RC1_trim: %.3f",RC1_trim_);
			}
			if (nodeHandle.getParam("mavros/unibo_controller/trim_RC2", RC2_trim_)){
				//ROS_INFO("RC2_trim: %.3f",RC2_trim_);
			}
			if (nodeHandle.getParam("mavros/unibo_controller/trim_RC3", RC3_trim_)){
				//ROS_INFO("RC3_trim: %.3f",RC3_trim_);
			}
			if (nodeHandle.getParam("mavros/unibo_controller/trim_RC4", RC4_trim_)){
				//ROS_INFO("RC4_trim: %.3f",RC4_trim_);
			}
			/*
			 * If safety is off, I translate velocity in RC values
			 */
			uint16_t vx_RC= (uint16_t)400.0f*(-msg->vxBody)/v_xy_max + RC1_trim_;         //New: 400 + 1520; Old:  500 + 1500
			uint16_t vy_RC=(uint16_t)400.0f*(msg->vyBody)/v_xy_max + RC2_trim_;		//New: 400 + 1520; Old:  500 + 1500
			/*
			 * it seems it loiters with 1420 instead of 1500...
			 */
			uint16_t vz_RC= RC3_trim_;
			if (msg->vzBody > 0){ //going down, mapped only in 420us
				vz_RC = vz_RC + (uint16_t)320.0f*(-msg->vzBody)/v_z_max;        //New: 320; Old:  420
			} else {        //going up, mapped in 580us
				vz_RC = vz_RC + (uint16_t)480.0f*(-msg->vzBody)/v_z_max;     //New: 480 Old: 580
			}
			uint16_t v_psi_RC = (uint16_t)400.0f*(msg->yawRate)/v_psi_max + RC4_trim_;		//New: 400 + 1520; Old:  500 + 1500

			velocity_.channels[0]=vy_RC;
			velocity_.channels[1]=vx_RC;
			velocity_.channels[2]=vz_RC;
			velocity_.channels[3]=v_psi_RC;


		}

		//DEBUG
		/*ROS_INFO("RC_OVERRIDE: [CH1:%u, CH2:%u, CH3:%u, CH4:%u, CH5:%u, CH6:%u, CH7:%u, CH8:%u]",
				velocity_.channels[0], velocity_.channels[1],
				velocity_.channels[2], velocity_.channels[3],
				velocity_.channels[4], velocity_.channels[5],
				velocity_.channels[6], velocity_.channels[7]);*/

		mavlink_message_t msg_mav;
		mavlink_msg_rc_channels_raw_pack_chan(UAS_PACK_CHAN(uas),&msg_mav,0,1,velocity_.channels[0],velocity_.channels[1],velocity_.channels[2],velocity_.channels[3],0,0,0,0,100);      //1 is the sequence that we are not considering right now
		UAS_FCU(uas)->send_message(&msg_mav);
	}

	/*
	 * Arming/disarming the UAV
	 */
	void arming(const mms::Arm::ConstPtr msg){
		mavlink_message_t msg_mav;
		if (msg->arm_disarm){
			enum MAV_CMD command = MAV_CMD_COMPONENT_ARM_DISARM;
			float param1 = 1;      //1-->arm 0-->disarm
			float param2 = 0;      //not used
			float param3 = 0;      //not used
			float param4 = 0;      //not used
			float param5 = 0;      //not used
			float param6 = 0;      //not used
			float param7 = 0;      //not used
			uint8_t confirmation = 1;
			mavlink_msg_command_long_pack_chan(UAS_PACK_CHAN(uas), &msg_mav,
								uas->get_tgt_system(),
								uas->get_tgt_component(),
								command,
								confirmation,
								param1, param2,
								param3, param4,
								param5, param6,
								param7);
			UAS_FCU(uas)->send_message(&msg_mav);
			//ROS_INFO("Arming UAV");
		} else {
			enum MAV_CMD command = MAV_CMD_COMPONENT_ARM_DISARM;
			float param1 = 0;      //1-->arm 0-->disarm
			float param2 = 0;      //not used
			float param3 = 0;      //not used
			float param4 = 0;      //not used
			float param5 = 0;      //not used
			float param6 = 0;      //not used
			float param7 = 0;      //not used
			uint8_t confirmation = 1;
			mavlink_msg_command_long_pack_chan(UAS_PACK_CHAN(uas), &msg_mav,
								uas->get_tgt_system(),
								uas->get_tgt_component(),
								command,
								confirmation,
								param1, param2,
								param3, param4,
								param5, param6,
								param7);
			UAS_FCU(uas)->send_message(&msg_mav);                        //TODO decide if send or not disarm by software
			//ROS_INFO("Disarming UAV");
		}
	}
};
};	// namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::UniboControllerAMSLPlugin, mavplugin::MavRosPlugin)
