#include "ros/ros.h"

#include <mavros/Global_position_int.h>

class test_PositionMSGClass{
  public:
    test_PositionMSGClass(ros::NodeHandle& node){

	ROS_INFO("[test_PositionMSG:] node started!");

	pubToPosition_=node.advertise<mavros::Global_position_int>("/global_position_int",10);

        outputPosition_.lat=445075000;
        outputPosition_.lon=113514000;
        outputPosition_.relative_alt=123;
        outputPosition_.hdg=123;
	ROS_INFO("[test_PositionMSG:] publishing on global_position_int.msg ...");
    }

    void run(){
      ros::Rate loopRate(1);
      while(ros::ok()){
	pubToPosition_.publish(outputPosition_);
        ROS_INFO("[test_PositionMSG:] new message sent!");
	loopRate.sleep();
      }
    }

  protected:
    ros::Publisher pubToPosition_;
    mavros::Global_position_int outputPosition_;
};

int main(int argc,char **argv){
	ros::init(argc,argv,"test_PositionMSG");
	ros::NodeHandle node;
	ROS_INFO("[test_PositionMSG:] initialising node ...");

	test_PositionMSGClass test_PositionMSG(node);
	test_PositionMSG.run();
	return 0;
}
