#include "ros/ros.h"
#include "setup.h"
#include "kalman.h"
#include <iostream>
#include <math.h>       /* pow */

using namespace Eigen;

// constructor
Kalman::Kalman( double initPos, double initVel, double initBias ){
	dt = 1.0/FREQUENCY;

	state << initPos, initVel, initBias;

	A << 1, dt, 0,                                    		// P(k) = P(k-1) + V(k-1)*dt
	     0, 1, -dt,							// V(k) = V(k-1) - B(k-1)*dt
	     0, 0,  1;							// B(k) = B(k-1)
	B << 0,									// input signal matrix
	     dt,
	     0;                       

	C << 1, 0, 0;                                 			// sensor mapping matrix (only p is measured)
	//D << ();

	Q << 	0.001,	0,		0,
			0,  	0.01,	0,
			0, 		0,		0.05;
	R.resize(1,1);
	R << 0.05;								// position measurement variance

	G = Vector3d::Zero();
	P = Matrix3d::Zero();
}

// destructor
Kalman::~Kalman( void ){
}

// member functions
void Kalman::prediction( double acc ){ 					// do prediction step
#ifdef DEBUG_MSGS
	ROS_INFO("PREprediction: state: p:%.6f v:%.6f b:%.6f | acc_dif:%.6f", state(ePOSITION), state(eVELOCITY), state(eBIAS), acc/(100.0*100.0));
#endif
	state = A * state + B * acc;                          	// predict next state ( /100 for cm -> m conversion )
	P = A * P * A.transpose() + Q;                  		// update covariance matrix of estimation
#ifdef DEBUG_MSGS
	ROS_INFO("POSTprediction: state: p:%.6f v:%.6f b:%.6f", state(ePOSITION), state(eVELOCITY), state(eBIAS));
#endif
}

void Kalman::update( double position ){ 				// do update step
#ifdef DEBUG_MSGS
	ROS_INFO("PREupdate: state: p:%.6f v:%.6f b:%.6f | pos:%.6f", state(ePOSITION), state(eVELOCITY), state(eBIAS), position);
#endif
	MatrixXd doInv(1,1);
	doInv = C * P * C.transpose() + R;				// update Kalman gain
	G = P * C.transpose() * doInv.inverse();       			// ...
	state = state + G * (position - C * state);                  	// update system state
	P = ( Matrix3d::Identity() - G * C ) * P;       		// update covariance
#ifdef DEBUG_MSGS
	ROS_INFO("POSTupdate: state: p:%.6f v:%.6f b:%.6f", state(ePOSITION), state(eVELOCITY), state(eBIAS));
#endif
}

Vector3d Kalman::getState( void ){					// read the state of kalman filter
	return state;
}

double Kalman::getEstPos( void ){					// read estimated position of filter
	return state(ePOSITION);
}

double Kalman::getEstVel( void ){					// read estimated velocity of filter
	return state(eVELOCITY);
}

double Kalman::getEstBias( void ){					// read estimated velocity of filter
	return state(eBIAS);
}


// non member functions
Vector3d readEstimated3dPosKalman( Kalman *kalman_x, Kalman *kalman_y, Kalman *kalman_z ){
	Vector3d pos;

	pos(0) = kalman_x->getEstPos();
	pos(1) = kalman_y->getEstPos();
	pos(2) = kalman_z->getEstPos();

	return pos;
}

Vector3d readEstimated3dVelKalman( Kalman *kalman_x, Kalman *kalman_y, Kalman *kalman_z ){
	Vector3d vel;

	vel(0) = kalman_x->getEstVel();
	vel(1) = kalman_y->getEstVel();
	vel(2) = kalman_z->getEstVel();

	return vel;
}
