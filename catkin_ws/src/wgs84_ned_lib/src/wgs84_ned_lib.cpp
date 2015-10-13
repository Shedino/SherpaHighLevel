#include "wgs84_ned_lib/wgs84_ned_lib.h"
#include <limits>
#include <cmath>
#include <math.h>
#include "ros/ros.h"

//using namespace std;
/* Define */
const double A = 6378137;    //major semiaxis
const double B = 6356752.3124;    //minor semiaxis

void get_pos_NED_from_WGS84 (double *x, double *y, double lat, double lon, double lat_home, double lon_home){
	//all lat and lon input are in degrees
	//x and y in meters
	//ROS_INFO("WGS LIB: converting WGS84 to NED");
	double e = sqrt(1-pow(B,2)/pow(A,2));           //TODO maybe can use define
	double lat_rad = lat*M_PI/180.0f;
	double lon_rad = lon*M_PI/180.0f;
	double lat_home_rad = lat_home*M_PI/180.0f;
	double lon_home_rad = lon_home*M_PI/180.0f;
	double radius = A/sqrt(1-pow(e,2)*pow(sin(lat_home_rad),2));
	*x = (lat_rad-lat_home_rad)*radius;
	*y = (lon_rad-lon_home_rad)*radius*cos(lat_home_rad);
	ROS_INFO("lat: %f - lon: %f - x: %f - y: %f", lat, lon, *x, *y);
}

void get_pos_WGS84_from_NED (double *lat, double *lon, double x, double y, double lat_home, double lon_home){
	//all lat and lon input are in degrees
	//x and y in meters
	//ROS_INFO("WGS LIB: converting WGS84 to NED");
	double e = sqrt(1-pow(B,2)/pow(A,2));
	double lat_home_rad = lat_home*M_PI/180.0f;
	double lon_home_rad = lon_home*M_PI/180.0f;
	double radius = A/sqrt(1-pow(e,2)*pow(sin(lat_home_rad),2));
	*lat = x/radius*180.0f/M_PI + lat_home;
	*lon = y/radius*180.0f/M_PI/cos(lat_home_rad) + lon_home;
	ROS_INFO("GET WGS: lat: %f - lon: %f - x: %f - y: %f", *lat, *lon, x, y);
}
