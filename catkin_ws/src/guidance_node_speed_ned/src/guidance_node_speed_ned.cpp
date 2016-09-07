#include "ros/ros.h"
#include <vector>

#include "guidance_node_amsl/Directive.h"
#include "mavros/PositionTarget.h"
#include "mavros/Safety.h"
#include "geometry_msgs/Twist.h"
#include "mavros/Attitude.h"
#include <tf/transform_broadcaster.h>
#include "nav_msgs/Odometry.h"


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

		_cmd_vel_pose.pose.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);

		enable_directive = false;

		//control param
		node.param("guidance_node_speed_ned/param/sat_xy_speed", param_[0], 1.5);
		node.param("guidance_node_speed_ned/param/sat_yaw_speed", param_[1], 1.0);
		node.param("guidance_node_speed_ned/param/gain_xy", param_[2], 0.2);
		node.param("guidance_node_speed_ned/param/gain_yaw", param_[3], 0.07);
		node.param("guidance_node_speed_ned/param/gain_integral_xy", param_[4],0.01);
		node.param("guidance_node_speed_ned/param/gain_integral_yaw", param_[5],0.01);

		//subscribers and publishers
		//subCmdVel = n_.subscribe("/cmd_vel",10, &GuidanceNodeSpeedClass::readCmdVel, this);
		subCmdVel = n_.subscribe("/cmd_vel_pose", 1, &GuidanceNodeSpeedClass::readCmdVelPose,this);
		//subCmdPos = n_.subscribe("/cmd_pose", 1, &GuidanceNodeSpeedClass::readCmdPose,this);
		subPosFilter = n_.subscribe("/odom", 1, &GuidanceNodeSpeedClass::readOdom,this);
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
		}


		if (!enable_directive){
			_cmd_vel_pose.twist.twist.linear.x = 0;
			_cmd_vel_pose.twist.twist.linear.y = 0;
			_cmd_vel_pose.twist.twist.linear.z = 0;
			_cmd_vel_pose.twist.twist.angular.x = 0;
			_cmd_vel_pose.twist.twist.angular.y = 0;
			_cmd_vel_pose.twist.twist.angular.z = 0;
		}

		//CONTROLLER

		//Calculate Error in NWU(ROS) frame
		error[0] = _cmd_vel_pose.pose.pose.position.x - _odom.pose.pose.position.x;
		error[1] = _cmd_vel_pose.pose.pose.position.y - _odom.pose.pose.position.y;
		//tf::Quaternion temp_quat(_cmd_vel_pose.pose.pose.orientation.x, _cmd_vel_pose.pose.pose.orientation.y,_cmd_vel_pose.pose.pose.orientation.z,_cmd_vel_pose.pose.pose.orientation.w);
		//temp_quat.normalize();
		error[2] = tf::getYaw(_cmd_vel_pose.pose.pose.orientation) - tf::getYaw(_odom.pose.pose.orientation);

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
		_cmd_vel_ned.linear.x = param_[2]*error[0] + integral[0] + _cmd_vel_pose.twist.twist.linear.x;
		_cmd_vel_ned.linear.y = param_[2]*error[1] + integral[1] + _cmd_vel_pose.twist.twist.linear.y;
		_cmd_vel_ned.angular.z = param_[3]*error[2] + integral[2] + _cmd_vel_pose.twist.twist.angular.z;

		//Convert NWU(ROS) --> NED
		_cmd_vel_ned.linear.y = -_cmd_vel_ned.linear.y;
		_cmd_vel_ned.angular.z = -_cmd_vel_ned.angular.z;

		//Convert NED-->BODY
		_directive.vxBody = cos(_attitude.yaw)*_cmd_vel_ned.linear.x + sin(_attitude.yaw)*_cmd_vel_ned.linear.y;
		_directive.vyBody = -sin(_attitude.yaw)*_cmd_vel_ned.linear.x + cos(_attitude.yaw)*_cmd_vel_ned.linear.y;
		_directive.yawRate = _cmd_vel_ned.angular.z;

		//Saturation
		_directive.vxBody = std::max((double)_directive.vxBody,-param_[0]);
		_directive.vxBody = std::min((double)_directive.vxBody,param_[0]);
		_directive.vyBody = std::max((double)_directive.vyBody,-param_[0]);
		_directive.vyBody = std::min((double)_directive.vyBody,param_[0]);
		_directive.yawRate = std::max((double)_directive.yawRate,-param_[1]);
		_directive.yawRate = std::min((double)_directive.yawRate,param_[1]);

		if (counter_print >= 2 && !_safety.safety){
			counter_print = 0;
			ROS_INFO("Guidance X CONTROL: %f, %f, %f", error[0], integral[0], _directive.vxBody);
			ROS_INFO("Guidance Y CONTROL: %f, %f, %f", error[1], integral[1], _directive.vyBody);
			ROS_INFO("Guidance Z ANGULAR CONTROL: %f, %f, %f", error[2], integral[2], _directive.yawRate);
			ROS_INFO("Guidance Z ANGULAR 2: %f, %f", tf::getYaw(_cmd_vel_pose.pose.pose.orientation), tf::getYaw(_odom.pose.pose.orientation));
			//ROS_INFO("Guidance enableL: %s", enable_directive ? "true" : "false");
		}

		pubDirective.publish(_directive);
	}

	void readCmdVelPose(const nav_msgs::Odometry::ConstPtr& msg){
		//ROS_INFO("Received CMDVEL");
		counter_wd = 0;
		if (_safety.safety){		//If ODROID not enabled
			_cmd_vel_pose.twist.twist.linear.x = 0;
			_cmd_vel_pose.twist.twist.linear.y = 0;
			_cmd_vel_pose.twist.twist.linear.z = 0;
			_cmd_vel_pose.twist.twist.angular.x = 0;
			_cmd_vel_pose.twist.twist.angular.y = 0;
			_cmd_vel_pose.twist.twist.angular.z = 0;
		} else {
			_cmd_vel_pose = *msg;
			//NWU (ROS) to NED
			//TODO put it into main loop and not in callback to run it at the node frequency
			//_cmd_vel.linear.x = _cmd_vel.linear.x*alpha + msg->linear.x*(1-alpha);
			//_cmd_vel.linear.y = _cmd_vel.linear.y*alpha - msg->linear.y*(1-alpha);
			//_cmd_vel.angular.z = _cmd_vel.angular.z*alpha - msg->angular.z*(1-alpha);
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

	void readOdom(const nav_msgs::Odometry::ConstPtr& msg){
		//ROS_INFO("Received POSFILTER");
		_odom = *msg;
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
	ros::Subscriber subCmdPos;

	ros::Publisher pubDirective;

	int rate;
	double dt;

	std::vector<double> integral;
	std::vector<double> error;

	std::vector<double> param_;

	uint16_t counter_wd;
	uint16_t counter_print;

	bool enable_directive;

	geometry_msgs::Twist _cmd_vel_ned;
	nav_msgs::Odometry _odom;
	nav_msgs::Odometry _cmd_vel_pose;
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
