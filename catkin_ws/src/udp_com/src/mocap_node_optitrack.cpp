/*
 * mocap_node.cpp
 *
 *  Created on: 02/02/2015
 *      Author:
 */

#include "socket.h"
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <string>
#include <unistd.h>

const int LOCAL_PORT = 54321;


geometry_msgs::Pose optitrack_position;
int flag2 = 0;
bool packetread = false;
int numBytes = 0;
float sqrt_norm;
int x,y,z,w;
int pos,i;
char ch_1[10], ch_2[10], ch_3[10], ch_4[10], ch_5[10], ch_6[10], ch_7[10], ch_8[10], ch_9[10], ch_10[10], ch_11[10], ch_12[10];
int lun, t;   
UdpSocket clientSocket(LOCAL_PORT);

int main( int argc, char* argv[] ){  
	ros::init(argc, argv, "mocap_node");
	ros::NodeHandle n;
	ros::Publisher optitrack_publisher_;
	ros::Rate r(50);
	optitrack_publisher_ = n.advertise<geometry_msgs::Pose>("mavros/mocap/pose", 10);	
	while (ros::ok()){
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
					//optitrack_positionpose.position.x=flag2*atoi(ch_1);
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
					//velocity_.channels[1]=flag2*atoi(ch_2);
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
					optitrack_position.position.x=atoi(ch_3);
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
					optitrack_position.position.y=atoi(ch_4);
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
					optitrack_position.position.z=atoi(ch_5);
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
					//optitrack_position.orientation.w=atoi(ch_6)/10000;// w/10000
					w=atoi(ch_6)/10000;
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
					//optitrack_position.orientation.x=atoi(ch_7);
					x=atoi(ch_7);
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
					//optitrack_position.orientation.y=atoi(ch_8);
					y=atoi(ch_8);
					//salto lo spazio
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_8[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_9[i]='\0';
					//optitrack_position.orientation.z=atoi(ch_8);
					z=atoi(ch_8);
					sqrt_norm=sqrt(w*w+x*x+y*y+z*z);
					optitrack_position.orientation.w=w/sqrt_norm;
					optitrack_position.orientation.x=x/sqrt_norm;
					optitrack_position.orientation.y=y/sqrt_norm;					
					optitrack_position.orientation.z=z/sqrt_norm;
				}
			}
		}while( numBytes > 0 );
		optitrack_publisher_.publish(optitrack_position);
		r.sleep();
		ros::spinOnce();
	}//while ros::ok
	// Process mocap data until SIGINT
	return 0;
}//main
