#include "ros/ros.h"


class SwmInterfaceClass {
public:
	SwmInterfaceClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		//subFromCmd_=n_.subscribe("sent_command", 10, &SwmInterfaceClass::readCmdMessage,this); //subscribe to "sent_command" to exclude the "cmd_verifier" node
		
		// publishers
		//pubToAckMission_=n_.advertise<mms_msgs::Ack_mission>("ack_mission", 10);
		
		//INIT
	}

	/*void readCmdMessage(const mms_msgs::Cmd::ConstPtr& msg)
	{

	}*/


	void loop()
	{
		//DO WE NEED IT??
	}

	void run() {
		ros::Rate loop_rate(rate);

		while (ros::ok())
		{
			ROS_INFO_ONCE("MMS: RUNNING");

			loop();
			ros::spinOnce();

			loop_rate.sleep();
		}
	}

protected:
/*state here*/
ros::NodeHandle n_;

// Subscribers

//ros::Subscriber subFromCmd_;
//mms_msgs::Cmd inputCmd_;

// Publishers
//ros::Publisher pubToAckMission_;
//mms_msgs::Ack_mission outputAckMission_;

int rate;

private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "swm_interface_cpp");
	ros::NodeHandle node;

	SwmInterfaceClass swm_interfaceNode(node);

	swm_interfaceNode.run();
	return 0;
}
