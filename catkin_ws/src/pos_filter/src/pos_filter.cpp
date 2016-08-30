#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <Eigen/Dense>
#include "kalman.h"
#include "complementary.h"
#include "setup.h"

// mavros
#include "mavros/Raw_imu.h"
#include "mavros/Attitude.h"
#include "mavros/PositionTarget.h"

// msgs
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/Point.h"

#include "pos_filter/tf_msg.h"
#include "pos_filter/filter_state_msg.h"

#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>

// namespaces
using namespace Eigen;

//global variables
static int posUpdateFlag = 0,
    imuUpdateFlag = 0,
    attUpdateFlag = 0;
Matrix3d rotationMatrix = Matrix3d::Identity();
Vector3d imuVec3;
ros::Time timestamp;
double mapOffset = 0.0;
int tf_init_done = 0;
tf::Quaternion slam_orientation(0.0, 0.0, 0.0, 0.0);

// msg buffer
static mavros::Raw_imu data_raw;						// IMU raw data msg object
static mavros::Attitude attitude;						// attitude msg object
mavros::PositionTarget estimated_pos;					// estimated position msg object
static geometry_msgs::Pose slam_pos;					// SLAM position msg object
static geometry_msgs::PoseStamped filter_pose_visu;			// SLAM position msg object
static geometry_msgs::PoseWithCovarianceStamped resetPose;			// SLAM position used to reset
static pos_filter::tf_msg tf_msg;						// filter tf conversion node
static pos_filter::filter_state_msg state_msg;			// debug message (for matlab)
//static geometry_msgs::PoseWithCovarianceStamped glob_pos_msg;

//function prototypes
void laserCallback(const geometry_msgs::PoseStamped::ConstPtr& msg);
void imuCallback(const mavros::Raw_imu::ConstPtr& msg);
void pf_tfCallback(const pos_filter::tf_msg::ConstPtr& msg);
void attitudeCallback(const mavros::Attitude::ConstPtr& msg);
void compFilter(double *pos, double *vel, double *bias, double acc, double slamPos);
void kalmanFilter(double *pos, double *vel, double *bias, double acc, double slamPos);
mavros::PositionTarget convFilterToMavrosPositionTarget( Vector3d pos, Vector3d vel );
void NedToNwu(short int *x, short int *y, short int *z);
void NwuToNed(short int *x, short int *y, short int *z);
double normalize_angle_rad(double angle_rad);
//void globPosCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);

//main
int main(int argc, char **argv)
{
/*** ROS init ***/
  ros::init(argc, argv, "pos_filter");

  ros::NodeHandle n;

  ros::Rate loop_rate(FREQUENCY);					// frequency of loop (see setup.h)
  
// variables
  long int cycleCount = 0;
  static int slamOffset2BigCount = 0;
  double yaw = 0.0;
  double slamToFilterOffset = 0,
		 slamToFilterYawError = 0;
  static bool slamError = false;
  
// subscribers
  ros::Subscriber mavros_imu_sub = n.subscribe("/imu", 1, &imuCallback);
  ros::Subscriber laser_sub = n.subscribe("/slam_out_pose", 1, &laserCallback);
  ros::Subscriber attitude_sub = n.subscribe("/attitude", 1, &attitudeCallback);
  ros::Subscriber pos_filter_tf_sub = n.subscribe("/pos_filter/tf_msg", 1, &pf_tfCallback);
//  ros::Subscriber glob_pos_sub = n.subscribe("/mavros/global_position/local", 1, &globPosCallback);

// publishers
  ros::Publisher position_pub = n.advertise<mavros::PositionTarget>("/pos_filter/pos_vel_out", 1);
  ros::Publisher state_msg_pub = n.advertise<pos_filter::filter_state_msg>("/filter_state", 1);
  ros::Publisher filter_pose_visu_pub = n.advertise<geometry_msgs::PoseStamped>("/pos_filter/visu_pose", 1);
  ros::Publisher reset_slam_pub = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 1);

  ros::spinOnce();

  ros::Duration(0.1).sleep();

  while (!tf_init_done){	// wait for first attitude and SLAM msg to do proper rotations
    ROS_WARN_STREAM("pos_filter_tf_node not running properly!");
    ros::Duration(1).sleep();
    ros::spinOnce();
  }

  while (!(posUpdateFlag && imuUpdateFlag && attUpdateFlag)){
    ROS_INFO("waiting for slam running and first mavros (IMU, attitude) messages");
    ros::Duration(1).sleep();
    ros::spinOnce();
  }

// filter objects
#if defined(COMPLEMENTARY) || defined(FILTER_COMPARE)
  ROS_INFO("Complementary filter active");
  Complementary comp_x(slam_pos.position.x, 0.0, 0.0);
  Complementary comp_y(slam_pos.position.y, 0.0, 0.0);
  Complementary comp_z(slam_pos.position.z, 0.0, 0.0);
  ROS_INFO( "Filter initial position is x: %.4f,    y: %.4f,    z: %.4f", slam_pos.position.x, slam_pos.position.y, slam_pos.position.z );
#endif
#if defined(KALMAN) || defined(FILTER_COMPARE)
  ROS_INFO("Kalman filter active");
  Kalman kalman_x(slam_pos.position.x, 0.0, 0.0);
  Kalman kalman_y(slam_pos.position.y, 0.0, 0.0);
  Kalman kalman_z(slam_pos.position.z, 0.0, 0.0);
  ROS_INFO( "Filter initial position is x: %.4f,    y: %.4f,    z: %.4f", slam_pos.position.x, slam_pos.position.y, slam_pos.position.z );
#endif

#ifdef DEBUG_MSGS
  ROS_INFO( "DEBUG messages are active!" );	
#endif
#ifdef FILTER_COMPARE
  ROS_WARN_STREAM( "Both filters are activated for data logging!" );	
#endif

/*** inf loop ***/
  while (ros::ok())
  {
    timestamp = ros::Time::now();

    // update callbacks
	ros::spinOnce();

#ifdef USE_SLAM_YAW
    //read yaw from slam
	yaw = tf::getYaw(slam_orientation);
	// check fpr NaN error
    if (yaw != yaw){
	  // received NaN -> print warn message
      ROS_WARN_STREAM("Received NaN from SLAM orientation!");
	  // reset variable so that no calculations will become NaN
	  yaw = 0.0;
    }
#else
	yaw = -attitude.yaw;
	mapOffset = 0.0;
#endif
	
    // update rotation matrix
    rotationMatrix = AngleAxisd(normalize_angle_rad(-yaw + mapOffset), Vector3d::UnitZ())	// z axis rotation respecting map offset
                   * AngleAxisd(attitude.pitch, Vector3d::UnitY())                          // y axis rotateon
                   * AngleAxisd(attitude.roll, Vector3d::UnitX());                          // x axis rotation
#ifdef DEBUG_MSGS
    std::cout << rotationMatrix << std::endl << std::endl << std::endl;
#endif
	
    // save imu values in 3 dim vector for matrix calculations and recalciulate units from cm to m
	imuVec3 << 	(double)data_raw.xacc/100.0,
				(double)data_raw.yacc/100.0,
				(double)data_raw.zacc/100.0;

    // rotate IMU data
	imuVec3 = rotationMatrix * imuVec3;
    
	// convert from NED (FCU) to NWU (ROS) convention
	imuVec3(1) = -imuVec3(1);
	imuVec3(2) = -imuVec3(2);

#ifdef FILTER_COMPARE
    state_msg.slam_flag = posUpdateFlag;
    state_msg.imu_flag = imuUpdateFlag;
#endif

	// calculate yaw error
	slamToFilterYawError = abs(normalize_angle_rad((tf::getYaw(slam_orientation) - tf_msg.mapOffset) + attitude.yaw));
	//ROS_INFO("yaw error: %.4f, slam: %.4f, mapOff: %.4f, att: %.4f", slamToFilterYawError, tf::getYaw(slam_orientation), tf_msg.mapOffset, attitude.yaw);

#if defined(COMPLEMENTARY) || defined(FILTER_COMPARE)
/* Complementary filter */
	slamToFilterOffset = sqrt( pow(slam_pos.position.x -  comp_x.getEstPos(), 2) + 		// calculate offset between last filter and slam position
		pow(slam_pos.position.y - comp_y.getEstPos(), 2) );
	
	if( slamToFilterOffset >= SLAM_RECOVER_LIMIT 					// position error ...
		|| slamToFilterYawError >= M_PI_4 ){						// or yaw error
		comp_x.update(imuVec3(0), comp_x.getEstPos());				// --> perform filter update with last filter position instead of slam position
		comp_y.update(imuVec3(1), comp_y.getEstPos());
		slamOffset2BigCount++;
	}
	else{															// offset is smaller than 50 cm
		comp_x.update(imuVec3(0), slam_pos.position.x);				// --> perform normal filter update
		comp_y.update(imuVec3(1), slam_pos.position.y);
		slamOffset2BigCount = 0;
	}

    estimated_pos = convFilterToMavrosPositionTarget( readEstimated3dPosComplementary( &comp_x, &comp_y, &comp_z ), readEstimated3dVelComplementary( &comp_x, &comp_y, &comp_z ) );
#endif

#if defined(KALMAN) || defined(FILTER_COMPARE)
/* Kalman filter */
      kalman_x.prediction(imuVec3(0));								// --> perform prediction step
      kalman_y.prediction(imuVec3(1));
//      kalman_z.prediction(accVec3.getZ());
      imuUpdateFlag = 0;											// reset flag
#ifdef DEBUG_MSGS
      ROS_INFO("KALMAN predicted");
#endif
	  slamToFilterOffset = sqrt( pow(slam_pos.position.x -  kalman_x.getEstPos(), 2) + 		// calculate offset between last filter and slam position
		pow(slam_pos.position.y - kalman_y.getEstPos(), 2) );
	if( slamToFilterOffset >= SLAM_RECOVER_LIMIT 					// position error ...
		|| slamToFilterYawError >= M_PI_4 ){						// or yaw error
		slamOffset2BigCount++;
		// no update step
	  }
	  else{
        kalman_x.update(slam_pos.position.x);						// --> perform update step
        kalman_y.update(slam_pos.position.y);
//        kalman_z.update(HEIGHT_ESTIMATION FROM WASP);
		slamOffset2BigCount = 0;
	  }
      posUpdateFlag = 0;
#ifdef DEBUG_MSGS
      ROS_INFO( "KALMAN updated");
#endif
    estimated_pos = convFilterToMavrosPositionTarget( readEstimated3dPosKalman( &kalman_x, &kalman_y, &kalman_z ), readEstimated3dVelKalman( &kalman_x, &kalman_y, &kalman_z ) );
#endif
	
	// set missing variables in mavros message
    estimated_pos.header.stamp = timestamp;				// timestamp
    estimated_pos.header.seq = cycleCount;				// sequence
    estimated_pos.yaw = yaw - mapOffset;				// yaw
    estimated_pos.yaw_rate = attitude.yawspeed;				// yaw speed
	/************DELETE****/
/*	static int temp_pos = 0;
	if (cycleCount % 500 == 0)
		temp_pos = cycleCount*2;
	estimated_pos.position.x = (float)temp_pos;
	estimated_pos.position.y = (float)temp_pos;*/
	/************DELETE END****/
	if ( slamOffset2BigCount >= 50 )					// 100 = 1s, 200 = 2s, ...
		slamError = true;

	if ( !slamError )									// if filter and slam running fine...
		position_pub.publish(estimated_pos);			// send msg
	else {								// initiate reset to recover from failure
          resetPose.header.stamp = timestamp;
          resetPose.header.frame_id = "map";
          resetPose.pose.pose.position.x = estimated_pos.position.x;
          resetPose.pose.pose.position.y = -estimated_pos.position.y;
          resetPose.pose.pose.position.z = -estimated_pos.position.z;
          resetPose.pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(attitude.roll, -attitude.pitch, normalize_angle_rad(-attitude.yaw + tf_msg.mapOffset));

          reset_slam_pub.publish(resetPose);
          ROS_WARN_STREAM("SLAM error occured -> trying to recover!");
		  ROS_INFO( "Recover pos x: %.2f | y: %.2f | yaw: %.2f !", estimated_pos.position.x, -estimated_pos.position.y, normalize_angle_rad(-attitude.yaw + tf_msg.mapOffset));
          slamOffset2BigCount = 0;
          slamError = 0;
        }

    // print position estimation once every second
    if ( cycleCount % 100 == 0 ){
      ROS_INFO( "new pos estimate: x: %.4f,    y: %.4f,    z: %.4f,    off: %.4f", estimated_pos.position.x, -estimated_pos.position.y, -estimated_pos.position.z, slamToFilterOffset );
    }

/*************************************************/
#ifdef FILTER_COMPARE
    state_msg.timestamp = timestamp;
    state_msg.mapOffset = mapOffset;
    state_msg.comp_posX = comp_x.getEstPos();
    state_msg.comp_posY = comp_y.getEstPos();
    state_msg.comp_posZ = 0;//comp_z.getEstPos();
    state_msg.comp_velX = comp_x.getEstVel();
    state_msg.comp_velY = comp_y.getEstVel();
    state_msg.comp_velZ = 0;//comp_z.getEstVel();
    state_msg.comp_biasX = comp_x.getEstBias();
    state_msg.comp_biasY = comp_y.getEstBias();
    state_msg.comp_biasZ = 0;//comp_z.getEstBias();
    state_msg.kalman_posX = kalman_x.getEstPos();
    state_msg.kalman_posY = kalman_y.getEstPos();
    state_msg.kalman_posZ = 0;//kalman_z.getEstPos();
    state_msg.kalman_velX = kalman_x.getEstVel();
    state_msg.kalman_velY = kalman_y.getEstVel();
    state_msg.kalman_velZ = 0;//kalman_z.getEstVel();
    state_msg.kalman_biasX = kalman_x.getEstBias();
    state_msg.kalman_biasY = kalman_y.getEstBias();
    state_msg.kalman_biasZ = 0;//kalman_z.getEstBias();
    state_msg.slam_posX = slam_pos.position.x;
    state_msg.slam_posY = slam_pos.position.y;
    state_msg.slam_posZ = slam_pos.position.z;
    state_msg.att_roll = attitude.roll;
    state_msg.att_pitch = attitude.pitch;
	state_msg.imu_x = imuVec3(0);
    state_msg.imu_y = imuVec3(1);
    state_msg.imu_z = imuVec3(2);
	state_msg.imu_raw_x = data_raw.xacc;
    state_msg.imu_raw_y = data_raw.yacc;
    state_msg.imu_raw_z = data_raw.zacc;
    state_msg.slam_yaw = tf::getYaw(slam_orientation);
/*	state_msg.glob_loc_x = estimated_pos.position.x;
	state_msg.glob_loc_y = glob_pos_msg.pose.pose.position.y;*/

    state_msg_pub.publish(state_msg);				// send msg

    //reset flags
    state_msg.imu_flag = 0;
    state_msg.slam_flag = 0;
#endif

#ifdef RVIZ_VISU
		filter_pose_visu.header.stamp = timestamp;
		filter_pose_visu.header.frame_id = "map";
	#ifdef COMPLEMENTARY
		filter_pose_visu.pose.position.x = comp_x.getEstPos();
		filter_pose_visu.pose.position.y = comp_y.getEstPos();
		filter_pose_visu.pose.position.z = comp_z.getEstPos();
	#endif
	#ifdef KALMAN
		filter_pose_visu.pose.position.x = kalman_x.getEstPos();
		filter_pose_visu.pose.position.y = kalman_y.getEstPos();
		filter_pose_visu.pose.position.z = kalman_z.getEstPos();
	#endif
	filter_pose_visu.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(attitude.roll, -attitude.pitch,  yaw-mapOffset);
    filter_pose_visu_pub.publish(filter_pose_visu);				// send msg	
#endif
/*************************************************/  
    cycleCount++;

    loop_rate.sleep();
  }

  return 0;
}


void laserCallback(const geometry_msgs::PoseStamped::ConstPtr& msg){
  // read SLAM message
  slam_pos = msg->pose;

  // set orientation to quat
  tf::Quaternion temp_quat(slam_pos.orientation.x, slam_pos.orientation.y, slam_pos.orientation.z, slam_pos.orientation.w);
  slam_orientation = temp_quat;
  slam_orientation.normalize();

  posUpdateFlag = 1;

#ifdef DEBUG_MSGS
    ROS_INFO( "LASER CB: x: %.4f,    y: %.4f,    z: %.4f", slam_pos.position.x, slam_pos.position.y, slam_pos.position.z );	
#endif
}

void imuCallback(const mavros::Raw_imu::ConstPtr& msg){
  // read attitude
  data_raw = *msg;						// copy msg

  imuUpdateFlag = 1;
#ifdef DEBUG_MSGS
    ROS_INFO( "IMU CB: acc_x: %i,    acc_y: %i,    acc_z: %i\n", data_raw.xacc, data_raw.yacc, data_raw.zacc );	
#endif
}

void attitudeCallback(const mavros::Attitude::ConstPtr& msg){
  // read attitude
  attitude = *msg;

  attUpdateFlag = 1;
}

void pf_tfCallback(const pos_filter::tf_msg::ConstPtr& msg){
  // read tf message
  tf_msg = *msg;

  // if message indicates that tf is initialized
  if ( tf_msg.init_done ){
    // copy global map offset to local variable
    mapOffset = msg->mapOffset;
    // set flag that initialization is done
    tf_init_done = 1;
  }
}

/*void globPosCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  // read tf message
  glob_pos_msg = *msg;
}*/

/***** converting functions *****/
// convert 3 dim vector to stamped pose message format
mavros::PositionTarget convFilterToMavrosPositionTarget( Vector3d pos, Vector3d vel ){
	mavros::PositionTarget posTarget;
	
	// set position
	posTarget.position.x = pos(0);
	posTarget.position.y = -pos(1);
	posTarget.position.z = -pos(2);
	
	// set velocity
	posTarget.velocity.x = vel(0);
	posTarget.velocity.y = -vel(1);
	posTarget.velocity.z = -vel(2);

	return posTarget;
}

// convert x, y, z from North-East-Down to North-West-Up convention
void NedToNwu(short int *x, short int *y, short int *z){
	*y = -*y;
	*z = -*z;
}

// convert x, y, z from North-West-Up to North-East-Down convention
void NwuToNed(short int *x, short int *y, short int *z){
	*y = -*y;
	*z = -*z;
}

double normalize_angle_rad(double angle_rad){
	if (angle_rad >= M_PI){
		angle_rad -= 2 * M_PI;
	}
	if (angle_rad <= -M_PI){
		angle_rad += 2 * M_PI;
	}
	return angle_rad;
}
