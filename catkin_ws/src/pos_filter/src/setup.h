#ifndef SETUP_H
#define SETUP_H

// defines
#define FREQUENCY 	100					// frequence of main loop in [Hz]
#define SLAM_RECOVER_LIMIT 1.0
#define SLAM_ERROR_LIMIT 10.0
//#define USE_SLAM_YAW

#define LASER_IMU_OFFSET 0.12			// height offset laser to IMU (positiv if laser higher than IMU) in [m]
#define USE_NAVIGATION_STACK

#define FILTER_COMPARE
#define KALMAN							// use enum filterType to specify which filter to use
//#define COMPLEMENTARY
#define RVIZ_VISU
#if defined(KALMAN) && defined(COMPLEMENTARY)
  #error "only one filter my be active!"
#elif !defined(KALMAN) && !defined(COMPLEMENTARY)
  #error "at least one filter must be active!"
#endif


//#define DEBUG_MSGS						// comment in to get debug information through ROS_INFO

enum stateVariables{ePOSITION = 0, eVELOCITY = 1, eBIAS = 2};

#endif
