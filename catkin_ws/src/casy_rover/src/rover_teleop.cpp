#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include "mavros/OverrideRCIn.h"
#include "mavros/RCIn.h"
#include "mavros/State.h"
#include <ros/console.h>
#include <iostream>
int flag = 0;
namespace casy_rover {

class Teleop
{
private:
  ros::NodeHandle node_handle_;
  ros::Subscriber joy_subscriber_;
  ros::Subscriber state_subscriber_;
  ros::Publisher velocity_publisher_;
  mavros::OverrideRCIn velocity_;


  struct Axis
  {
    int axis;
    double max;
  };

  struct Button
  {
    int button;
  };

  struct {
    Axis x;
    Axis y;
    Axis z;
    Axis yaw;
  } axes_;

  struct
  {
    Button slow;
  } buttons_;

  double slow_factor_;

public:
  Teleop()
  {
    ros::NodeHandle params("~");

    axes_.x.axis = 990;
    axes_.x.max = 2000;
    axes_.y.axis = 990;
    axes_.y.max = 2000;
    axes_.z.axis = 990;
    axes_.z.max = 2000;
    axes_.yaw.axis = 0;
    axes_.yaw.max = 90.0*M_PI/180.0;
    buttons_.slow.button = 0;
    slow_factor_ = 0.2;

    params.getParam("x_axis", axes_.x.axis);
    params.getParam("y_axis", axes_.y.axis);
    params.getParam("z_axis", axes_.z.axis);
    params.getParam("yaw_axis", axes_.yaw.axis);
    params.getParam("x_velocity_max", axes_.x.max);
    params.getParam("y_velocity_max", axes_.y.max);
    params.getParam("z_velocity_max", axes_.z.max);
    params.getParam("yaw_velocity_max", axes_.yaw.max);
    params.getParam("slow_button", buttons_.slow.button);
    params.getParam("slow_factor", slow_factor_);
	
	velocity_publisher_ = node_handle_.advertise<mavros::OverrideRCIn>("mavros/rc/override", 10);
    state_subscriber_ = node_handle_.subscribe<mavros::State>("mavros/state", 1, boost::bind(&Teleop::stateCallback, this, _1));
  }

  ~Teleop()
  {
    stop();
  }

void stateCallback(const mavros::StateConstPtr& state)
  {
	std::string manual_mode="CMODE(0)";
	std::string auto_mode="CMODE(3)";
   	if (state->mode == manual_mode) {
     		 			ROS_INFO("MANUALE");
					velocity_.channels[0] = 0;
			     		velocity_.channels[1] = 0;
			     		velocity_.channels[2] = 0;
			     		velocity_.channels[3] = 0;
					velocity_.channels[4] = 0;
			     		velocity_.channels[5] = 0;
			     		velocity_.channels[6] = 0;
			     		velocity_.channels[7] = 0;
					velocity_publisher_.publish(velocity_);
			  	  } else if (state->mode == auto_mode && state->mode != manual_mode){
						ROS_INFO("STEERING");
						joy_subscriber_ = node_handle_.subscribe<sensor_msgs::Joy>("joy", 1, boost::bind(&Teleop::joyCallback, this, _1));
					}
  }


  void joyCallback(const sensor_msgs::JoyConstPtr& joy)
  {
	//steering
	int n1 = static_cast<int>(joy->axes[3] * 1000);
    	velocity_.channels[0] = 1500 + (n1/2);
	//Debug	
	//ROS_INFO("%d", n1);
	//ROS_INFO("%d", velocity_.channel[0]);
  	
	//throttle
	int n2 = static_cast<int>(joy->axes[1] * 1000);
	velocity_.channels[2] = 1500 + (n2/2);
	//Debug	
	//ROS_INFO("%d", n2);
	//ROS_INFO("%d", velocity_.channel[2]);

	//Buttons are'nt config yet
   	 if (getButton(joy, buttons_.slow.button)) {
     		 velocity_.channels[0] *= slow_factor_;
     		 velocity_.channels[1] *= slow_factor_;
     		 velocity_.channels[2] *= slow_factor_;
     		 velocity_.channels[3] *= slow_factor_;
  	  }
    	velocity_publisher_.publish(velocity_);
  }

  sensor_msgs::Joy::_axes_type::value_type getAxis(const sensor_msgs::JoyConstPtr& joy, int axis)
  {
    if (axis == 0) return 0;
    sensor_msgs::Joy::_axes_type::value_type sign = 1.0;
    if (axis < 0) { sign = -1.0; axis = axis; }
    if ((size_t)axis > joy->axes.size()* axes_.x.max) return 0;
    return sign * joy->axes[axis - 1];
  }

  sensor_msgs::Joy::_buttons_type::value_type getButton(const sensor_msgs::JoyConstPtr& joy, int button)
  {
    if (button <= 0) return 0;
    if ((size_t)button > joy->axes.size()) return 0;
    return joy->buttons[button - 1];
  }

  void stop()
  {
    velocity_ = mavros::OverrideRCIn();
    velocity_publisher_.publish(velocity_);
  }
};

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rover_teleop");

  casy_rover::Teleop teleop;
  ros::spin();

  return 0;
}
