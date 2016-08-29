#include "ros/ros.h"

#include "geographic_msgs/GeoPose.h" //geopose

class DcmWaspBridgeClass {
public:
	DcmWaspBridgeClass(ros::NodeHandle& node){

		n_ = node;

		//subscribers
		subGeopose_ = n_.subscribe("geopose", 10, &DcmWaspBridgeClass::readGeopose,this); //subscribe to geopose
		

		// publishers
		//pubToAckMission_=n_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);

		//Init something
		rate = 10;
		counter_ = 0;
	}

	void readGeopose(const geographic_msgs::GeoPose::ConstPtr& msg){
		geopose_ = *msg;
	}

	void dcm_bridge_handle()
	{
		
	}

void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("WBRIDGE: RUNNING");

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
ros::Subscriber subGeopose_;
geographic_msgs::GeoPose geopose_;

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
