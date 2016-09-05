#include "ros/ros.h"
#include <vector>

#include "guidance_node_amsl/Directive.h"
#include "mavros/PositionTarget.h"
#include "mavros/Safety.h"
#include "geometry_msgs/Twist.h"
#include "mavros/Attitude.h"

#define alpha 0.0

class GuidanceNodeSpeedClass {
public:
	GuidanceNodeSpeedClass(ros::NodeHandle& node){

		n_=node;

		param_ = std::vector<double>(6);
		integral = std::vector<double>(3);	//x,y and yaw
		error = std::vector<double>(3);		//x,y and yaw

		//frequency
		rate = 10;
		dt = 1.0/rate;

		_directive.vxBody = 0;
		_directive.vyBody = 0;
		_directive.vzBody = 0; //Not used
		_directive.yawRate = 0;

		enable_directive = false;

		//control param
		node.param("guidance_node_speed_ned/param/sat_xy_speed", param_[0], 1.5);
		node.param("guidance_node_speed_ned/param/sat_yaw_speed", param_[1], 1.0);
		node.param("guidance_node_speed_ned/param/gain_xy_speed", param_[2], 0.01);
		node.param("guidance_node_speed_ned/param/gain_yaw_speed", param_[3], 0.01);
		node.param("guidance_node_speed_ned/param/gain_integral_xy_speed", param_[4],0.025);
		node.param("guidance_node_speed_ned/param/gain_integral_yaw_speed", param_[5],0.015);

		//subscribers and publishers
		//subCmdVel = n_.subscribe("/cmd_vel",10, &GuidanceNodeSpeedClass::readCmdVel, this);
		subCmdVel = n_.subscribe("/cmd_vel", 1, &GuidanceNodeSpeedClass::readCmdVel,this);
		subPosFilter = n_.subscribe("/pos_filter/pos_vel_out", 1, &GuidanceNodeSpeedClass::readPosFilter,this);
		subSafety = n_.subscribe("/safety_odroid", 1, &GuidanceNodeSpeedClass::readSafety,this);
		subAttitude = n_.subscribe("/attitude", 1, &GuidanceNodeSpeedClass::readAttitude,this);

		pubDirective = n_.advertise<guidance_node_amsl::Directive>("/directive", 10);

		ROS_INFO("Params: %f - %f - %f - %f", param_[0], param_[1], param_[2], param_[3]);

	}

	void loop(){
		//WatchDog
		if (counter_wd >= 10){
			enable_directive = false;
			if (counter_wd > 1000){
				counter_wd = 10;
			}
		} else {
			if (_pos_vel.type_mask != 1){	//IF SLAM is working fine type_mask == 0
				enable_directive = true;
			}
		}


		if (!enable_directive){
			_cmd_vel.linear.x = 0;
			_cmd_vel.linear.y = 0;
			_cmd_vel.angular.z = 0;
		}

		//CONTROLLER

		//Calculate Error
		error[0] = _cmd_vel.linear.x - _pos_vel.velocity.x;
		error[1] = _cmd_vel.linear.y - _pos_vel.velocity.y;
		//error[2] = _cmd_vel.linear.z - _pos_vel.velocity.z;
		error[2] = _cmd_vel.angular.z - _pos_vel.yaw_rate;

		//Calculate Integral and anti-wind up
		integral[0] = integral[0] + param_[4]*error[0]*dt;
		integral[0] = std::max(integral[0],-param_[0]/2);
		integral[0] = std::min(integral[0],param_[0]/2);
		integral[1] = integral[1] + param_[4]*error[1]*dt;
		integral[1] = std::max(integral[1],-param_[0]/2);
		integral[1] = std::min(integral[1],param_[0]/2);
		integral[2] = integral[2] + param_[5]*error[2]*dt;
		integral[2] = std::max(integral[2],-param_[1]/2);
		integral[2] = std::min(integral[2],param_[1]/2);

		//Proportional + I + FF
		_directive.vxBody = param_[2]*error[0] + integral[0] + _cmd_vel.linear.x;
		_directive.vyBody = param_[2]*error[1] + integral[1] + _cmd_vel.linear.y;
		_directive.yawRate = param_[3]*error[2] + integral[2] + _cmd_vel.angular.z;

		//Saturation
		_directive.vxBody = std::max((double)_directive.vxBody,-param_[0]);
		_directive.vxBody = std::min((double)_directive.vxBody,param_[0]);
		_directive.vyBody = std::max((double)_directive.vyBody,-param_[0]);
		_directive.vyBody = std::min((double)_directive.vyBody,param_[0]);
		_directive.yawRate = std::max((double)_directive.yawRate,-param_[1]);
		_directive.yawRate = std::min((double)_directive.yawRate,param_[1]);

		if (counter_print >= 2 && !_safety.safety){
			counter_print = 0;
			ROS_INFO("Guidance X CONTROL: %f, %f, %f, %f, %f", _cmd_vel.linear.x, _pos_vel.velocity.x, error[0], integral[0], _directive.vxBody);
			ROS_INFO("Guidance Y CONTROL: %f, %f, %f, %f, %f", _cmd_vel.linear.y, _pos_vel.velocity.y, error[1], integral[1], _directive.vyBody);
			ROS_INFO("Guidance Z ANGULAR CONTROL: %f, %f, %f, %f, %f", _cmd_vel.angular.z, _pos_vel.yaw_rate, error[2], integral[2], _directive.yawRate);
			//ROS_INFO("Guidance enableL: %s", enable_directive ? "true" : "false");
		}

		pubDirective.publish(_directive);
	}

	void readCmdVel(const geometry_msgs::Twist::ConstPtr& msg){
		//ROS_INFO("Received CMDVEL");
		counter_wd = 0;
		//_cmd_vel  = *msg;
		if (_safety.safety){		//If ODROID not enabled
			_cmd_vel.linear.x = 0;
			_cmd_vel.linear.y = 0;
			_cmd_vel.angular.z = 0;
		} else {
			//LOW PASS
			//TODO put it into main loop and not in callback to run it at the node frequency
			_cmd_vel.linear.x = _cmd_vel.linear.x*alpha + msg->linear.x*(1-alpha);
			_cmd_vel.linear.y = _cmd_vel.linear.y*alpha - msg->linear.y*(1-alpha);
			_cmd_vel.angular.z = _cmd_vel.angular.z*alpha - msg->angular.z*(1-alpha);
			//_cmd_vel.linear.y = -_cmd_vel.linear.y;		//CMD_VEL is in NWU (ROS) while controller is Body in NED
			//_cmd_vel.angular.z = -_cmd_vel.angular.z;	//CMD_VEL is in NWU (ROS) while controller is Body in NED
		}
	}

	void readSafety(const mavros::Safety::ConstPtr& msg){
		if (msg->safety){
			integral[0] = 0;
			integral[1] = 0;
			integral[2] = 0;
		}
		_safety = *msg;
	}

	void readAttitude(const mavros::Attitude::ConstPtr& msg){
		_attitude = *msg;
		//ROS_INFO("Yaw: %f",msg->yaw);
	}

	void readPosFilter(const mavros::PositionTarget::ConstPtr& msg){
		//ROS_INFO("Received POSFILTER");
		_pos_vel = *msg;
		double temp_x, temp_y;
		temp_x = _pos_vel.velocity.x;
		temp_y = _pos_vel.velocity.y;
		//ROTATING VELOCITIES FROM NED TO BODY
		_pos_vel.velocity.x = cos(_attitude.yaw)*temp_x + sin(_attitude.yaw)*temp_y;
		_pos_vel.velocity.y = -sin(_attitude.yaw)*temp_x + cos(_attitude.yaw)*temp_y;
		//ROS_INFO("Test: %f , %f , %f , %f", -sin(_attitude.yaw), cos(_attitude.yaw), temp_x, temp_y);
	}

	void run() {
		ros::Rate loop_rate(rate);
		while (ros::ok())
		{
			counter_wd++;
			counter_print++;
			ros::spinOnce();
			loop();
			loop_rate.sleep();
		}
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	ros::Subscriber subCmdVel;
	ros::Subscriber subPosFilter;
	ros::Subscriber subSafety;
	ros::Subscriber subAttitude;

	ros::Publisher pubDirective;

	int rate;
	double dt;

	std::vector<double> integral;
	std::vector<double> error;

	std::vector<double> param_;

	uint16_t counter_wd;
	uint16_t counter_print;

	bool enable_directive;

	geometry_msgs::Twist _cmd_vel;
	mavros::PositionTarget _pos_vel;
	guidance_node_amsl::Directive _directive;
	mavros::Safety _safety;
	mavros::Attitude _attitude;

private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "guidance_node_speed_ned");
	ros::NodeHandle node;

	GuidanceNodeSpeedClass GuidanceNodeSpeed(node);
	GuidanceNodeSpeed.run();
	return 0;
}
