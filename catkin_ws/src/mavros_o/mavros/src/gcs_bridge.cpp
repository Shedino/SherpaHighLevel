
/**
 * @brief MAVROS GCS proxy
 * @file gcs_bridge.cpp
 * @author Vladimir Ermakov <vooon341@gmail.com>
 */
/*
 * Copyright 2014 Vladimir Ermakov.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */

#include <ros/ros.h>
#include <ros/console.h>

#include <mavros/utils.h>
#include <mavconn/interface.h>

#include <mavros/Mavlink.h>

#include "guidance_node_amsl/Reference.h"  //MIC

using namespace mavros;
using namespace mavconn;

ros::Publisher mavlink_pub;
ros::Subscriber mavlink_sub;
MAVConnInterface::Ptr gcs_link;
 /*---MIC added------}*/
//ros::NodeHandle _node;
ros::Publisher setpoint_position_pub;
auto ref = boost::make_shared<guidance_node_amsl::Reference>();
 /*------------- */

void mavlink_pub_cb(const mavlink_message_t *mmsg, uint8_t sysid, uint8_t compid) { 
	MavlinkPtr rmsg = boost::make_shared<Mavlink>();

	/*---MIC added------*/
	if (mmsg->msgid == 87){   //received mavlink_msg_global_position_int
		//ROS_INFO("Received message id 87");
		mavlink_position_target_global_int_t ref_global_position;
		mavlink_msg_position_target_global_int_decode(mmsg, &ref_global_position);

		//ref->Timestamp=ref_global_position.time_boot_ms;   
		ref->AltitudeRelative=ref_global_position.alt;         //in mm
		ref->Latitude=ref_global_position.lat_int;
		ref->Longitude=ref_global_position.lon_int;
		ref->Yawangle=ref_global_position.yaw;
		ref->Mode = 0;
		//ref->Safety=0;                           //not used in reference

		setpoint_position_pub.publish(ref);
		//ROS_INFO("Published ref");
	}
	/* ------------- */

	rmsg->header.stamp = ros::Time::now();
	mavutils::copy_mavlink_to_ros(mmsg, rmsg);
	mavlink_pub.publish(rmsg);
};

void mavlink_sub_cb(const Mavlink::ConstPtr &rmsg) {
	mavlink_message_t mmsg;
	
	if (mavutils::copy_ros_to_mavlink(rmsg, mmsg)){
		gcs_link->send_message(&mmsg, rmsg->sysid, rmsg->compid);
		//ROS_INFO("BRIDGE: Sending to UDP (probably)");
	} else
		ROS_ERROR("Packet drop: illegal payload64 size");
};

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "gcs_bridge");
	ros::NodeHandle priv_nh("~");
	ros::NodeHandle mavlink_nh("/mavlink");
	/*---MIC added------*/
	ros::NodeHandle references;
	setpoint_position_pub = references.advertise<guidance_node_amsl::Reference>("/reference", 10);
	/* ------------- */
	

	std::string gcs_url;
	priv_nh.param<std::string>("gcs_url", gcs_url, "udp://@");

	try {
		gcs_link = MAVConnInterface::open_url(gcs_url);
		//ROS_INFO("BRIDGE: url: %s",gcs_url.c_str());       //DEBUG MIC
	}
	catch (mavconn::DeviceError &ex) {
		ROS_FATAL("GCS: %s", ex.what());
		return 1;
	}

	mavlink_pub = mavlink_nh.advertise<Mavlink>("to", 10);
	gcs_link->message_received.connect(mavlink_pub_cb);

	mavlink_sub = mavlink_nh.subscribe("from", 10, mavlink_sub_cb,
		ros::TransportHints()
			.unreliable()
			.maxDatagramSize(1024));

	ros::spin();
	return 0;
}

