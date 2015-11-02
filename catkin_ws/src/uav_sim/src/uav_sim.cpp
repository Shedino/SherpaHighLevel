#include "ros/ros.h"

#include "guidance_node_amsl/Directive.h"
#include "guidance_node_amsl/Reference.h"
#include "mms_msgs/MMS_status.h"
#include "guidance_node_amsl/Position_nav.h"
#include "mavros/Global_position_int.h"
#include "mavros/Safety.h"
#include "mavros/Sonar.h"
#include "mms_msgs/Sys_status.h"
#include "geographic_msgs/GeoPose.h"
#include "geographic_msgs/GeoPoint.h"
#include "geometry_msgs/Quaternion.h"

#include <math.h>



// STATES DEFINITION
#define ON_GROUND_NO_HOME 10
#define SETTING_HOME 20
#define ON_GROUND_DISARMED 30
#define ARMING 40
#define DISARMING 45
#define ON_GROUND_ARMED 50
#define PERFORMING_TAKEOFF 70
#define IN_FLIGHT 80
#define GRID 90
#define PERFORMING_GO_TO 100
#define PERFORMING_LANDING 120
#define MANUAL_FLIGHT 1000

double PI = 3.1416; // pi

class UavSimNodeClass {
public:
	UavSimNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromDirective_ = n_.subscribe("/directive", 10, &UavSimNodeClass::readDirective,this);
		subFromReference_ = n_.subscribe("/reference", 10, &UavSimNodeClass::readReference,this);
		subFromState_ = n_.subscribe("/mms_status", 10, &UavSimNodeClass::readState,this);
		subFromPosNav_ = n_.subscribe("/position_nav", 10, &UavSimNodeClass::readPosNav,this);
		
		// publishers
		pubToSafety_ = n_.advertise<mavros::Safety>("/safety_odroid",10);
		pubToSystStatus_ = n_.advertise<mms_msgs::Sys_status>("/system_status",10);
		pubToGlobPosInt_ = n_.advertise<mavros::Global_position_int>("/global_position_int",10);
		pubToSonar_ = n_.advertise<mavros::Sonar>("/sonar",10);
		pubGeopose_ = n_.advertise<geographic_msgs::GeoPose>("geopose",10);
	
		rate = 10;
		counter_print = 0;
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
		counter_print++;
		//pubToSafety_.publish(safety_);
		pubToSystStatus_.publish(sys_status_);
		pubToSonar_.publish(sonar_);
		quaternion_.x = 1;
		quaternion_.y = 0;
		quaternion_.z = 0;
		quaternion_.w = 0;

		switch(inputMmsStatus_.mms_state)
		{
			case ON_GROUND_NO_HOME:
				safety_.safety = false;                              //HARDCODED INITIALIAZED
				sonar_.distance = 5000;
				sys_status_.armed = false;             //armed
				sys_status_.voltage_battery = 15000;  //15 V
				sys_status_.valid_ref_frame = 11;     //baro
				globPosInt_.time_boot_ms = 0;
				globPosInt_.lat = 584943710;     //TERRA LAB
				globPosInt_.lon = 151015000;	//TERRA LAB
				globPosInt_.alt = 100800;		//TERRA LAB
				globPosInt_.relative_alt = 0; //starting from 0
				globPosInt_.vx = 0;
				globPosInt_.vy = 0;
				globPosInt_.vz = 0;
				globPosInt_.hdg = 0;            //degrees * 100
				break;

			case SETTING_HOME:
				break;

			case ON_GROUND_DISARMED:
				break;

			case ARMING:
				sys_status_.armed = true;             //armed
				pubToSystStatus_.publish(sys_status_);
				break;

			case DISARMING:
				sys_status_.armed = false;             //armed
				pubToSystStatus_.publish(sys_status_);
				break;

			case ON_GROUND_ARMED:
				break;

			case PERFORMING_TAKEOFF:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING TAKEOFF");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/60;
				globPosInt_.alt += (reference_.AltitudeRelative - inputPos_.Altitude)/60;
				break;

			case IN_FLIGHT:
				break;

			case PERFORMING_GO_TO:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING GO TO");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;
				globPosInt_.alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;   
				globPosInt_.lat += (reference_.Latitude - inputPos_.Latitude)/70;
				globPosInt_.lon += (reference_.Longitude - inputPos_.Longitude)/70;
				globPosInt_.hdg += (reference_.Yawangle - inputPos_.YawAngle)*180/M_PI*5;
				break;

			case GRID:	
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING GRID");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;
				globPosInt_.alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;  
				globPosInt_.lat += (reference_.Latitude - inputPos_.Latitude)/70;
				globPosInt_.lon += (reference_.Longitude - inputPos_.Longitude)/70;
				break;

			case PERFORMING_LANDING:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING LANDING");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				globPosInt_.relative_alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;
				globPosInt_.alt += (reference_.AltitudeRelative - inputPos_.Altitude)/80;    
				break;
			
			case MANUAL_FLIGHT:
				break;
		}
		
		std::string location;                                 //location: for indoor at terra  TODO remove later
		if (n_.getParam("/location", location)){
			if (location == "terra"){
				int temp_lat = globPosInt_.lat;
				globPosInt_.lat = -globPosInt_.lon;
				globPosInt_.lon = temp_lat;
			}
		}
		pubToGlobPosInt_.publish(globPosInt_);
		
		geopoint_.latitude = globPosInt_.lat / 10000000.0f;
		geopoint_.longitude = globPosInt_.lon / 10000000.0f;
		geopoint_.altitude = globPosInt_.alt / 1000.0f;
		geopose_.position = geopoint_;
		geopose_.orientation = quaternion_;
		pubGeopose_.publish(geopose_);
}




void run() {
	ros::Rate loop_rate(rate);

	while (ros::ok())
	{
		ROS_INFO_ONCE("SIM: RUNNING");

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
ros::Publisher pubGeopose_;


guidance_node_amsl::Directive directive_;
guidance_node_amsl::Position_nav inputPos_;
guidance_node_amsl::Reference reference_;
mms_msgs::MMS_status inputMmsStatus_;
mavros::Global_position_int globPosInt_;
mavros::Safety safety_;
mavros::Sonar sonar_;
mms_msgs::Sys_status sys_status_;

geographic_msgs::GeoPose geopose_;
geographic_msgs::GeoPoint geopoint_;
geometry_msgs::Quaternion quaternion_;

int rate;

uint16_t counter_print;

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
