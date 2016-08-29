//#include <mavros/RCIn.h>
#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>

#include "mavros/Attitude.h"
#include "mavros/Raw_imu.h"
#include "mavros/PositionTarget.h"			// Nico
#include "geometry_msgs/Twist.h"

#include <tf/transform_datatypes.h>
#include <wgs84_ned_lib/wgs84_ned_lib.h>  

namespace mavplugin {
/**
 * @brief Mavros plugin
 */
class UniboControllerUSBPlugin : public MavRosPlugin {
public:
	UniboControllerUSBPlugin() :
		nodeHandle(),
		uas(nullptr),
		v_xy_max(3.0),
		v_z_max(1.5),
		v_psi_max(3.14)
	{};

	void initialize(UAS &uas_)
	{
		uas = &uas_;

		/* --- SUBSCRIPTIONS --- */
		filtered_pos_sub = nodeHandle.subscribe("/pos_filter/pos_vel_out", 1, &UniboControllerUSBPlugin::filtered_pos_cb, this);		// Nico

		/* --- PUBLISHERS --- */
		attitude_pub = nodeHandle.advertise<mavros::Attitude>("/attitude", 10);
		imu_pub = nodeHandle.advertise<mavros::Raw_imu>("/imu", 10);

		//message to send position and velocity to the quadcopter
		mavros::PositionTarget state_;			// Nico
	}

	//should be logic mapping between id number and message type
	const message_map get_rx_handlers() {
		return {
			MESSAGE_HANDLER(MAVLINK_MSG_ID_ATTITUDE, &UniboControllerUSBPlugin::handle_attitude),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_HEARTBEAT, &UniboControllerUSBPlugin::handle_heartbeat),
			MESSAGE_HANDLER(MAVLINK_MSG_ID_RAW_IMU, &UniboControllerUSBPlugin::handle_IMU_read)
		};
	}

private:
	ros::NodeHandle nodeHandle;
	UAS *uas;

	ros::Subscriber filtered_pos_sub;		// Nico
	ros::Publisher attitude_pub;
	ros::Publisher imu_pub;


	mavros::Attitude attitude_msg;  //private and accessed by many handlers
	mavros::Raw_imu imu_;


	//saturation parameters
	/*
	 * Questi tipi di dato sono matlab per specifiche ros del param server.
	 * Nel nodo "guidance_node" sono convertiti a float perchè la legge di guida matlab ha come input
	 * dei float. Adesso non sono convertiti perchè tanto possono essere usati così
	 */
	double v_xy_max, v_z_max, v_psi_max;

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

		//position_pub.publish(position_msg);       //already published when received position_int
		attitude_pub.publish(attitude_msg);
	}

	void handle_heartbeat(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid) {

		mavlink_heartbeat_t heart;
		mavlink_msg_heartbeat_decode(msg, &heart);

		//auto position_msg = boost::make_shared<guidance_node_amsl::Position>();
		ROS_INFO("Heatbeat.");
	}

	void handle_IMU_read(const mavlink_message_t *msg, uint8_t sysid, uint8_t compid){

		mavlink_raw_imu_t raw_imu;
		mavlink_msg_raw_imu_decode(msg, &raw_imu);

		imu_.time_usec = raw_imu.time_usec;
		imu_.xacc = raw_imu.xacc;
		imu_.yacc = raw_imu.yacc;
		imu_.zacc = raw_imu.zacc;
		imu_.xgyro = raw_imu.xgyro;
		imu_.ygyro = raw_imu.ygyro;
		imu_.zgyro = raw_imu.zgyro;
		imu_.xmag = raw_imu.xmag;
		imu_.ymag = raw_imu.ymag;
		imu_.zmag = raw_imu.zmag;

		imu_pub.publish(imu_);
	}
	
	/*
	 * send LOCATION_POS_NED
	 */
	void filtered_pos_cb(const mavros::PositionTarget::ConstPtr& msg){
		// mavlink msg object
		mavlink_message_t msg_mav;
		
		// mavlink_msg_local_position_ned_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, uint32_t time_boot_ms, float x, float y, float z, float vx, float vy, float vz)
		mavlink_msg_local_position_ned_pack(0, 0, &msg_mav, msg->header.seq * 10, (float)msg->position.x, (float)msg->position.y, (float)msg->position.z, (float)msg->velocity.x, (float)msg->velocity.y, (float)msg->velocity.z);
		
		// send message
		UAS_FCU(uas)->send_message(&msg_mav);
		
		/*// prepare GPS_INPUT message
		ros::Time now = ros::Time::now();
		double lat, lon;
		uint32_t time_week_ms = (now.sec * 1000 + now.nsec / 1000) % (1000 * 3600 * 24 * 7); //not correct....
		uint16_t time_week = 0;	// not correct...
		get_pos_WGS84_from_NED (&lat, &lon, msg->position.x, msg->position.y, 44.4924295, 11.3310251);
				
		mavlink_msg_gps_input_pack(0, 0, &msg_mav, msg->header.seq * 10, 1, 0xFF, time_week_ms, time_week, 3, (int32_t)(lat*10e7), (int32_t)(lon*10e7), msg->position.z, 0, 0, msg->velocity.x, msg->velocity.y, msg->velocity.y, 0.01, 0.005, 0.005, 16);

		// send message
		UAS_FCU(uas)->send_message(&msg_mav);*/
	}

};
};	// namespace mavplugin

PLUGINLIB_EXPORT_CLASS(mavplugin::UniboControllerUSBPlugin, mavplugin::MavRosPlugin)
