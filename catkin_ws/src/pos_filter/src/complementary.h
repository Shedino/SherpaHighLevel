#include "setup.h"
#include <Eigen/Dense>

using namespace Eigen;

// class definition
class Complementary{
public:
	Complementary(double initPos, double initVel, double initBias); // constructor with value
	~Complementary( void ); 					// destructor

	Vector3d getState( void );					// read state of filter
	double getEstPos( void );					// read estimated position of filter
	double getEstVel( void );					// read estimated velocity of filter
	double getEstBias( void );					// read estimated bias of filter

	void update( double acc, double pos );				// update filter

private:
	double dt;
	double pole;
	double k1, k2, k3;

	Matrix3d A;							// system matrix
	Vector3d B;							// input to state mapping matrix

	Vector3d state, state_error, dot_state;
	double est_acc;
};


// non member functions
Vector3d readEstimated3dPosComplementary( Complementary *comp_x, Complementary *comp_y, Complementary *comp_z );
Vector3d readEstimated3dVelComplementary( Complementary *comp_x, Complementary *comp_y, Complementary *comp_z );
