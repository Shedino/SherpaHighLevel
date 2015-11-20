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
#include "qos_sensors_autopilot/Qos_sensors.h"
#include <wgs84_ned_lib/wgs84_ned_lib.h> 
#include <tf/transform_datatypes.h>

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
#define LEASHING 140
#define PAUSED 150
#define MANUAL_FLIGHT 1000

double PI = 3.1416; // pi

class position_ned{
	public:
		double x;
		double y;
		double alt;
		double yaw;
};

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
		pubQosSensors_ = n_.advertise<qos_sensors_autopilot::Qos_sensors>("/qos_sensors",2);
	
		rate = 10;
		counter_print = 0;
		
		home_lat = 584943710;   //TERRA LAB
		home_lon = 151015000;
		home_alt = 100800;
		
		position_ned_.x = 0;
		position_ned_.y = 0;
		position_ned_.alt = 0;
		position_ned_.yaw = 0;
		
		qos_sens_.camera_present = false;
		qos_sens_.camera_working = false;
		qos_sens_.camera_failure_counter = 0;
		qos_sens_.sonar_present = false;
		qos_sens_.sonar_working = false;
		qos_sens_.sonar_failure_counter = 0;
		qos_sens_.artva_present = false;
		qos_sens_.artva_working = false;
		qos_sens_.artva_failure_counter = 0;
		qos_sens_.laser_present = false;
		qos_sens_.laser_working = false;
		qos_sens_.laser_failure_counter = 0;
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

		switch(inputMmsStatus_.mms_state)
		{
			case ON_GROUND_NO_HOME:
				safety_.safety = false;                              //HARDCODED INITIALIAZED
				sys_status_.armed = false;             //armed
				sys_status_.voltage_battery = 15000;  //15 V
				sys_status_.valid_ref_frame = 11;     //baro
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
				
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;

			case IN_FLIGHT:
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;

			case PERFORMING_GO_TO:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING GO TO");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;

			case GRID:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING GRID");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;

			case PERFORMING_LANDING:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING LANDING");
					ROS_INFO("SIM: Alt: %d - Rel_alt: %d", globPosInt_.alt, globPosInt_.relative_alt);
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;
			
			case MANUAL_FLIGHT:
				break;

			case LEASHING:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING LEASHING");
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;
				
			case PAUSED:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING PAUSED");
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;
		}
		
		double temp_ref_latitude;
		double temp_ref_longitude;
		get_pos_WGS84_from_NED (&temp_ref_latitude, &temp_ref_longitude, position_ned_.x, position_ned_.y, home_lat/10000000.0f, home_lon/10000000.0f);
		globPosInt_.time_boot_ms = 0;
		globPosInt_.lat = int(temp_ref_latitude*10000000.0f);    
		globPosInt_.lon = int(temp_ref_longitude*10000000.0f);	
		globPosInt_.alt = int(position_ned_.alt*1000.0f) + home_alt;		
		globPosInt_.relative_alt = int(position_ned_.alt*1000.0f); 
		globPosInt_.vx = 0;
		globPosInt_.vy = 0;
		globPosInt_.vz = 0;
		globPosInt_.hdg = position_ned_.yaw * 180 / M_PI * 100;            //degrees * 100*/
		
		pubToGlobPosInt_.publish(globPosInt_);
		
		geopoint_.latitude = ((double)globPosInt_.lat) / 10000000.0;
		geopoint_.longitude = ((double)globPosInt_.lon) / 10000000.0;
		geopoint_.altitude = globPosInt_.alt / 1000.0f;
		geopose_.position = geopoint_;
		quaternion_ = tf::createQuaternionMsgFromRollPitchYaw(0,0,position_ned_.yaw);
		//quaternion_.x = cos(position_ned_.yaw/2);
		//quaternion_.y = 0;
		//quaternion_.z = 0;
		//quaternion_.w = sin(position_ned_.yaw/2);
		geopose_.orientation = quaternion_;
		pubGeopose_.publish(geopose_);
		
		pubToSystStatus_.publish(sys_status_);
		if (globPosInt_.relative_alt <= 3000)
			sonar_.distance = globPosInt_.relative_alt;
		else
			sonar_.distance = 0;
		pubToSonar_.publish(sonar_);
		pubQosSensors_.publish(qos_sens_);
}




void run() {
	ros::Rate loop_rate(rate);
	
	//read parameters here
	bool camera_pres;
	bool sonar_pres;
	bool artva_pres;
	bool laser_pres;
	if (n_.getParam("uav_sim/camera_present", camera_pres)){
		qos_sens_.camera_present = camera_pres;
		qos_sens_.camera_working = camera_pres;
		ROS_INFO("SIM: Camera present: %s", qos_sens_.camera_present ? "yeah" : "nooo");
	}
	if (n_.getParam("uav_sim/sonar_present", sonar_pres)){
		qos_sens_.sonar_present = sonar_pres;
		qos_sens_.sonar_working = sonar_pres;
		ROS_INFO("SIM: Sonar present: %s", qos_sens_.sonar_present ? "yeah" : "nooo");
	}
	if (n_.getParam("uav_sim/artva_present", artva_pres)){
		qos_sens_.artva_present = artva_pres;
		qos_sens_.artva_working = artva_pres;
		ROS_INFO("SIM: ARTVA present: %s", qos_sens_.artva_present ? "yeah" : "nooo");
	}
	if (n_.getParam("uav_sim/laser_present", laser_pres)){
		qos_sens_.laser_present = laser_pres;
		qos_sens_.laser_working = laser_pres;
		ROS_INFO("SIM: Laser present: %s", qos_sens_.laser_present ? "yeah" : "nooo");
	}
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
ros::Publisher pubQosSensors_;


guidance_node_amsl::Directive directive_;
guidance_node_amsl::Position_nav inputPos_;
guidance_node_amsl::Reference reference_;
mms_msgs::MMS_status inputMmsStatus_;
mavros::Global_position_int globPosInt_;
mavros::Safety safety_;
mavros::Sonar sonar_;
mms_msgs::Sys_status sys_status_;
qos_sensors_autopilot::Qos_sensors qos_sens_;

geographic_msgs::GeoPose geopose_;
geographic_msgs::GeoPoint geopoint_;
geometry_msgs::Quaternion quaternion_;

int rate;

int home_lat;
int home_lon;
int home_alt;

position_ned position_ned_;

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
