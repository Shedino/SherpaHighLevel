/*
 * Socket.cpp
 *
 *  Created on: 02/02/2015
 *      Author:
 */

#include "socket.h"
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include <ros/ros.h>

//socket semplice udp in ascolto su tutti gli indirizzi ip e alla porta 1511

int udp_socket;
sockaddr_in remote_addr;
int addr_len = sizeof(struct sockaddr);

UdpSocket::UdpSocket( const int local_port){
	struct sockaddr_in clientaddr;
	// Create a UDP socket
	ROS_INFO( "Creating socket..." );
	udp_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if( udp_socket < 0 ) throw SocketException( strerror( errno ) );
	// Allow reuse of local addresses
	ROS_INFO( "Setting socket options..." );
	int option_value = 1;
	int result = setsockopt( udp_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&option_value, sizeof( int ) );
	if( result == -1 ){
		std::stringstream error;
		error << "Failed to set socket option: ";
		switch( errno ){
			case EBADF:
			error << "EBADF";
			break;
			case EFAULT:
			error << "EFAULT";
			break;
			case EINVAL:
			error << "EINVAL";
			break;
			case ENOPROTOOPT:
			error << "ENOPROTOOPT";
			break;
			case ENOTSOCK:
			error << "ENOTSOCK";
			break;
			default:
			error << "unknown error";
			break;
		}
		throw SocketException( error.str().c_str() );    
	}
	// Fill struct for local address
	memset ( &clientaddr, 0, sizeof ( clientaddr ) );
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = htonl( INADDR_ANY );
	clientaddr.sin_port = htons( local_port );
	ROS_INFO( "Local address: %s:%i", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );
	// Bind the socket
	ROS_INFO( "Binding socket to local address..." );
	result = bind( udp_socket, (sockaddr*)&clientaddr, sizeof( clientaddr ) );
	if( result == -1 ){
		std::stringstream error;
		error << "Failed to bind socket to local address:" << strerror( errno );
		throw SocketException( error.str().c_str() );
	}
	// Make socket non-blocking
	ROS_INFO( "Enabling non-blocking I/O" );
	int flags = fcntl( udp_socket, F_GETFL , 0 );
	result = fcntl(udp_socket, F_SETFL, flags | O_NONBLOCK);
	if( result == -1 )  {
		std::stringstream error;
		error << "Failed to enable non-blocking I/O: " << strerror( errno );
		throw SocketException( error.str().c_str() );
	}     
}

UdpSocket::~UdpSocket(){
	close( udp_socket );
}

int UdpSocket::recv(){
	memset ( buf, 0, MAXRECV + 1 );
	int status = recvfrom(udp_socket,buf,MAXRECV,0,(sockaddr *)&remote_addr,(socklen_t*)&addr_len);
	 //if( status > 0 ) ROS_INFO("%s", buf);
	/*ROS_INFO( "%4i bytes received from %s:%i", status, inet_ntoa( remote_addr.sin_addr ), ntohs( remote_addr.sin_port ) );
	else  if( status == 0 )
	ROS_INFO( "Connection closed by peer" ); */
	return status;
}

int UdpSocket::send(char *buff){
	//memset ( buff, 0, MAXRECV + 1 );
	//ROS_INFO("%s", buff);
	//int addr_len = sizeof(struct sockaddr);
	int status = sendto(udp_socket, buff, strlen(buff), 0, (sockaddr *)&remote_addr, addr_len);
	/*if( status > 0 )
	ROS_INFO( "IM SENDING");
	else ROS_INFO( "IM NOT SENDING");*/
	return status;
}
