#include <ros/ros.h>

#include <mms_msgs/Cmd.h>
#include <mms_msgs/Ack_cmd.h>
#include <mms_msgs/Ack_mission.h>
//#include <mavros/mavros.h>
#include "mloader/Mission.h"

// STATES DEFINITION
#define NO_MISSION_LOADED 10
#define LOADING_MISSION 20
#define MISSION_LOADED 30
#define SENDING_MISSION_ITEM 40
#define WAITING_FOR_MISSION_ACCEPTED 50
#define WAITING_FOR_MISSION_ITEM_REACHED 60

class MloaderNodeClass
{

public:
	MloaderNodeClass(ros::NodeHandle& node)
        {

		n_=node;

		//subscribers
		subFromMission_=n_.subscribe("/mission", 10, &MloaderNodeClass::readMission,this);
		subFromAckCmd_=n_.subscribe("/ack_cmd", 10, &MloaderNodeClass::readAckCmd,this);
		subFromAckMission_=n_.subscribe("/ack_mission", 10, &MloaderNodeClass::readAckCmd,this);
		//subFromCmd_ = n_.subscribe("/command", 10, &MmsNodeClass::readCmdMessage,this);

		// publishers
		pubToCmd_ = n_.advertise<mms_msgs::Cmd>("/command", 10);

		//Initializing outputCmd_
		outputCmd_.command = 95; // 95 = MAV_CMD_NAV_LAST = NOT OPERATIVE
		outputCmd_.param1 = 0;
		outputCmd_.param2 = 0;
		outputCmd_.param3 = 0;
		outputCmd_.param4 = 0;
		outputCmd_.param5 = 0;
		outputCmd_.param6 = 0;
		outputCmd_.param7 = 0;

		current_mission_item = 1;
		rate = 10;
		MISSION_ACCEPTED = false;
		MISSION_ITEM_REACHED = false;
		currentState = NO_MISSION_LOADED;
	}

	void readMission(const mloader::Mission::ConstPtr& msg)
	{
		ROS_INFO("MLOADER: MISSION_RECEIVED");

		inputMission_.mission_item_number = msg -> mission_item_number;
		inputMission_.command  = msg -> command;
		inputMission_.param1  = msg -> param1;
		inputMission_.param2 = msg -> param2;
		inputMission_.param3  = msg -> param3;
		inputMission_.param4  = msg -> param4;
		inputMission_.param5 = msg -> param5;
		inputMission_.param6  = msg -> param6;
		inputMission_.param7  = msg -> param7;

		ROS_INFO("MLOADER: %d MISSION ITEMS FOUND", inputMission_.mission_item_number);
		
	}

	void readAckCmd(const mms_msgs::Ack_cmd::ConstPtr& msg)
	{
		ROS_INFO("ACK_CMD_RECEIVED");

		inputAckCmd_.command = msg -> command;
		inputAckCmd_.mav_command_accepted  = msg -> mav_command_accepted;
	}

	void readAckMission(const mms_msgs::Ack_mission::ConstPtr& msg)
	{
		ROS_INFO("ACK_MISSION_RECEIVED");

		inputAckMission_.mission_item_reached = msg -> mission_item_reached;
		inputAckMission_.seq  = msg -> seq;
		inputAckMission_.mav_mission_accepted  = msg -> mav_mission_accepted;
	}

	void set_events_false()
	{
		// INPUTS GCS -> MLOADER
		// MISSION_COUNT = false;
		// MISSION_CLEAR_ALL = false;
		// MISSION_START = false;
		// INPUTS MMS -> MLOADER
		MISSION_ACCEPTED = false;
		MISSION_ITEM_REACHED = false;
	}

	void MLOADER_Handle()
	{
		switch(currentState)

		{

		case NO_MISSION_LOADED:
			set_events_false();

			if (inputMission_.mission_item_number >= 1)
			{
				currentState = SENDING_MISSION_ITEM;
				ROS_INFO("MLOADER_CURRENT_STATE: SENDING_MISSION_ITEM");
				current_mission_item  = 1;
			}
			break;

			//		case LOADING_MISSION:
			//set_events_false();
			//break;

		    //case MISSION_LOADED:
			//set_events_false();
			//break;

		case SENDING_MISSION_ITEM:
			set_events_false();
			if (current_mission_item <= inputMission_.mission_item_number)
			{
				ROS_INFO("MLOADER - CURRENT MISSION ITEM: %d", current_mission_item);

				// SENDING THE i-th MISSION ITEM

				outputCmd_.command = inputMission_.command[current_mission_item]; // 95 = MAV_CMD_NAV_LAST = NOT OPERATIVE
				outputCmd_.param1 = inputMission_.param1[current_mission_item];
				outputCmd_.param2 = inputMission_.param2[current_mission_item];
				outputCmd_.param3 = inputMission_.param3[current_mission_item];
				outputCmd_.param4 = inputMission_.param4[current_mission_item];
				outputCmd_.param5 = inputMission_.param5[current_mission_item];
				outputCmd_.param6 = inputMission_.param6[current_mission_item];
				outputCmd_.param7 = inputMission_.param7[current_mission_item];
				pubToCmd_.publish(outputCmd_);
				ROS_INFO("MLOADER->MMS: MISSION_ITEM_ID = %d", outputCmd_.command);

				currentState = WAITING_FOR_MISSION_ACCEPTED;
				ROS_INFO("MLOADER_CURRENT_STATE: WAITING_FOR_MISSION_ACCEPTED");

				current_mission_item  = current_mission_item+1;
			}
			else
			{
				currentState = NO_MISSION_LOADED;
				ROS_INFO("MLOADER_CURRENT_STATE: NO_MISSION_LOADED");
			}

			break;

		case WAITING_FOR_MISSION_ACCEPTED:
			set_events_false();
			if (MISSION_ACCEPTED)
			{
				outputCmd_.command = 300; // MAV_CMD_MISSION_START
				outputCmd_.param1 = 0;
				outputCmd_.param2 = 0;
				outputCmd_.param3 = 0;
				outputCmd_.param4 = 0;
				outputCmd_.param5 = 0;
				outputCmd_.param6 = 0;
				outputCmd_.param7 = 0;
				pubToCmd_.publish(outputCmd_);
				ROS_INFO("MLOADER->MMS: MAV_CMD_MISSION_START");

				currentState = WAITING_FOR_MISSION_ITEM_REACHED;
				ROS_INFO("MLOADER_CURRENT_STATE: WAITING_FOR_MISSION_ITEM_REACHED");
			}
			break;

		case WAITING_FOR_MISSION_ITEM_REACHED:
			set_events_false();
			if (MISSION_ITEM_REACHED)
			{
				currentState = SENDING_MISSION_ITEM;
				ROS_INFO("MLOADER_CURRENT_STATE: SENDING_MISSION_ITEM");
			}
			break;
		}

	}

	void run()
        {
		ros::Rate loop_rate(rate);

		while (ros::ok())
		{
			// ROS_INFO("MMS running");

			//MLOADER_Handle();
			ros::spinOnce();

			loop_rate.sleep();
		}
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	ros::Subscriber subFromAckCmd_;
	mms_msgs::Ack_cmd inputAckCmd_;

	ros::Subscriber subFromAckMission_;
	mms_msgs::Ack_mission inputAckMission_;

	ros::Subscriber subFromMission_;
	mloader::Mission inputMission_;

	ros::Publisher pubToCmd_;
	mms_msgs::Cmd outputCmd_;

	//ROS_INFO("MLOADER_CURRENT_STATE: NO_MISSION_LOADED");

	int current_mission_item;
	int rate;
	// INPUTS MMS -> MLOADER
	bool MISSION_ACCEPTED;
	bool MISSION_ITEM_REACHED;

	// STATE INITIALIZATION
	int currentState;

// private:
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mloader");
	ros::NodeHandle node;

	MloaderNodeClass mloaderNode(node);

	mloaderNode.run();
	return 0;
}
