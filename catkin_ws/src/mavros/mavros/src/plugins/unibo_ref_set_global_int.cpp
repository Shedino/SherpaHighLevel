//#include <mavros/RCIn.h>
#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include "mavros/OverrideRCIn.h"

#include <guidance_node/Directive.h>
#include <guidance_node/MessageHoc.h>


namespace mavplugin {
/**
 * @brief Mavros plugin
 */
class UniboRefPlugin : public MavRosPlugin {
public:
	UniboRefPlugin() :
		nodeHandle(),
		uas(nullptr)
	{};

	void initialize(UAS &uas_)
	{
		uas = &uas_;

		//directive_sub = nodeHandle.subscribe("/directive", 10, &UniboControllerPlugin::directive_cb, this);	//OLD
		//position_pub = nodeHandle.advertise<guidance_node::MessageHoc>("/position", 10);			//OLD
		setpoint_position_pub = nodeHandle.advertise<guidance_node::MessageHoc>("/reference", 10);

		//velocity_publisher_=nodeHandle.advertise<mavros::OverrideRCIn>("mavros/rc/override", 10);		//OLD
	}

	//should be logic mapping between id number and message type
	const message_map get_rx_handlers() {
		return {
			//MESSAGE_HANDLER(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, &UniboControllerPlugin::handle_global_position_int),	//OLD
			//MESSAGE_HANDLER(MAVLINK_MSG_ID_ATTITUDE, &UniboControllerPlugin::handle_attitude),				//OLD
			//MESSAGE_HANDLER(MAVLINK_MSG_ID_RC_CHANNELS_RAW, &UniboControllerPlugin::handle_rc_channels_raw)		//OLD
			MESSAGE_HANDLER(MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT, &UniboRefPlugin::handle_position_target_global_int)
		};
	}

private:
	ros::NodeHandle nodeHandle;
	UAS *uas;

	//ros::Publisher position_pub;          //OLD
	//ros::Subscriber directive_sub;	//OLD
	//ros::Publisher velocity_publisher_;	//OLD
	ros::Publisher setpoint_position_pub;


	void handle_position_target_global_int(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {		

		mavlink_position_target_global_int_t ref_global_position;
		mavlink_msg_position_target_global_int_decode(msg, &ref_global_position);

		auto ref_position_msg = boost::make_shared<guidance_node::MessageHoc>();

		//getting new data
		ref_position_msg->Timestamp=ref_global_position.time_boot_ms;
		ref_position_msg->Altitude=ref_global_position.alt;
		ref_position_msg->Latitude=ref_global_position.lat_int;
		ref_position_msg->Longitude=ref_global_position.lon_int;
		ref_position_msg->Yawangle=ref_global_position.yaw;
		ref_position_msg->Safety=0;                           //not used in reference
		
		//DEBUG
		ROS_INFO("Handle reference. Lat: %d Lon: %d Alt: %d Yaw: %f", ref_global_position.lat_int, ref_global_position.lon_int, ref_global_position.alt, ref_global_position.yaw);

		setpoint_position_pub.publish(ref_position_msg);
	}

};
};	// namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::UniboRefPlugin, mavplugin::MavRosPlugin)
