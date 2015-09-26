#include "ros/ros.h"

#include "guidance_node_amsl/Directive.h"
#include "guidance_node_amsl/Reference.h"
#include "mms_msgs/MMS_status.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mavros/Global_position_int.h"
#include "mavros/Safety.h"
#include "mavros/Sonar.h"
#include "mms_msgs/Sys_status.h"


// STATES DEFINITION
#define ON_GROUND_NO_HOME 10
#define SETTING_HOME 20
#define ON_GROUND_DISARMED 30
#define ARMING 40
#define DISARMING 45
#define ON_GROUND_ARMED 50
#define ON_GROUND_READY_TO_TAKEOFF 60
#define PERFORMING_TAKEOFF 70
#define IN_FLIGHT 80
#define READY_TO_GO 90
#define PERFORMING_GO_TO 100
#define READY_TO_LAND 110
#define PERFORMING_LANDING 120

double PI = 3.1416; // pi

class UavSimNodeClass {
public:
	UavSimNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromDirective_=n_.subscribe("/directive", 10, &UavSimNodeClass::readDirective,this);
		subFromReference_=n_.subscribe("/reference", 10, &UavSimNodeClass::readReference,this);
		subFromState_=n_.subscribe("/mms_status", 10, &UavSimNodeClass::readState,this);
		subFromPosNav_=n_.subscribe("/position_nav", 10, &UavSimNodeClass::readPosNav,this);
		
		// publishers
		pubToSafety_=n_.advertise<mavros::Safety>("/safety",10);
		pubToSystStatus_=n_.advertise<mms_msgs::Sys_status>("/system_status",10);
		pubToGlobPosInt_=n_.advertise<mavros::Global_position_int>("/global_position_int",10);
		pubToSonar_=n_.advertise<mavros::Sonar>("/sonar",10);
	
	}

	


	void readDirective(const guidance_node_amsl::Directive::ConstPtr& msg)
	{
		directive_.vxBody = msg->vxBody;
		directive_.vyBody = msg->vyBody;
		directive_.vzBody = msg->vzBody;
		directive_.yawRate =  msg->yawRate;
	}




	void readReference(const guidance_node_amsl::Reference::ConstPtr& msg)
	{
		reference_.Latitude = msg->Latitude;
		reference_.Longitude = msg->Longitude;
		reference_.AltitudeRelative = msg->AltitudeRelative;
		reference_.Yawangle = msg->Yawangle;
		reference_.Mode = msg->Mode;
		reference_.frame = msg->frame;
	}


	void readState(const mms_msgs::MMS_status::ConstPtr& msg)
	{
		inputMmsStatus_.mms_state=msg->mms_state;
		inputMmsStatus_.target_ref_frame=msg->target_ref_frame;
	}



	void readPosNav(const guidance_node_amsl::Position_nav::ConstPtr& msg)
	{
		inputPos_.Latitude = msg->Latitude;
		inputPos_.Longitude = msg->Longitude;
		inputPos_.Altitude = msg->Altitude;
		inputPos_.Timestamp = msg->Timestamp;
		inputPos_.YawAngle = msg->YawAngle;
		inputPos_.frame = msg->frame;
	}


	void loop_handle()
	{
		pubToSafety_.publish(safety_);
		pubToSystStatus_.publish(sys_status_);
		pubToSonar_.publish(sonar_);

		switch(inputMmsStatus_.mms_state)
		{
			case ON_GROUND_NO_HOME:
				safety_.safety = false;                              //HARDCODED INITIALIAZED
				sonar_.distance = 5000;
				sys_status_.armed = false;             //armed
				sys_status_.voltage_battery = 15000;  //15 V
				sys_status_.valid_ref_frame = 11;     //baro
				globPosInt_.time_boot_ms = 0;
				globPosInt_.lat = 0;
				globPosInt_.lon = 0;
				globPosInt_.alt = 0;
				globPosInt_.relative_alt = 0;
				globPosInt_.vx = 0;
				globPosInt_.vy = 0;
				globPosInt_.vz = 0;
				globPosInt_.hdg = 0;
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case SETTING_HOME:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case ON_GROUND_DISARMED:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case ARMING:
				sys_status_.armed = true;             //armed
				pubToSystStatus_.publish(sys_status_);
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case DISARMING:
				sys_status_.armed = false;             //armed
				pubToSystStatus_.publish(sys_status_);
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case ON_GROUND_ARMED:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case ON_GROUND_READY_TO_TAKEOFF:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case PERFORMING_TAKEOFF:
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;   
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case IN_FLIGHT:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case READY_TO_GO:
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case PERFORMING_GO_TO:
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;   
				globPosInt_.lat += (reference_.Latitude - inputPos_.Latitude)/70;
				globPosInt_.lon += (reference_.Longitude - inputPos_.Longitude)/70;
				pubToGlobPosInt_.publish(globPosInt_);
				break;

			case READY_TO_LAND:
				pubToGlobPosInt_.publish(globPosInt_);
				break;
	

			case PERFORMING_LANDING:
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;     
				pubToGlobPosInt_.publish(globPosInt_);
				break;
		}

}




void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("REF: RUNNING");

		loop_handle();
		ros::spinOnce();

		loop_rate.sleep();
	}
}

protected:
/*state here*/
ros::NodeHandle n_;

ros::Subscriber subFromDirective_;
ros::Subscriber subFromReference_;
ros::Subscriber subFromState_;
ros::Subscriber subFromPosNav_;

ros::Publisher pubToSafety_;
ros::Publisher pubToSystStatus_;
ros::Publisher pubToGlobPosInt_;
ros::Publisher pubToSonar_;


guidance_node_amsl::Directive directive_;
guidance_node_amsl::Position_nav inputPos_;
guidance_node_amsl::Reference reference_;
mms_msgs::MMS_status inputMmsStatus_;
mavros::Global_position_int globPosInt_;
mavros::Safety safety_;
mavros::Sonar sonar_;
mms_msgs::Sys_status sys_status_;



int rate = 10;

uint16_t counter_print = 0;

private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "uav_sim");
	ros::NodeHandle node;

	UavSimNodeClass uav_simNode(node);

	uav_simNode.run();
	return 0;
}