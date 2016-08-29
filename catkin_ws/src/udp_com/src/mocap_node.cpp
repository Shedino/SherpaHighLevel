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
#include "mavros/RCIn.h"
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

void stateCallback(const mavros::RCInConstPtr& state)
  {
	if (state->channels[4] <= 1326) flag2=0;
	if (state->channels[4] > 1326) flag2=1;
	//ROS_INFO("FLAG = %d",flag2);
  }




int main( int argc, char* argv[] ){  
	ros::init(argc, argv, "mocap_node");
	ros::NodeHandle n;
	ros::Subscriber state_subscriber_;
	ros::Publisher velocity_publisher_;
	ros::Rate r(50);
	velocity_publisher_ = n.advertise<mavros::OverrideRCIn>("mavros/rc/override", 10);	
	while (ros::ok()){
		state_subscriber_ = n.subscribe("mavros/rc/in", 1000, stateCallback );
		
	   	if (flag2 == 0){
			//ROS_INFO("MANUALE");
			velocity_.channels[0] = 0;
			velocity_.channels[1] = 0;
			velocity_.channels[2] = 0;
			velocity_.channels[3] = 0;
			velocity_.channels[4] = 0;
			velocity_.channels[5] = 0;
			velocity_.channels[6] = 0;
			velocity_.channels[7] = 0;
			velocity_publisher_.publish(velocity_);					
		}
		if (flag2 == 1){
			//ROS_INFO("Learning");
			do{
				numBytes = clientSocket.recv();
				//Parse mocap dat
				if( numBytes > 0 ){
					const char* buffer = clientSocket.getBuffer();
					unsigned short lenghtPack = *((unsigned short*)(&buffer[0]));
					//printf("packet length:%d\n",lenghtPack);
					// Look for the beginning of a NatNet package
					// printf("%c\n",buffer[0]);
					pos=0;
					if (buffer[0]=='S'){
											//elaborazione pacchetto udp
											pos=2;
											//lettura lenght
											i=0;
											while(buffer[pos]!=' '){
												ch_1[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_1[i]='\0';
											velocity_.channels[0]=flag2*atoi(ch_1);
											//salto lo spazio
											pos++;
											//lettura type
											i=0;
											while(buffer[pos]!=' '){
												ch_2[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_2[i]='\0';
											velocity_.channels[1]=flag2*atoi(ch_2);
											//salto lo spazio
											pos++;
											//lettura x
											i=0;
											while(buffer[pos]!=' '){
												ch_3[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_3[i]='\0';
											velocity_.channels[2]=flag2*atoi(ch_3);
											//salto lo spazio
											pos++;
											//lettura y
											i=0;
											while(buffer[pos]!=' '){
												ch_4[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_4[i]='\0';
											velocity_.channels[3]=flag2*atoi(ch_4);
											//salto lo spazio
											pos++;
											//lettura z
											i=0;
											while(buffer[pos]!=' '){
												ch_5[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_5[i]='\0';
											velocity_.channels[4]=atoi(ch_5);
											//salto lo spazio
											pos++;
											//lettura q1
											i=0;
											while(buffer[pos]!=' '){
												ch_6[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_6[i]='\0';
											velocity_.channels[5]=flag2*atoi(ch_6);
											//salto lo spazio
											pos++;
											//lettura q2
											i=0;
											while(buffer[pos]!=' '){	
												ch_7[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_7[i]='\0';
											velocity_.channels[6]=flag2*atoi(ch_7);
											//salto lo spazio
											pos++;
											//lettura q3
											i=0;
											while(buffer[pos]!=' '){
												ch_8[i]=buffer[pos];
												i++;
												pos++;
											}
											ch_8[i]='\0';
											velocity_.channels[7]=flag2*atoi(ch_8);
											//salto lo spazio
											pos++;	
											//if(buffer[pos]=='E') printf("End of the message\n");
											//printf("info pacchetto: ch_1:%d ch_2:%d ch_3:%d ch_4:%d ch_5:%d ch_6:%d ch_7:%d ch_8:%d \n",velocity_.channels[0], velocity_.channels[1], velocity_.channels[2], velocity_.channels[3], velocity_.channels[4], velocity_.channels[5], velocity_.channels[6], velocity_.channels[7]);
											//invio messaggi sul topic
											//velocity_publisher_.publish(velocity_);
											//loop_rate.sleep();
										}
									
										}
									}while( numBytes > 0 );
								// Don't try again immediately
								//if( !packetread ){
								//	usleep( 10 );
								velocity_publisher_.publish(velocity_);
		}//flag2=1
		
		r.sleep();
		ros::spinOnce();
	}//while ros::ok
	// Process mocap data until SIGINT
	return 0;
}//main
