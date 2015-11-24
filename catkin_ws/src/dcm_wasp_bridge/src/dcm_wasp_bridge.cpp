#include "ros/ros.h"

#include "mms_msgs/Cmd.h" // input

class DcmWaspBridgeClass {
public:
	DcmWaspBridgeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		//subFromCmd_=n_.subscribe("/sent_command", 10, &DcmWaspBridgeClass::readCmdMessage,this); //subscribe to "sent_command" to exclude the "cmd_verifier" node
		
		// publishers
		//pubToAckMission_=n_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);

		//Init something
		rate = 10;
		counter_ = 0;
	}

	/*void readGridAckMessage(const mms_msgs::Grid_ack::ConstPtr& msg){
		Grid_ack_.grid_completed = msg->grid_completed;
		Grid_ack_.completion_type = msg->completion_type;
		if (Grid_ack_.grid_completed) GRID_ENDED = true;
		else GRID_ENDED = false;
	}*/

	void dcm_bridge_handle()
	{
		
	}

void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("MMS: RUNNING");

		dcm_bridge_handle();
		counter_++;
		ros::spinOnce();

		loop_rate.sleep();
	}
}

protected:

uint16_t counter_;
int rate;
ros::NodeHandle n_;

private:


};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "dcm_wasp_bridge");
	ros::NodeHandle node;

	DcmWaspBridgeClass DcmBridgeNode(node);

	DcmBridgeNode.run();
	return 0;
}
