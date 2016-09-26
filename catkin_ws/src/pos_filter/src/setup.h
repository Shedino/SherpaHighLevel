#ifndef SETUP_H
#define SETUP_H

// defines
#define FREQUENCY 	100					// frequence of main loop in [Hz]
#define SLAM_RECOVER_LIMIT 1.0			// difference in filter estimated position and new SLAM measured position to perform recovery in [m]
//#define USE_SLAM_YAW					// use SLAM yaw instead of magnetometer yaw for rotation of IMU in NED frame to ROS map in ENU frame

#define LASER_IMU_OFFSET 0.12			// height offset laser to IMU (positiv if laser higher than IMU) in [m]
#define USE_NAVIGATION_STACK			// 

//#define FILTER_COMPARE				// uncomment for debugging of the two filters -> both filters run and topic /filter_state will be available
//#define DEBUG_MSGS					// print debug information in terminal

#define KALMAN							// use KALMAN filter (only one filter can be active at any time!)
//#define COMPLEMENTARY					// use COMPLEMENTARY filter (only one filter can be active at any time!)

#define RVIZ_VISU						// publish filter output position to PoseStamped for visualization in rviz

/* create error if both or no filters are active */
#if defined(KALMAN) && defined(COMPLEMENTARY)
  #error "only one filter my be active!"
#elif !defined(KALMAN) && !defined(COMPLEMENTARY)
  #error "at least one filter must be active!"
#endif



enum stateVariables{ePOSITION = 0, eVELOCITY = 1, eBIAS = 2};

#endif
