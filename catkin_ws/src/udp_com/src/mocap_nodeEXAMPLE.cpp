/*
 * mocap_node.cpp
 *
 *  Created on: 02/02/2015
 *      Author:
 */

#include "socket.h"
#include <ros/ros.h>
#include "mavros/OverrideRCIn.h"
#include "mavros/State.h"
#include <string>
#include <unistd.h>

const int LOCAL_PORT = 54321;


mavros::OverrideRCIn velocity_;
int flag2 = 0;
bool packetread = false;
int numBytes = 0;
int pos,i;
char ch_1[10], ch_2[10], ch_3[10], ch_4[10], ch_5[10], ch_6[10], ch_7[10], ch_8[10];
int lun, t;   
UdpSocket clientSocket(LOCAL_PORT);
std::string manual_mode="CMODE(0)";
std::string auto_mode="CMODE(2)";



/*void processUdpPacket(ros::NodeHandle n){
						
}*/

void stateCallback(const mavros::StateConstPtr& state)
  {
	if (state->mode == manual_mode) flag2=0;
	if (state->mode == auto_mode) flag2=1;
	ROS_INFO("FLAG = %d",flag2);
  }




int main( int argc, char* argv[] ){  
	ros::init(argc, argv, "mocap_node");
	ros::NodeHandle n;
	ros::Subscriber state_subscriber_;
	ros::Publisher velocity_publisher_;
	ros::Rate r(10);
	while(ros::ok()){
	
		state_subscriber_ = n.subscribe("mavros/state", 1000, stateCallback );
		
	   	if (flag2 == 0){
			ROS_INFO("MANUALE");
			velocity_.channels[0] = 0;
			velocity_.channels[1] = 0;
			velocity_.channels[2] = 0;
			velocity_.channels[3] = 0;
			velocity_.channels[4] = 0;
			velocity_.channels[5] = 0;
			velocity_.channels[6] = 0;
			velocity_.channels[7] = 0;	
			velocity_publisher_ = n.advertise<mavros::OverrideRCIn>("mavros/rc/override", 10);
			r.sleep();				
		}
		
	ros::spinOnce();
	}
	// Process mocap data until SIGINT
	return 0;
}//main
