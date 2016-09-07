#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <iostream>
#include "setup.h"

// mavros
#include "mavros/Attitude.h"
//#include "mavros/mavros_uas.h" // requires c++ 2011 standard support

// msgs
#include "pos_filter/tf_msg.h"

#include <sstream>

//global variables
int attUpdateFlag = 0;
ros::Time timestamp;
double mapOffset = 0.0;

// msg buffer
mavros::Attitude attitude;							// attitude msg object
pos_filter::tf_msg tf_msg;

//function prototypes
void attitudeCallback(const mavros::Attitude::ConstPtr& msg);


//main
int main(int argc, char **argv)
{
/*** ROS init ***/
  ros::init(argc, argv, "pos_filter_tf");

  ros::NodeHandle n;

  ros::Rate loop_rate(FREQUENCY);					// frequency of loop
  
// subscribers
  ros::Subscriber attitude_sub = n.subscribe("/attitude", 10, &attitudeCallback);

// publishers
  ros::Publisher pos_tf_pub = n.advertise<pos_filter::tf_msg>("/pos_filter/tf_msg", 1);

// transforms
  tf::TransformBroadcaster odom_broadcaster;

    ros::Duration(1).sleep();
    ros::spinOnce();

  while (!attUpdateFlag){	// wait for first attitude msg
    ROS_WARN_STREAM("no attitude message received!");
    ros::Duration(1).sleep();
    ros::spinOnce();
  }

/*** inf loop ***/
  while (ros::ok())
  {
	ros::spinOnce();
		  
    timestamp = ros::Time::now();

    if ( attUpdateFlag ){
      geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(attitude.roll, -attitude.pitch, -mapOffset);

      geometry_msgs::TransformStamped odom_trans;
      odom_trans.header.stamp = ros::Time::now();
      odom_trans.header.frame_id = "odom";
      odom_trans.child_frame_id = "base";

      odom_trans.transform.translation.x = 0.0;
      odom_trans.transform.translation.y = 0.0;
      odom_trans.transform.translation.z = 1.0;
      odom_trans.transform.rotation = odom_quat;
	
      odom_broadcaster.sendTransform(odom_trans);

      tf_msg.timestamp = timestamp;
      tf_msg.mapOffset = mapOffset;
      tf_msg.init_done = 1;

      pos_tf_pub.publish(tf_msg);				// send msg

      attUpdateFlag = 0;
    }

    loop_rate.sleep();
  }

  return 0;
}

void attitudeCallback(const mavros::Attitude::ConstPtr& msg){
	static int initDone = 0;

	if (!initDone){
		mapOffset = msg->yaw;
		initDone = 1;
	}

	attitude = *msg;

	attUpdateFlag = 1;
}



