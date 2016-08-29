#ifndef KALMAN_H
#define KALMAN_H

#include <Eigen/Dense>
#include "setup.h"

using namespace Eigen;

class Kalman{								// Kalman class
public:
	Kalman(double initPos, double initVel, double initBias); 	// constructor with value
	~Kalman( void ); 						// destructor

	void prediction( double acc ); 					// perform prediction step
	void update( double pos ); 					// perform update step
	Vector3d getState( void );					// read state of filter
	double getEstPos( void );					// read estimated position of filter
	double getEstVel( void );					// read estimated velocity of filter
	double getEstBias( void );					// read estimated bias of filter

private:
	double dt;                         				// time difference (100 Hz)
	Vector3d G;			                   		// 1 measurement to 3 state variables --> 3x1
	Matrix3d P; 		                  			// 3 state variables --> 3x3

	Matrix3d A;							// system matrix
	Vector3d B;							// input to state mapping matrix
	RowVector3d C;							// measurement to state mapping matrix
	//Vector1d D(0);                                       		// input to sensor mapping - not necessary right now

	Matrix3d Q;							// process noise variance
	MatrixXd R;                  					// measurement noise variance (only pos measurement --> 1x1)

	Vector3d state;							// state vector
};

// non member functions
Vector3d readEstimated3dPosKalman( Kalman *kalman_x, Kalman *kalman_y, Kalman *kalman_z );
Vector3d readEstimated3dVelKalman( Kalman *kalman_x, Kalman *kalman_y, Kalman *kalman_z );

#endif
