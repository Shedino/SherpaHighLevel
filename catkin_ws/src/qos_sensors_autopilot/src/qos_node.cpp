#include "ros/ros.h"

#include <sensor_msgs/image_encodings.h>
// #include "mavros/Sonar.h"
#include "mavros/ArtvaRead.h"
#include "mavros/Global_position_int.h"
#include "mavros/Attitude.h"
#include "qos_sensors_autopilot/Qos_sensors.h"
#include "qos_sensors_autopilot/Qos_autopilot.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/Range.h>
    

class QosNodeClass {
public:
	QosNodeClass(ros::NodeHandle& node){

		n_ = node;

		//subscribers
		subCamera_ = n_.subscribe("usb_cam/image_raw", 1, &QosNodeClass::imageCb, this);
		subSonar_ = n_.subscribe("sonar", 5, &QosNodeClass::sonarCb, this);
		subArtva_ = n_.subscribe("artva_read", 5, &QosNodeClass::artvaCb, this);
		subLaser_ = n_.subscribe("laser", 10, &QosNodeClass::laserCb, this);      //TODO
		subPosition_ = n_.subscribe("global_position_int", 5, &QosNodeClass::positionCb, this);
		subAttitude_ = n_.subscribe("attitude", 5, &QosNodeClass::attitudeCb, this);
		subAltimeter_ = n_.subscribe("altimeter", 5, &QosNodeClass::altimeterCb, this);
		
		// publishers
		pubQosSensors_ = n_.advertise<qos_sensors_autopilot::Qos_sensors>("qos_sensors",2);
		pubQosAutopilot_ = n_.advertise<qos_sensors_autopilot::Qos_autopilot>("qos_autopilot",2);
		
		//INIT
		rate = 10;
		
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
		qos_sens_.altimeter_present = false;
		qos_sens_.altimeter_working = false;
		qos_sens_.altimeter_failure_counter = 0;
		
		
		qos_autop_.autopilot_connected = false;
		qos_autop_.receiving_position = false;
		qos_autop_.receiving_attitude = false;

		camera_present = false;
		sonar_present = false;
		artva_present = false;
		laser_present = false;
		altimeter_present = false;

		counter_camera = 0;
		counter_camera_fail = 0;
		counter_sonar = 0;
		counter_sonar_fail = 0;
		counter_artva = 0;
		counter_artva_fail = 0;
		counter_laser = 0;
		counter_laser_fail = 0;
		
		counter_altimeter = 0;
		counter_altimeter_fail = 0;
				
		counter_pub = 0;
		counter_position = 0;
		counter_attitude = 0;		
	}

	
	void Qos_run(){
		counter_camera++;
		counter_sonar++;
		counter_artva++;
		counter_laser++;
		counter_pub++;
		counter_position++;
		counter_attitude++;
		counter_altimeter++;

		if (counter_camera >= 20){
			if (qos_sens_.camera_working) counter_camera_fail++;   //first time detected increases counter to count the fails
			qos_sens_.camera_working = false;
			if (counter_camera >= 10000) counter_camera = 20;    //for overflow
		}
		if (counter_sonar >= 20){
			if (qos_sens_.sonar_working) counter_sonar_fail++;   //first time detected increases counter to count the fails
			qos_sens_.sonar_working = false;
			if (counter_sonar >= 10000) counter_sonar = 20;    //for overflow
		}
		if (counter_artva >= 20){
			if (qos_sens_.artva_working) counter_artva_fail++;   //first time detected increases counter to count the fails
			qos_sens_.artva_working = false;
			if (counter_artva >= 10000) counter_artva = 20;    //for overflow
		}
		if (counter_laser >= 20){
			if (qos_sens_.laser_working) counter_laser_fail++;   //first time detected increases counter to count the fails
			qos_sens_.laser_working = false;
			if (counter_laser >= 10000) counter_laser = 20;    //for overflow
		}
		if (counter_altimeter >= 20){
			if (qos_sens_.altimeter_working) counter_altimeter_fail++;   //first time detected increases counter to count the fails
			qos_sens_.altimeter_working = false;
			if (counter_altimeter >= 10000) counter_altimeter = 20;    //for overflow
		}
		if (counter_position >= 20){
			qos_autop_.receiving_position = false;
			if (counter_position >= 10000) counter_position = 20;    //for overflow
		}
		if (counter_attitude >= 20){
			qos_autop_.receiving_attitude = false;
			if (counter_attitude >= 10000) counter_attitude = 20;    //for overflow
		}
		
		if (counter_pub >= 10){      //1 Hz publishing
			counter_pub = 0;
			qos_sens_.camera_present = camera_present;
			qos_sens_.camera_failure_counter = counter_camera_fail;
			qos_sens_.sonar_present = sonar_present;
			qos_sens_.sonar_failure_counter = counter_sonar_fail;
			qos_sens_.artva_present = artva_present;
			qos_sens_.artva_failure_counter = counter_artva_fail;
			qos_sens_.laser_present = laser_present;
			qos_sens_.laser_failure_counter = counter_laser_fail;
			qos_sens_.altimeter_present = altimeter_present;
			qos_sens_.altimeter_failure_counter = counter_altimeter_fail;
			qos_autop_.autopilot_connected = autopilot_connected;

			pubQosSensors_.publish(qos_sens_);
			pubQosAutopilot_.publish(qos_autop_);
		}
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg){
		if (!camera_present) camera_present = true;
		qos_sens_.camera_working = true;
		counter_camera = 0;
	}

	void sonarCb(const sensor_msgs::Range::ConstPtr& msg){
		if (!sonar_present) sonar_present = true;
		qos_sens_.sonar_working = true;
		counter_sonar = 0;
	}
	
	void laserCb(const sensor_msgs::Range::ConstPtr& msg){
		if (!laser_present) laser_present = true;
		qos_sens_.laser_working = true;
		counter_laser = 0;
	}	
	
	void altimeterCb(const sensor_msgs::Range::ConstPtr& msg){
		if (!altimeter_present) altimeter_present = true;
		qos_sens_.altimeter_working = true;
		counter_altimeter = 0;
	}	
	
	void artvaCb(const mavros::ArtvaRead::ConstPtr& msg){
		if (!artva_present) artva_present = true;
		qos_sens_.artva_working = true;
		counter_artva = 0;
	}

	void positionCb(const mavros::Global_position_int::ConstPtr& msg){        //TODO too easy check if the autopilot is connect checking just position and attitude
		if (!autopilot_connected) autopilot_connected = true;
		qos_autop_.receiving_position = true;
		counter_position = 0;
	}

	void attitudeCb(const mavros::Attitude::ConstPtr& msg){
		if (!autopilot_connected) autopilot_connected = true;
		qos_autop_.receiving_attitude = true;
		counter_attitude = 0;
	}
	
	void run() {
		ros::Rate loop_rate(rate);

		while (ros::ok())
		{
			ROS_INFO_ONCE("QoS: RUNNING");

			Qos_run();
			ros::spinOnce();

			loop_rate.sleep();
		}
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	ros::Subscriber subCamera_;
	ros::Subscriber subSonar_;
	ros::Subscriber subArtva_;
	ros::Subscriber subLaser_;
	ros::Subscriber subPosition_;
	ros::Subscriber subAttitude_;
	ros::Subscriber subAltimeter_;

	ros::Publisher pubQosSensors_;
	ros::Publisher pubQosAutopilot_;

	qos_sensors_autopilot::Qos_sensors qos_sens_;
	qos_sensors_autopilot::Qos_autopilot qos_autop_;


	int rate;
	bool camera_present;
	bool sonar_present;
	bool artva_present;
	bool laser_present;
	bool altimeter_present;	
	bool autopilot_connected;

	uint16_t counter_camera;
	uint16_t counter_camera_fail;
	uint16_t counter_sonar;
	uint16_t counter_sonar_fail;
	uint16_t counter_artva;
	uint16_t counter_artva_fail;
	uint16_t counter_laser;
	uint16_t counter_laser_fail;
	uint16_t counter_pub;
	uint16_t counter_position;
	uint16_t counter_attitude;
	uint16_t counter_altimeter;
	uint16_t counter_altimeter_fail;
	
private:

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "qos_sensors_autopilot");
	ros::NodeHandle node;

	QosNodeClass qosNode(node);

	qosNode.run();
	return 0;
}





