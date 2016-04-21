#include "ros/ros.h"
#include "mavros/ArtvaRead.h"

class test_ARTVAClass{
  public:
    test_ARTVAClass(ros::NodeHandle& node){
	ROS_INFO("[test_ARTVA:] node started!");
	pubToArtva_=node.advertise<mavros::ArtvaRead>("/artva_read",10);
    }

    void run(){
      ros::Rate loopRate(1);
      while(ros::ok()){
        outputArtva_.rec1_distance=10;
        outputArtva_.rec1_direction=30;
        outputArtva_.rec2_distance=0;
        outputArtva_.rec2_direction=0;
        outputArtva_.rec3_distance=0;
        outputArtva_.rec3_direction=0;
        outputArtva_.rec4_distance=0;
        outputArtva_.rec4_direction=0;
		pubToArtva_.publish(outputArtva_);
        ROS_INFO("[test_ARTVA:] new message sent!");
		loopRate.sleep();
      }
    }

  protected:
    ros::Publisher pubToArtva_;
    mavros::ArtvaRead outputArtva_;
};


int main(int argc,char **argv){
	ros::init(argc,argv,"test_ARTVA");
	ros::NodeHandle node;
	ROS_INFO("[test_ARTVA:] initialising node ...");

	test_ARTVAClass test_ARTVA(node);
	test_ARTVA.run();
	return 0;
}
