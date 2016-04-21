#include "ros/ros.h"
// #include <vector>

#include <mms_msgs/Cmd.h> // input & output
#include <mms_msgs/Ack_cmd.h>// output

class CmdVerifierNodeClass{
public:
	CmdVerifierNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromSentCmd_=n_.subscribe("/sent_command", 10, &CmdVerifierNodeClass::readCmdMessage,this);

		// publishers
		pubToAckCmd_=n_.advertise<mms_msgs::Ack_cmd>("/ack_cmd", 10);
		pubToVerifiedCmd_=n_.advertise<mms_msgs::Cmd>("/command", 10);
}

	void readCmdMessage(const mms_msgs::Cmd::ConstPtr& msg)
	{
		inputCmd_.command = msg -> command;
		inputCmd_.param1  = msg -> param1;
		inputCmd_.param2  = msg -> param2;
		inputCmd_.param3  = msg -> param3;
		inputCmd_.param4  = msg -> param4;
		inputCmd_.param5  = msg -> param5;
		inputCmd_.param6  = msg -> param6;
		inputCmd_.param7  = msg -> param7;
        inputCmd_.frame   = msg -> frame;
        inputCmd_.seq     = msg -> seq;

        ROS_INFO("CMD_VERIFIER: CMD_RECEIVED %d", inputCmd_.command);
        CmdVerifier_Handle();
	}

	void CmdVerifier_Handle()
	{
		switch(inputCmd_.command)
		{
		case 16:  // MAV_CMD_NAV_WAYPOINT
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_NAV_WAYPOINT RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		} break;
		case 21:  // MAV_CMD_NAV_LAND
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_NAV_LAND RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 22:  // MAV_CMD_NAV_TAKEOFF
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_NAV_TAKEOFF RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 179: // MAV_CMD_DO_SET_HOME
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_DO_SET_HOME RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 252: // MAV_CMD_OVERRIDE_GOTO
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_OVERRIDE_GOTO RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 300: // MAV_CMD_MISSION_START
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_MISSION_START RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 2000: // MAV_CMD_IMAGE_START_CAPTURE
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_IMAGE_START_CAPTURE RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 2500: // MAV_CMD_VIDEO_START_CAPTURE
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_VIDEO_START_CAPTURE RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		case 2501: // MAV_CMD_VIDEO_STOP_CAPTURE
		{
			ROS_INFO("CMD_VERIFIER: MAV_CMD_VIDEO_STOP_CAPTURE RECEIVED");
			outputAckCmd_.mav_command_accepted = true;
			outputAckCmd_.command = inputCmd_.command;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_ACCEPTED");
			outputVerifiedCmd_=inputCmd_;
			pubToVerifiedCmd_.publish(outputVerifiedCmd_);
			ROS_INFO("CMD_VERIFIER->verified_cmd: CMD_PUBLISHED");
		}break;
		default:
		{
			outputAckCmd_.mav_command_accepted = false;
			outputAckCmd_.command = 0;
			pubToAckCmd_.publish(outputAckCmd_);
			ROS_INFO("CMD_VERIFIER->ack_cmd: CMD_NOT_ACCEPTED");
		}
		}
	}

void run()
{
	// ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("CMD_VERIFIER: RUNNING");

		// CmdVerifier_Handle();
		// counter_++;
		ros::spinOnce();

		// loop_rate.sleep();
	}
}

protected:
/*state here*/
ros::NodeHandle n_;

// Subscribers
ros::Subscriber subFromSentCmd_;
mms_msgs::Cmd inputCmd_;

// Publishers
ros::Publisher pubToAckCmd_;
mms_msgs::Ack_cmd outputAckCmd_;

ros::Publisher pubToVerifiedCmd_;
mms_msgs::Cmd outputVerifiedCmd_;

// private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cmd_verifier");
	ros::NodeHandle node;

	CmdVerifierNodeClass CmdVerifierNode(node);

	CmdVerifierNode.run();
	return 0;
}
