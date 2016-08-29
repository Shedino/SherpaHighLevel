#ifndef SETUP_H
#define SETUP_H

// defines
#define FREQUENCY 	100					// frequence of main loop in [Hz]
#define SLAM_ERROR_LIMIT 10.0
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
