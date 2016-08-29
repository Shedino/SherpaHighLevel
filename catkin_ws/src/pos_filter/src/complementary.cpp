#include "ros/ros.h"
#include "setup.h"
#include "complementary.h"

// constructor
Complementary::Complementary( double initPos, double initVel, double initBias ){
	dt = 1.0/FREQUENCY;

	pole = -3.0;          
	k1 = pole * (-3.0);
	k2 = pow(k1, 2.0) / 3.0;
	k3 = pow(pole, 3.0);

	state << initPos,
		 initVel,
		 initBias;
	dot_state <<  0.0,
			0.0,
			0.0;

	est_acc = 0.0;

	A << -k1, 1.0,  0.0,                                    		
	     -k2, 0.0, -1.0,						
	     -k3, 0.0,  0.0;						
	B << 	0.0,							// input signal matrix
		0.0,
		0.0;   
}

// destructor
Complementary::~Complementary( void ){
}

// member functions
void Complementary::update( double acc, double pos ){
	double hat_a = acc - state(eBIAS);
	double tilde_p = pos - state(ePOSITION);
#ifdef DEBUG_MSGS
	ROS_INFO("PREupdate: state: p:%.4f v:%.4f b:%.4f | acc_dif:%.4f", state(ePOSITION), state(eVELOCITY), state(eBIAS), acc/(100.0*100.0));
#endif
	//state_error(ePOSITION) = pos - state(ePOSITION);

	//state_error = A * state_error + B * acc;
	
	dot_state(ePOSITION) = state(eVELOCITY) + k1 * tilde_p;
	dot_state(eVELOCITY) = hat_a + k2 * tilde_p;
	dot_state(eBIAS) = k3 * tilde_p;
	//state = state + state_error * dt;

	state = state + dot_state * dt;
#ifdef DEBUG_MSGS
	ROS_INFO("POSTupdate: state: p:%.4f v:%.4f b:%.4f | pos:%.4f", state(ePOSITION), state(eVELOCITY), state(eBIAS), pos);
#endif
}

Vector3d Complementary::getState( void ){					// read the state of kalman filter
	return state;
}

double Complementary::getEstPos( void ){					// read estimated position of filter
	return state(ePOSITION);
}

double Complementary::getEstVel( void ){					// read estimated velocity of filter
	return state(eVELOCITY);
}

double Complementary::getEstBias( void ){					// read estimated velocity of filter
	return state(eBIAS);
}

// non member functions
Vector3d readEstimated3dPosComplementary( Complementary *comp_x, Complementary *comp_y, Complementary *comp_z ){
	Vector3d pos;

	pos(0) = comp_x->getEstPos();
	pos(1) = comp_y->getEstPos();
	pos(2) = comp_z->getEstPos();

	return pos;
}

Vector3d readEstimated3dVelComplementary( Complementary *comp_x, Complementary *comp_y, Complementary *comp_z ){
	Vector3d vel;

	vel(0) = comp_x->getEstVel();
	vel(1) = comp_y->getEstVel();
	vel(2) = comp_z->getEstVel();

	return vel;
}
