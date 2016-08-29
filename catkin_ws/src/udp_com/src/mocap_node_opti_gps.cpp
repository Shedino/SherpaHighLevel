/*
 * mocap_node.cpp
 *
 *  Created on: 02/03/2015
 *      Author: Dott. Antonio Mauto Galiano
		antoniomauro.galiano@gmail.com
 */

#include "socket.h"
#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <unistd.h>
#include "std_msgs/String.h"
#include <sstream>
#include <dynamic_reconfigure/server.h>
#include <tf/transform_datatypes.h>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>
#include <stdint.h>
#include <math.h>
#include <ctime>

#define PI_G 3.14159265358979323846 

using namespace std;

const int LOCAL_PORT = 54321;

int cycle;
int i2cerrors;
int errorcode;
geometry_msgs::Pose optitrack_position;
int flag2 = 0;
bool packetread = false;
int numBytes = 0;
double vel_x, vel_y, vel_z, w, x,y,z, sqrt_norm, new_vel_x, new_vel_y, new_vel_z, speed, g_speed;
int pos,i;
char ch_1[10], ch_2[10], ch_3[10], ch_4[10], ch_5[10], ch_6[10], ch_7[10], ch_8[10], ch_9[10], ch_10[10], ch_11[10], ch_12[10], ch_13[10], ch_14[10], ch_15[10];
int lun, t;   
UdpSocket clientSocket(LOCAL_PORT);

uint8_t setNavOld[] = {0};

uint8_t setNavOld1[] = {0};

uint8_t setNavOld2[] = {0};

uint8_t setNav[] = {
    /*0xB5, 0x62,
    0x0D, 0x01,
    0x10, 0x00,
    0x10, 0x3F, 0x66, 0x18,//i=6-->9
    0x00, 0x00, 0x00, 0x00,
    0x98, 0xE9, 0xFF, 0xFF,
    0x2C, 0x07,
    0x02,
    0x00,
    0x9F, 0x91,//TIME*/
    
    0xB5, 0x62, //HEADER i=24,25	Newi=0,1
    0x01, 0x06, //ID MESSAGE i=26,27
    0x34, 0x00, //LENGHT i=28,29
    0x1C, 0x3D, 0x66, 0x18, //GPS MILLISECONDS i=30-->33
    0xB6, 0x6D, 0xFC, 0xFF, //FRACTIONAL NANOSECOND REMINDER i=34-->37
    0x2C, 0x07, //WEEK i=38,39          Newi= 14,15
    0x03, //GPS FIX i=40
    0x0F, //FLAGS i=41
    0x7B, 0x7A, 0xA2, 0x1A,//i=42-->45
    0x5E, 0x2F, 0x56, 0x05,//i=46-->49   Newi=22-->25
    0xAE, 0x20, 0x82, 0x1A,//i=50-->53	
    0x0A, 0x00, 0x00, 0x00,//i=54-->57
    0xF6, 0xFF, 0xFF, 0xFF,//i=58-->61	Newi=34-->37
    0x14, 0x00, 0x00, 0x00,//i=62-->65
    0x00, 0x00, 0x00, 0x00,//i=66-->69
    0xD8, 0x00, 0x00, 0x00, //i=70-->73
    0x0A, 0x00, //PDOP i=74,75         Newi=50,51
    0x02,//i=76		Newi=52
    0x0A,//NUM SATELLITES i=77
    0x24, //i=78
    0x67, 0x01, 0x00, //i=79,80,81
    0x05, 0x90,//NAVIGATION_SOLUTION_INFORMATION i=82,83
    
    0xB5, 0x62, //HEADER i=84,85	Newi=60,61
    0x01, 0x03, //ID MESSAGE i=86,87
    0x10, 0x00, //MESSAGE LENGHT i=88,89	Newi=64,65
    0x1C, 0x3D, 0x66, 0x18, //GPS MILLISECONDS i=90 --> i=93
    0x03, //GPS-FIX STATUS i=94		Newi=70
    0x0F, //Navigation Status Flags i=95	Newi=71
    0xC1, //Fix Status Information  i=96
    0x00, // flags2 i=97
    0x6E, 0x57, 0x3E, 0x00, //Time to first fix i=98 -- >i=101
    0x6F, 0xFD, 0x45, 0x00, //Milliseconds since Startup / Reset i=102 --> i=105
    0xB1, 0xD7,//NAVIGATION_STATUS i=106,107	Newi=82,83


    0xB5, 0x62, //HEADER i=108.109	Newi=84,85
    0x01, 0x02, //ID MESSAGE i=110,111
    0x1C, 0x00, //MESSAGE LENGHT i=112,113
    0x1C, 0x3D, 0x66, 0x18,//GPS MILLISECOND TIME i=114 --> i=117	Newi=90-->93
    0xCC, 0xD3, 0xB0, 0x06,//LONGITUDE DEGREE  possible i=118 --> i=121	Newi=94-->97
    0x26, 0x00, 0x85, 0x1A,//LATITUDE DEGREE i=122 --> i=125		Newi=98-->101
    0x5A, 0x8D, 0x01, 0x00,//HEIGHT ABOUT ELLIPSOIDE MM i=126 --> i=129
    0x75, 0xDB, 0x00, 0x00,//HEIGHT ABOVE SEA LEVEL MM i=130 --> i=133
    0x0A, 0x00, 0x00, 0x00,//HORIZONTAL ACCURACY MM i=134 --> i=137
    0x4B, 0x3C, 0x00, 0x00,//VERTICAL ACCURACY MM i=138 --> i= 141
    0xED, 0x12,//CKSA, CKSB (POSITION_LLH END) i=142,143		Newi=118,119

    0xB5, 0x62,//i=144,145		Newi=120,121
    0x01, 0x12,//i=146,147
    0x24, 0x00,//i=148,149		Newi=124,125
    0x1C, 0x3D, 0x66, 0x18,//		i=150->153
    0x00, 0x00, 0x00, 0x00,// velN cm/s i=154->157	Newi=130-->133
    0x00, 0x00, 0x00, 0x00,// velE cm/s i=158->161
    0x00, 0x00, 0x00, 0x00,// velD cm/s i=162->165
    0x00, 0x00, 0x00, 0x00,// speed 3D cm/s i=166->169
    0x00, 0x00, 0x00, 0x00,//i=170->173
    0xC6, 0xCD, 0x16, 0x01,//Heading 2D deg i=174->177	Newi=150-->153
    0x0A, 0x00, 0x00, 0x00,//i=178->181
    0x91, 0x11, 0x46, 0x00,//i=182->185
    0xC3, 0x5C}; //i=186,187 VELOCITY_NED

/*uint8_t check[] = {0x01, 0x02, //ID MESSAGE i=110,111
    0x1C, 0x00, //MESSAGE LENGHT i=112,113
    0x1C, 0x3D, 0x66, 0x18,//GPS MILLISECOND TIME i=114 --> i=117
    0xCC, 0xD3, 0xB0, 0x06,//LONGITUDE DEGREE  possible i=118 --> i=121
    0x26, 0x00, 0x85, 0x1A,//LATITUDE DEGREE i=122 --> i=125 -->i=15
    0x5A, 0x8D, 0x01, 0x00,//HEIGHT ABOUT ELLIPSOIDE MM i=126 --> i=129
    0x75, 0xDB, 0x00, 0x00,//HEIGHT ABOVE SEA LEVEL MM i=130 --> i=133
    0xF2, 0x2C, 0x00, 0x00,//HORIZONTAL ACCURACY MM i=134 --> i=137
    0x4B, 0x3C, 0x00, 0x00}; //LLH CHECK

uint8_t check2[] = {0x01, 0x06, //ID MESSAGE
    0x34, 0x00, //LENGHT
    0x1C, 0x3D, 0x66, 0x18, //GPS MILLISECONDS
    0xB6, 0x6D, 0xFC, 0xFF, //FRACTIONAL NANOSECOND REMINDER 
    0x2C, 0x07, //WEEK
    0x03, //GPS FIX
    0x0F, //FLAGS
    0x7B, 0x7A, 0xA2, 0x1A,
    0x5E, 0x2F, 0x56, 0x05,
    0xAE, 0x20, 0x82, 0x1A,
    0x85, 0x07, 0x00, 0x00,
    0xF6, 0xFF, 0xFF, 0xFF,
    0x14, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xD8, 0x00, 0x00, 0x00, 
    0x64, 0x00, //PDOP
    0x02,
    0x0A,
    0x24,
    0x67, 0x01, 0x00};//NAVIGATION SOLUTION

uint8_t check3[] = {0x01, 0x03, //ID MESSAGE
    0x10, 0x00, //MESSAGE LENGHT
    0x1C, 0x3D, 0x66, 0x18, //GPS MILLISECONDS
    0x03, //GPS-FIX STATUS
    0x0F, //Navigation Status Flags i=95
    0xC1, //Fix Status Information
    0x00, // flags2
    0x6E, 0x57, 0x3E, 0x00, //Time to first fix
    0x6F, 0xFD, 0x45, 0x00};//NAVIGATIO STATUS CHECK*/


//OPEN SERIAL PORT
int open_port(char* port){
	int fd; // File descriptor for the port
	 // Open serial port
	 // O_RDWR - Read and write
	 // O_NOCTTY - Ignore special chars like CTRL-C
	//fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK );
	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY );
	if (fd < 0){
		ROS_INFO("porta non aperta correttamente");
		return(-1);
	}
	return (fd);
	}

//SETUP SERIAL PORT
bool setup_port(int fd, int baud, int data_bits, int stop_bits, bool parity, bool hardware_control){
	struct termios  config;
	do {
		 if(!isatty(fd)){
			 fprintf(stderr, "\nERROR: file descriptor %d is NOT a serial port\n", fd);
			 return false;
		 }
		if(tcgetattr(fd, &config) < 0){
			fprintf(stderr, "\nERROR: could not read configuration of fd %d\n", fd);
			break;
		}
		//
		// Input flags - Turn off input processing
		// convert break to null byte, no CR to NL translation,
		// no NL to CR translation, don't mark parity errors or breaks
		// no input parity check, don't strip high bit off,
		// no XON/XOFF software flow control
		//
		config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
		//
		// Output flags - Turn off output processing
		// no CR to NL translation, no NL to CR-NL translation,
		// no NL to CR translation, no column 0 CR suppression,
		// no Ctrl-D suppression, no fill characters, no case mapping,
		// no local output processing
		//
		/*config.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OPOST);
		#ifdef OLCUC
			config.c_oflag &= ~OLCUC;
		#endif

		#ifdef ONOEOT
			config.c_oflag &= ~ONOEOT;
  		#endif*/
		config.c_oflag = 0;
		//
		// No line processing:
		// echo off, echo newline off, canonical mode off,
		// extended input processing off, signal chars off
		//
		config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
		//
		// Turn off character processing
		// clear current char size mask, no parity checking,
		// no output processing, force 8 bit input
		//
		config.c_cflag &= ~(CSIZE | PARENB);
		config.c_cflag |= CS8;
		//
		// One input byte is enough to return from read()
		// Inter-character timer off
		//
		config.c_cc[VMIN]  = 1;
		config.c_cc[VTIME] = 10; // was 0

		if (cfsetispeed(&config, baud) < 0 || cfsetospeed(&config, baud) < 0){
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			break;
		}

		//
		// Finally, apply the configuration
		//
		if(tcsetattr(fd, TCSANOW, &config) < 0){
			fprintf(stderr, "\nERROR: could not set configuration of fd %d\n", fd);
			break;
		}
		return true;
	}while(0);
	return false;
}

//CLOSE SERIAL PORT
void close_port(int fd){
	close(fd);
}

//READ AND WRITE
bool ubx_write(int fd){
    int controllo = write(fd,  setNav, sizeof(setNav));
//	if (controllo > 0) ROS_INFO("IM SENDING");
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

int main( int argc, char* argv[] ){  
	ros::init(argc, argv, "gps_optitrack_node");
	ros::NodeHandle n;
	ros::NodeHandle pnh("~");
	ros::Publisher optitrack_publisher_;
	ros::Rate r(5);
	uint32_t CK_A = 0, CK_B = 0, CK_C = 0, CK_D = 0, CK_E = 0, CK_F = 0, CK_G = 0, CK_H = 0, CK_I = 0, CK_L = 0;
 	uint8_t sum1=0x00, sum2=0x00, sum3=0x00, sum4=0x00, sum5=0x00, sum6=0x00,  sum7=0x00, sum8=0x00, sum9=0x00, sum10=0x00, flag_lat=0x00, flag_lon=0x00;;
	double R; //According to WGS84
	double f_test; //According to WGS84
	double R_N, R_M, Lat0, Lat, Lon0, Lon, sqrt_test, Alt;
	double new_x, new_y, heading, yaw_local;
	uint8_t bArray_Lat [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Alt [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Lon [ sizeof( double ) ] = { 0 };
	
	uint8_t bArray_Heading [ sizeof( double ) ] = { 0 };

	uint8_t bArray_Velx [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Vely [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Velz [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Speed [ sizeof( double ) ] = { 0 };
	uint8_t bArray_Gspeed [ sizeof( double ) ] = { 0 };

	uint8_t bArray_Time [ sizeof( long ) ] = { 0 };
	uint8_t bArray_BeginTime [ sizeof( long ) ] = { 0 };
	f_test=1/298.257223563;
	R  = 6378137;
	double sin_lat;
	int count;
	int count_nav=0;
	double time_begin, time_now, time_flag;
	
	time_begin = (ros::Time::now().toSec())*1000;
	//ROS_INFO("TIME %f",time_begin);
	bArray_BeginTime[3] = (int) time_begin >> 24;
	bArray_BeginTime[2] = (int) time_begin >> 16;
	bArray_BeginTime[1] = (int) time_begin >> 8;
	bArray_BeginTime[0] = (int) time_begin;
	
	
	
	//ROS_INFO("setNav[101] %d   setNav[100] %d  setNav[99] %d  setNav[98] %d",setNav[101], setNav[100], setNav[99], setNav[98]);

	/*time_t t = time(0);
   	struct  tm * now = localtime(&t);
    	cout << now->tm_hour << " " << now->tm_min << " " << now->tm_sec << endl;*/

	std::string serial_port;
  	pnh.param<std::string>("serial_port", serial_port, "/dev/ttyUSB0");
	int serial_baud;
	pnh.param<int>("serial_baud", serial_baud, B38400);
	
	char *usb_port = new char[serial_port.length() + 1];
	strcpy(usb_port, serial_port.c_str());
	
	int fd=open_port(usb_port);
	if(fd<0){
		ROS_INFO("Errore durante l'apertura della porta");
		fflush(stdout);
		exit(-1);
	} else ROS_INFO("Porta aperta correttamente");
	bool setup=setup_port(fd,serial_baud,8,1,false,false);
	if(!setup){
		ROS_INFO("Errore di setup della porta");
		exit(-2);
	}else ROS_INFO("Setup porta completato");
	optitrack_publisher_ = n.advertise<geometry_msgs::Pose>("mavros/mocap/pose", 10);
	
		
	//ROS_INFO("NUMBER OF SATELLITE%d setNav[75]%d setNav[74]%d", setNav[77], setNav[75], setNav[74]);
	while (ros::ok()){
		//ROS_INFO("BEGIN LOOP  %f", ros::Time::now().toSec());
		do{
			numBytes = clientSocket.recv();
			//Parse mocap dat
			if( numBytes > 0 ){
				const char* buffer = clientSocket.getBuffer();
				unsigned short lenghtPack = *((unsigned short*)(&buffer[0]));
				//printf("packet length:%d\n",lenghtPack);
				// Look for the beginning of a NatNet package
				//printf("%c\n",buffer[0]);
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
					//ROS_INFO("%d  %f", count_nav, optitrack_position.position.x);
					//count_nav++;
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
					//ROS_INFO("X%f Y%f Z%f",optitrack_position.position.x,optitrack_position.position.y,optitrack_position.position.z);
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
					w=atof(ch_6)/10000;
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
					x=atof(ch_7)/10000;
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
					y=atof(ch_8)/10000;
					//salto lo spazio
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_9[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_9[i]='\0';
					//optitrack_position.orientation.z=atoi(ch_8);
					z=atof(ch_9)/10000;
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_10[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_10[i]='\0';
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);
					vel_x=atof(ch_10);
					//ROS_INFO("VEL_X %f", vel_x);
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_11[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_11[i]='\0';
					vel_y=atof(ch_11);
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_12[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_12[i]='\0';
					vel_z=atof(ch_12);
					//pos++;
					//if(buffer[pos]=='E') printf("End of the message\n");
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);
					/*pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_13[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_13[i]='\0';
					//vel_x=atoi(ch_13);
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_14[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_14[i]='\0';
					vel_y=atof(ch_14);
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);
					pos++;
					i=0;
					while(buffer[pos]!=' '){
						ch_15[i]=buffer[pos];
						i++;
						pos++;
					}
					ch_15[i]='\0';
					vel_z=atof(ch_15);
					//optitrack_position.orientation.z=atoi(ch_8);
					//z=atoi(ch_8);*/
					//ROS_INFO("vel_x %f   vel_y %f  vel_z %f",vel_x, vel_y, vel_z);
					
					sqrt_norm=sqrt(w*w+x*x+y*y+z*z);
					optitrack_position.orientation.w=w/sqrt_norm;
					optitrack_position.orientation.x=x/sqrt_norm;
					optitrack_position.orientation.y=y/sqrt_norm;					
					optitrack_position.orientation.z=z/sqrt_norm;
					
					yaw_local=atan2((optitrack_position.orientation.x*optitrack_position.orientation.y+optitrack_position.orientation.w*optitrack_position.orientation.z),(optitrack_position.orientation.w*optitrack_position.orientation.w+optitrack_position.orientation.x*optitrack_position.orientation.x-optitrack_position.orientation.y*optitrack_position.orientation.y-optitrack_position.orientation.z*optitrack_position.orientation.z));//yaw in local frame
					//ROS_INFO("YAW LOCAL %f",(yaw_local* (180 / PI_G)) );
					heading=((yaw_local * (180 / PI_G)) - 75)* 100000; //degree * 100000
	                                //ROS_INFO("HEADING %f",heading);
					bArray_Heading[3] = (int) heading >> 24;
                                        bArray_Heading[2] = (int) heading >> 16;
                                        bArray_Heading[1] = (int) heading >> 8;
                                        bArray_Heading[0] = (int) heading;

					
					//NED2LLA Estimate geodetic latitude, longitude, and altitude from flat Earth position
					//function [Lat,Lon] = NED2LLA( optitrack_position.position.x , optitrack_position.position.y, N0, E0, Lat0, Lon0)
					//ROS_INFO("X %f   Y %f   Z %f",optitrack_position.position.x, optitrack_position.position.y, optitrack_position.position.z);
					//ROS_INFO("OW %f   OX %f   OY %f   OZ %f", w, x, y, z);	
					
					//convert Lat0 and Lon0 to radians
					Lat0 = 44.492395 * (PI_G / 180.0);
					Lon0 = 11.330142 * (PI_G / 180.0);
					//Lat0 = 44.492395;
                                        //Lon0 = 11.330142;
					//ROS_INFO("Latitude0 %f   Longitude0 %f RADIAN",Lat0, Lon0);
					
					/*Lat0 = Lat0 * (180.0 / PI_G);
					Lon0 = Lon0 * (180.0 / PI_G);
					ROS_INFO("Latitude0 %f   Longitude0 %f DEGREE",Lat0, Lon0);
					ROS_INFO("f_test %f ",f_test);*/

					// To convert the North and East coordinates to geodetic latitude and longitude, 
					// the radius of curvature in the prime vertical (R_N) and the radius of curvature 
					// in the meridian (R_M) are used. (R_N) and (R_M) are defined by the following relationships:
					
					R_N = R / sqrt( 1 -  (( (2*f_test) - (f_test*f_test) )  *   (sin(Lat0) * sin(Lat0) ) )   ) ;
					
					//ROS_INFO("R %f ",R);
					//ROS_INFO("R_N %f ",R_N);
					/*R_N = R/sqrt( 1 - ( ((2*0.003352811)-(0.003352811*0.003352811)) * (sin(Lat0)*sin(Lat0)) ) );
					sin_lat=sin(Lat0);
					ROS_INFO("sin_Lat %f ",sin_lat);
					ROS_INFO("f_test %f ",f_test);
					sqrt_test=sqrt( 1- ( ((2*0.003352811)-(0.003352811*0.003352811)) * (sin(Lat0)*sin(Lat0)) ) );
					ROS_INFO("SQRT %d ",sqrt_test);
					ROS_INFO("R_N %d ",R_N);*/
					R_M = R_N * (        (      (1 - ((2*f_test)-(f_test*f_test)))    /    (1-( (2*f_test)-(f_test*f_test) ) * (sin(Lat0) * sin(Lat0)))     )   );
					//ROS_INFO("R_M %f ",R_M);	
					//R_M = R_N * (  (1-(2*0.003352811-pow(0.003352811,2))) /(1-(2*0.003352811-pow(0.003352811,2))*(pow(sin(Lat0),2))));

					//where (R) is the equatorial radius of the planet and(f) is the flattening of the planet.
					//According to WGS84: R = 6378137 m and f= 1/298.257223563;
					
					new_x=(cos((-75*PI_G)/180)*optitrack_position.position.x) - (sin((-75*PI_G)/180)*optitrack_position.position.y);
					new_y=(sin((-75*PI_G)/180)*optitrack_position.position.x) + (cos((-75*PI_G)/180)*optitrack_position.position.y);
					
					new_vel_x=((cos((-75*PI_G)/180)*vel_x) - (sin((-75*PI_G)/180)*vel_y))*100;// cm/s
					new_vel_y=((sin((-75*PI_G)/180)*vel_x) + (cos((-75*PI_G)/180)*vel_y))*100;// cm/s
					new_vel_z=vel_z*100; // cm/s
					speed=sqrt( (new_vel_x*new_vel_x)+(new_vel_y*new_vel_y)+(new_vel_z*new_vel_z) );
					g_speed=sqrt( (new_vel_x*new_vel_x)+(new_vel_y*new_vel_y) );
					bArray_Velx[3] = (int) new_vel_x >> 24;
					bArray_Velx[2] = (int) new_vel_x >> 16;
					bArray_Velx[1] = (int) new_vel_x >> 8;
					bArray_Velx[0] = (int) new_vel_x;
					bArray_Vely[3] = (int) new_vel_y >> 24;
					bArray_Vely[2] = (int) new_vel_y >> 16;
					bArray_Vely[1] = (int) new_vel_y >> 8;
					bArray_Vely[0] = (int) new_vel_y;
					bArray_Velz[3] = (int) new_vel_z >> 24;
					bArray_Velz[2] = (int) new_vel_z >> 16;
					bArray_Velz[1] = (int) new_vel_z >> 8;
					bArray_Velz[0] = (int) new_vel_z;
					bArray_Speed[3] = (int) speed >> 24;
					bArray_Speed[2] = (int) speed >> 16;
					bArray_Speed[1] = (int) speed >> 8;
					bArray_Speed[0] = (int) speed;
					bArray_Gspeed[3] = (int) g_speed >> 24;
					bArray_Gspeed[2] = (int) g_speed >> 16;
					bArray_Gspeed[1] = (int) g_speed >> 8;
					bArray_Gspeed[0] = (int) g_speed;
					
					//ROS_INFO("NEW_X %f   NEW_Y %f",new_x, new_y);
					Lat = Lat0 + (atan( 1/R_M ) * (new_x/1000));
					Lon = Lon0 + (atan( 1 / (R_N*cos(Lat)) ) * (new_y/1000));
					//Lat = (Lat0 + 0.00001 * (new_x/1000))*10000000;
					//Lon = (Lon0 + 0.00001 * (new_y/1000))*10000000;
					//ROS_INFO("Latitude %f   Longitude %f RADIAN",Lat, Lon);					
					
					Lat = (Lat * (180.0 / PI_G)) * 10000000;
					Lon = (Lon * (180.0 / PI_G)) * 10000000;
					
					
					//ROS_INFO("Latitude %f   Longitude %f DEGREE",Lat, Lon);
					

					Alt = - (optitrack_position.position.z);
					
					

					time_flag = (ros::Time::now().toSec())*1000;
					time_now = time_flag - time_begin;
					
					//ROS_INFO("TIME FROM FIRST FIX %f",time_now);
					bArray_Time[3] = (int) time_now >> 24;
					bArray_Time[2] = (int) time_now >> 16;
					bArray_Time[1] = (int) time_now >> 8;
					bArray_Time[0] = (int) time_now;
					//ROS_INFO("Altitude %f",Alt);
					bArray_Alt[3] = (int) Alt >> 24;
					bArray_Alt[2] = (int) Alt >> 16;
					bArray_Alt[1] = (int) Alt >> 8;
					bArray_Alt[0] = (int) Alt;
					///memcpy( bArray_Lat , &Lat , sizeof( double ) );
					bArray_Lat[3] = (int) Lat >> 24;
					bArray_Lat[2] = (int) Lat >> 16;
					bArray_Lat[1] = (int) Lat >> 8;
					bArray_Lat[0] = (int) Lat;
					//memcpy( bArray_Lon , &Lon , sizeof( double ) );
					bArray_Lon[3] = (int) Lon >> 24;
					bArray_Lon[2] = (int) Lon >> 16;
					bArray_Lon[1] = (int) Lon >> 8;
					bArray_Lon[0] = (int) Lon;
					//LAST FIX TIME
					/*setNav[77]=bArray_BeginTime[3] &0xff;
					setNav[76]=bArray_BeginTime[2] &0xff;
					setNav[75]=bArray_BeginTime[1] &0xff;
					setNav[74]=bArray_BeginTime[0] &0xff;*/
					
					//VELOCITY NED
					setNav[133]=bArray_Velx[3] &0xff;
					setNav[132]=bArray_Velx[2] &0xff;
					setNav[131]=bArray_Velx[1] &0xff;
					setNav[130]=bArray_Velx[0] &0xff;

					setNav[137]=bArray_Vely[3] &0xff;
					setNav[136]=bArray_Vely[2] &0xff;
					setNav[135]=bArray_Vely[1] &0xff;
					setNav[134]=bArray_Vely[0] &0xff;

					setNav[141]=bArray_Velz[3] &0xff;
					setNav[140]=bArray_Velz[2] &0xff;
					setNav[139]=bArray_Velz[1] &0xff;
					setNav[138]=bArray_Velz[0] &0xff;
					
					setNav[145]=bArray_Speed[3] &0xff;
					setNav[144]=bArray_Speed[2] &0xff;
					setNav[143]=bArray_Speed[1] &0xff;
					setNav[142]=bArray_Speed[0] &0xff;
					
					setNav[149]=bArray_Gspeed[3] &0xff;
					setNav[148]=bArray_Gspeed[2] &0xff;
					setNav[147]=bArray_Gspeed[1] &0xff;
					setNav[146]=bArray_Gspeed[0] &0xff;
										
					//TIME TIME
					/*setNav[9]=bArray_Time[3] &0xff;
					setNav[8]=bArray_Time[2] &0xff;
					setNav[7]=bArray_Time[1] &0xff;
					setNav[6]=bArray_Time[0] &0xff;*/
					//ROS_INFO("setNav[9] %d   setNav[8] %d  setNav[7] %d  setNav[6] %d",setNav[9], setNav[8], setNav[7], setNav[6]);
					
					/*//TIME NAVIGATION SOLUTION
					setNav[9]=bArray_Time[3] &0xff;
					setNav[8]=bArray_Time[2] &0xff;
					setNav[7]=bArray_Time[1] &0xff;
					setNav[6]=bArray_Time[0] &0xff;
					
					//TIME NAVIGATION STATUS
					setNav[69]=bArray_Time[3] &0xff;
					setNav[68]=bArray_Time[2] &0xff;
					setNav[67]=bArray_Time[1] &0xff;
					setNav[66]=bArray_Time[0] &0xff;
					
					//TIME LLH
					setNav[93]=bArray_Time[3] &0xff;
					setNav[92]=bArray_Time[2] &0xff;
					setNav[91]=bArray_Time[1] &0xff;
					setNav[90]=bArray_Time[0] &0xff;
					
					//TIME VELOCITY NED
					setNav[129]=bArray_Time[3] &0xff;
					setNav[128]=bArray_Time[2] &0xff;
					setNav[127]=bArray_Time[1] &0xff;
					setNav[126]=bArray_Time[0] &0xff;*/
					
					//HEADING
					setNav[153]=bArray_Heading[3] &0xff;
                                        setNav[152]=bArray_Heading[2] &0xff;
                                        setNav[151]=bArray_Heading[1] &0xff;
                                        setNav[150]=bArray_Heading[0] &0xff; 

					
					setNav[101]=bArray_Lat[3] &0xff;
					setNav[100]=bArray_Lat[2] &0xff;
					setNav[99]=bArray_Lat[1] &0xff;
					setNav[98]=bArray_Lat[0] &0xff;
					//ROS_INFO("setNav[125] %d   setNav[124] %d  setNav[123] %d  setNav[122] %d",setNav[125], setNav[124], setNav[123], setNav[122]);
					//ROS_INFO("setNav[133] %d   setNav[132] %d  setNav[131] %d  setNav[130] %d",setNav[133], setNav[132], setNav[131], setNav[130]);
					setNav[97]=bArray_Lon[3] &0xff;
					setNav[96]=bArray_Lon[2] &0xff;
					setNav[95]=bArray_Lon[1] &0xff;
					setNav[94]=bArray_Lon[0] &0xff;
					setNav[105]=bArray_Alt[3] &0xff;
					setNav[104]=bArray_Alt[2] &0xff;
					setNav[103]=bArray_Alt[1] &0xff;
					setNav[102]=bArray_Alt[0] &0xff;
					setNav[109]=bArray_Alt[3] &0xff;
					setNav[108]=bArray_Alt[2] &0xff;
					setNav[107]=bArray_Alt[1] &0xff;
					setNav[106]=bArray_Alt[0] &0xff;
					/*check[23]=bArray_Alt[3] &0xff;
					check[22]=bArray_Alt[2] &0xff;
					check[21]=bArray_Alt[1] &0xff;
					check[20]=bArray_Alt[0] &0xff;
					check[19]=bArray_Alt[3] &0xff;
					check[18]=bArray_Alt[2] &0xff;
					check[17]=bArray_Alt[1] &0xff;
					check[16]=bArray_Alt[0] &0xff;
					check[15]=bArray_Lat[3] &0xff;
					check[14]=bArray_Lat[2] &0xff;
					check[13]=bArray_Lat[1] &0xff;
					check[12]=bArray_Lat[0] &0xff;
					check[11]=bArray_Lon[3] &0xff;
					check[10]=bArray_Lon[2] &0xff;
					check[9]=bArray_Lon[1] &0xff;
					check[8]=bArray_Lon[0] &0xff;*/
					
					//checksum LLH					
					for(count=86; count<118; count++){
					    CK_A = CK_A + setNav[count];
					    CK_B = CK_B + CK_A;
					  }
			  		sum1 = CK_A &0xff;
			  		sum2 = CK_B &0xff;
					setNav[118]=sum1;
					setNav[119]=sum2;
					
					//checksum for NAVIGATION SOLUTION	
					for(count=2; count<58; count++){
				 	       CK_C = CK_C + setNav[count];
					       CK_D = CK_D + CK_C;
					}
					sum3 = CK_C &0xff;
					sum4 = CK_D &0xff;
					setNav[58]=sum3;
					setNav[59]=sum4;

					//checksum for NAVIGATION STATUS
					for(count=62; count<82; count++){
					       CK_E = CK_E + setNav[count];
					       CK_F = CK_F + CK_E;
					}
					sum5 = CK_E &0xff;
					sum6 = CK_F &0xff;
					setNav[82]=sum5;
					setNav[83]=sum6;
					
					//checksum for VELOCITY NED
					for(count=122; count<162; count++){
					       CK_G = CK_G + setNav[count];
					       CK_H = CK_H + CK_G;
					}
					sum7 = CK_G &0xff;
					sum8 = CK_H &0xff;
					setNav[162]=sum7;
					setNav[163]=sum8;
						
					//checksum for TIME
					for(count=2; count<22; count++){
					       CK_I = CK_I + setNav[count];
					       CK_L = CK_L + CK_I;
					}
					sum9 = CK_I &0xff;
					sum10 = CK_L &0xff;
					setNav[22]=sum9;
					setNav[23]=sum10;
					
										
					//if (count_nav == 0) strncpy(setNavOld2, &setNav, sizeof(setNav)); 
					//if (count_nav == 1) memcpy(&setNavOld1, &setNav, sizeof(setNav));
					/*if (count_nav == 2){ 
						memcpy(&setNavOld, &setNav, sizeof(setNav));
						ubx_write(fd,setNavOld2);
					}
					if (count_nav == 3){
						memcpy(&setNavOld2, &setNavOld1, sizeof(setNavOld1));
						ubx_write(fd,setNavOld1);
					}
					if (count_nav == 4){
						memcpy(&setNavOld1, &setNavOld, sizeof(setNavOld));
						ubx_write(fd,setNavOld);
					}*/
					//ROS_INFO("COUNT = %d",count_nav);
					//usleep(500000);
					ubx_write(fd);
					CK_A = 0;
					CK_B = 0;
					CK_C = 0;
					CK_D = 0;
					CK_E = 0;
					CK_F = 0;
					CK_G = 0;
					CK_H = 0;
					CK_I = 0;
					CK_L = 0;
					/*ROS_INFO("ARRAY %d",bArray[0]);
					ROS_INFO("ARRAY %d",bArray[1]);
					ROS_INFO("ARRAY %d",bArray[2]);
					ROS_INFO("ARRAY %d",bArray[3]);*/
				}
			 }
			
		}while( numBytes > 0 );
		//count_nav=count_nav + 1;
                //if (count_nav == 5) count_nav = 2;
		optitrack_publisher_.publish(optitrack_position);
		r.sleep();
		ros::spinOnce();
		
		//ROS_INFO("END LOOP  %f", ros::Time::now().toSec());		
	}//while ros::ok
	// Process mocap data until SIGINT
	close_port(fd);
	return 0;
}//main
