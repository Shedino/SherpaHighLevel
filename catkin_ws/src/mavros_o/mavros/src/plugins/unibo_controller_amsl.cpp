//#include <mavros/RCIn.h>
#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include "mavros/OverrideRCIn.h"

#include <guidance_node_amsl/Directive.h>
#include <guidance_node_amsl/Position.h>
#include <mms/Ack_arm.h>
#include <mms/Arm.h>
#include <mms/Sys_status.h>

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
		v_psi_max(3.14)
	{};

	void initialize(UAS &uas_)
	{
		uas = &uas_;

		directive_sub = nodeHandle.subscribe("/directive", 10, &UniboControllerAMSLPlugin::directive_cb, this);
		position_pub = nodeHandle.advertise<guidance_node_amsl::Position>("/position", 10);
		arm_sub = nodeHandle.subscribe("/arm", 10, &UniboControllerAMSLPlugin::arming, this);
		arm_ack_pub = nodeHandle.advertise<mms::Ack_arm>("acknowledge_arming", 10);
		velocity_publisher_=nodeHandle.advertise<mavros::OverrideRCIn>("mavros/rc/override", 10);
		sys_status_pub = nodeHandle.advertise<mms::Sys_status>("/system_status", 10);

		nodeHandle.param("guidance_node_amsl/param/sat_xy", v_xy_max, 3.0);
		nodeHandle.param("guidance_node_amsl/param/sat_z", v_z_max, 1.5);
		nodeHandle.param("guidance_node_amsl/param/sat_yaw", v_psi_max, 3.14);

		//DEBUG
//		ROS_INFO("INITIALIZE UNIBO PLUGIN");
	}

	//should be logic mapping between id number and message type
	const message_map get_rx_handlers() {
		return {
			MESSAGE_HANDLER(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, &UniboControllerAMSLPlugin::handle_global_position_int),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_ATTITUDE, &UniboControllerAMSLPlugin::handle_attitude),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_RC_CHANNELS_RAW, &UniboControllerAMSLPlugin::handle_rc_channels_raw),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_COMMAND_ACK, &UniboControllerAMSLPlugin::handle_arm_ack),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_HEARTBEAT, &UniboControllerAMSLPlugin::handle_heartbeat),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_SYS_STATUS, &UniboControllerAMSLPlugin::handle_status)
		};
	}

private:
	ros::NodeHandle nodeHandle;
	UAS *uas;

	ros::Publisher position_pub;
	ros::Subscriber directive_sub;
	ros::Publisher velocity_publisher_;
	ros::Subscriber arm_sub;
	ros::Publisher arm_ack_pub;
	ros::Publisher sys_status_pub;
	
	mms::Sys_status _system_status;

	//message to move the quadcopter
	mavros::OverrideRCIn velocity_;

	//common message to join data from handles
	guidance_node_amsl::Position commonMessage;

	//safety flag
	bool safetyOn;

	//saturation parameters
	/*
	 * Questi tipi di dato sono matlab per specifiche ros del param server.
	 * Nel nodo "guidance_node" sono convertiti a float perchè la legge di guida matlab ha come input
	 * dei float. Adesso non sono convertiti perchè tanto possono essere usati così
	 */
	double v_xy_max, v_z_max, v_psi_max;

	//check if safety is on
	void handle_rc_channels_raw(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_rc_channels_raw_t channels_raw;
		mavlink_msg_rc_channels_raw_decode(msg,&channels_raw);

		/*
		 * If safety is on, chan5_raw must be less than 1200
		 */
		//DEBUG
		//ROS_INFO("CHANNEL 6 IS: %u",channels_raw.chan6_raw);
		if(channels_raw.chan6_raw > 1800){
			safetyOn = false;
			commonMessage.Safety = 0;
		}
		else {
			safetyOn=true;
			commonMessage.Safety= 1;
		}

		//DEBUG
//		ROS_INFO("HANDLE RC RAW");

	}

	void handle_attitude(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_attitude_t attitude;
		mavlink_msg_attitude_decode(msg, &attitude);

		auto position_msg = boost::make_shared<guidance_node_amsl::Position>();

		//getting new data
		position_msg->Yawangle=attitude.yaw;

		//copying data from commonMessage
		position_msg->Timestamp=commonMessage.Timestamp;
		position_msg->AltitudeRelative=commonMessage.AltitudeRelative;
		position_msg->AltitudeAMSL=commonMessage.AltitudeAMSL;
		position_msg->Latitude=commonMessage.Latitude;
		position_msg->Longitude=commonMessage.Longitude;
		position_msg->Safety = commonMessage.Safety;

		//saving data
		commonMessage.Yawangle=attitude.yaw;

		//DEBUG
//		ROS_INFO("%f", commonMessage.Yawangle);

		position_pub.publish(position_msg);
	}

	void handle_heartbeat(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_heartbeat_t heart;
		mavlink_msg_heartbeat_decode(msg, &heart);

		//auto position_msg = boost::make_shared<guidance_node_amsl::Position>();
		//ROS_INFO("Heatbeat. Base Mode: %d - Sys Status: %d ",heart.base_mode, heart.system_status);
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
			ROS_INFO("Received arm_disarm");
			if (cmd_ack.result == MAV_RESULT_ACCEPTED){
				arm_ack_msg->mav_result = true;
				ROS_INFO("Arm-disarm: succsessful");
			} else {
				arm_ack_msg->mav_result = false;
				ROS_INFO("Arm-disarm: fail");
			}
			arm_ack_pub.publish(arm_ack_msg);
		}
	}

	void handle_global_position_int(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_global_position_int_t global_position;
		mavlink_msg_global_position_int_decode(msg, &global_position);

		auto position_msg = boost::make_shared<guidance_node_amsl::Position>();

		//getting new data
		position_msg->Timestamp=global_position.time_boot_ms;
		position_msg->AltitudeRelative=global_position.relative_alt;
		position_msg->AltitudeAMSL = global_position.alt;
		position_msg->Latitude=global_position.lat;
		position_msg->Longitude=global_position.lon;

		//copying data from commonMessage
		position_msg->Yawangle=commonMessage.Yawangle;
		position_msg->Safety = commonMessage.Safety;

		//saving data
		commonMessage.Timestamp=global_position.time_boot_ms;
		commonMessage.AltitudeRelative=global_position.relative_alt;
		commonMessage.AltitudeAMSL=global_position.alt;
		commonMessage.Latitude=global_position.lat;
		commonMessage.Longitude=global_position.lon;

		//DEBUG
//		ROS_INFO("HANDLE POSITION");


		position_pub.publish(position_msg);
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
			/*
			 * If safety is off, I translate velocity in RC values
			 */
			uint16_t vx_RC= (uint16_t)400.0f*(-msg->vxBody)/v_xy_max + 1520;
			uint16_t vy_RC=(uint16_t)400.0f*(msg->vyBody)/v_xy_max + 1520;
			/*
			 * it seems it loiters with 1420 instead of 1500...
			 */
			uint16_t vz_RC= 1420;
			if (msg->vzBody > 0){ //going down, mapped only in 420us
				vz_RC = vz_RC + (uint16_t)320.0f*(-msg->vzBody)/v_z_max;        //was 420
			} else {        //going up, mapped in 580us
				vz_RC = vz_RC + (uint16_t)480.0f*(-msg->vzBody)/v_z_max;     // was 580
			}
			uint16_t v_psi_RC = (uint16_t)400.0f*(msg->yawRate)/v_psi_max + 1520;

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

		velocity_publisher_.publish(velocity_);

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
			ROS_INFO("Arming UAV");
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
			ROS_INFO("Disarming UAV");
		}
	}
};
};	// namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::UniboControllerAMSLPlugin, mavplugin::MavRosPlugin)
