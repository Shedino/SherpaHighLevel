#include "ros/ros.h"
#include "wasp_follow_human1/Operator.h"

class test_OperatorMSGClass{
  public:
    test_OperatorMSGClass(ros::NodeHandle& node){
	ROS_INFO("[test_OperatorMSG:] node started!");
	pubToOperator_=node.advertise<wasp_follow_human1::Operator>("/operator",10);
        outputOperator_.Latitude=445075000;
        outputOperator_.Longitude=113514000;
        outputOperator_.AltitudeRelative=0;
        outputOperator_.Yawangle=0;
        outputOperator_.Distance=10;
        outputOperator_.Direction=1;
	ROS_INFO("[test_OperatorMSG:] publishing on Operator.msg ...");
    }

    void run(){
      ros::Rate loopRate(1);
      while(ros::ok()){
	pubToOperator_.publish(outputOperator_);
        ROS_INFO("[test_OperatorMSG:] new message sent!");
	loopRate.sleep();
      }
    }

  protected:
    ros::Publisher pubToOperator_;
    wasp_follow_human1::Operator outputOperator_;
};

int main(int argc,char **argv){
	ros::init(argc,argv,"test_OperatorMSG");
	ros::NodeHandle node;
	ROS_INFO("[test_OperatorMSG:] initialising node ...");

	test_OperatorMSGClass test_OperatorMSG(node);
	test_OperatorMSG.run();
	return 0;
}
