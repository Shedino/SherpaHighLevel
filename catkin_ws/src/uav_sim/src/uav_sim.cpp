#include "ros/ros.h"

#include <tf/transform_broadcaster.h>
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
#include "geometry_msgs/Point.h"
#include "qos_sensors_autopilot/Qos_sensors.h"
#include <wgs84_ned_lib/wgs84_ned_lib.h> 
#include <tf/transform_datatypes.h>
#include "swm_interface/Query.h"
#include "geometry_msgs/Vector3.h" // Added by NIcola 2016/12/10
#include "mavros/ArtvaRead.h" // Added by NIcola 2016/12/10

#include <math.h>



// STATES DEFINITION
/*#define ON_GROUND_NO_HOME 10
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
#define MANUAL_FLIGHT 1000*/

double PI = 3.1416; // pi

class position_ned{
	public:
		double x;
		double y;
		double alt;
		double yaw;
};

class matrix3  // Added by NIcola 2016/12/10
{
	public:
	double e11, e12, e13;
	double e21, e22, e23;
	double e31, e32, e33;
};
	
class ortovox  // Added by NIcola 2016/12/10
{
	public:
	int d;
	int delta;
};

class UavSimNodeClass {
public:
	UavSimNodeClass(ros::NodeHandle& node){

		n_=node;

		//subscribers
		subFromDirective_ = n_.subscribe("directive", 10, &UavSimNodeClass::readDirective,this);
		subFromReference_ = n_.subscribe("reference", 10, &UavSimNodeClass::readReference,this);
		subFromState_ = n_.subscribe("mms_status", 10, &UavSimNodeClass::readState,this);
		subFromPosNav_ = n_.subscribe("position_nav", 10, &UavSimNodeClass::readPosNav,this);
		
		// publishers
		pubToSafety_ = n_.advertise<mavros::Safety>("safety_odroid",10);
		pubToSystStatus_ = n_.advertise<mms_msgs::Sys_status>("system_status",10);
		pubToGlobPosInt_ = n_.advertise<mavros::Global_position_int>("global_position_int",10);
		pubToSonar_ = n_.advertise<mavros::Sonar>("sonar",10);
		pubGeopose_ = n_.advertise<geographic_msgs::GeoPose>("geopose",10);
		pubQosSensors_ = n_.advertise<qos_sensors_autopilot::Qos_sensors>("qos_sensors",2);
		pubNedPose_ = n_.advertise<geometry_msgs::Pose>("ned_pose",2);
		artva_pub = n_.advertise<mavros::ArtvaRead>("artva_read", 2); // Added by NIcola 2016/12/10
		
		// Services
		client_query = n_.serviceClient<swm_interface::Query>("query_swm");

		rate = 10;
		counter_print = 0;
		
		if (n_.getParam("/home_lat", temp_lat)){
			home_lat = temp_lat*10000000;
			ROS_INFO("SIM: Set home lat");
		} else {
			home_lat = 441532780;   //CESENA LAB
		}
		if (n_.getParam("/home_lon", temp_lon)){
			home_lon = temp_lon*10000000;
			ROS_INFO("SIM: Set home lon");
		} else {
			home_lon = 151015000; //CESENA LAB
		}
		if (n_.getParam("/home_alt", temp_alt)){
			home_alt = temp_alt*1000;
			ROS_INFO("SIM: Set home alt");
		} else {
			home_alt = 41000; //CESENA LAB
		}

		/*home_lat = 584943710;   //TERRA LAB
		home_lon = 151015000;
		home_alt = 100800;*/

		/*home_lat = 441532780;   //CESENA LAB
		home_lon = 122414260;
		home_alt = 41000;*/

		/*home_lat = 464869000;   //PORDOI
		home_lon = 118142200;
		home_alt = 2200000;*/
		
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
	
		// Added by NIcola 2016/12/10
		/* initialize random seed: */
		srand (time(NULL));
		pt.x = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100.0f)));//(rand()%100)-50; 
		pt.y = -50.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100.0f)));//(rand()%100)-50; 
		pt.z = -2.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0f)));   //(rand()%2) - 2; 
		//ROS_WARN("Victim at: %f ; %f ; %f", pt.x, pt.y, pt.z);
		// --------------------------
		// Added by NIcola 2016/12/10
		double phi = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*PI))); // rand() % (int 2*PI);
		double theta = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*PI))); // rand() % (int 2*PI);
		double psi = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*PI))); // rand() % (int 2*PI);
		//ROS_WARN("angles: %f ; %f ; %f", phi, theta, psi);
		Rt.e11 = cos(theta)*cos(psi);
		Rt.e12 = sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi);
		Rt.e13 = cos(phi)*sin(theta)*cos(psi)+cos(phi)*sin(psi);
		Rt.e21 = cos(theta)*sin(psi);
		Rt.e22 = sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi);
		Rt.e23 = cos(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi);
		Rt.e31 = -sin(theta);
		Rt.e32 = sin(phi)*cos(theta);
		Rt.e33 = cos(phi)*cos(theta);
		//ROS_WARN("Victim attitude 1st row: %f - %f - %f", Rt.e11, Rt.e12, Rt.e13);
		//ROS_WARN("Victim attitude 2nd row: %f - %f - %f", Rt.e21, Rt.e22, Rt.e23);
		//ROS_WARN("Victim attitude 3rd row: %f - %f - %f", Rt.e31, Rt.e32, Rt.e33);
		// --------------------------

		// ros::Time new_artva_time;
		last_artva_time = ros::Time::now();
		// last_artva_time = ros::Time::now();
		// ros::Time new_artva_time = ros::Time::now();
		}

	// Added by NIcola 2016/12/10
	void arva_sim(geometry_msgs::Point pt, geometry_msgs::Point p, matrix3 Rt, matrix3 R)
	{   // [d,delta] = arva_sim(pt,p,Rt,R)
		
		double radius;
		double n_H_noise, r_noise;
		double d1, d3;
		double normHb;
		// vector3 p, pt,
		geometry_msgs::Vector3 m_b, m_i;
		geometry_msgs::Vector3 r, Am;
		geometry_msgs::Vector3 H_noise, Ht, H, Hb;
		// matrix3 R, Rt;
		matrix3 A;
		// ortovox arva_msg; 
		
		// MAGNETIC DIPOLE
		m_b.x = 1.0f;
		m_b.y = 0.0f;
		m_b.z = 0.0f;
		// m_vect = Rt*[m;0;0];
		m_i.x = Rt.e11*m_b.x+Rt.e12*m_b.y+Rt.e13*m_b.z;
		m_i.y = Rt.e21*m_b.x+Rt.e22*m_b.y+Rt.e23*m_b.z;
		m_i.z = Rt.e31*m_b.x+Rt.e32*m_b.y+Rt.e33*m_b.z;
		//ROS_WARN("m: %f - %f - %f", m_i.x, m_i.y, m_i.z);

		// NOISE
		r_noise = 50.0f; // [m]
		n_H_noise = 1.0f/pow(r_noise,3)*(1/(4*PI))*1.5420f; // 1.5420
		//ROS_WARN("n_H_noise: %f", n_H_noise);

		// Relative position
		r.x = p.x-pt.x;
		r.y = p.y-pt.y;
		r.z = p.z-pt.z;
		//ROS_WARN("r: %f - %f - %f", r.x, r.y, r.z);

		A.e11 = 2.*pow(r.x,2)-pow(r.y,2)-pow(r.z,2);
		A.e12 = 3.*r.x*r.y;
		A.e13 = 3.*r.x*r.z;
		A.e21 = A.e12;
		A.e22 = 2.*pow(r.y,2)-pow(r.x,2)-pow(r.z,2);
		A.e23 = 3.*r.y*r.z;
		A.e31 = A.e13;
		A.e32 = A.e23;
		A.e33 = 2.*pow(r.z,2)-pow(r.x,2)-pow(r.y,2);
		Am.x = A.e11*m_i.x+A.e12*m_i.y+A.e13*m_i.z;
		Am.y = A.e21*m_i.x+A.e22*m_i.y+A.e23*m_i.z;
		Am.z = A.e31*m_i.x+A.e32*m_i.y+A.e33*m_i.z;

		radius = sqrt(pow(r.x,2)+pow(r.y,2)+pow(r.z,2));

		/* initialize random seed:
		srand (time(NULL));*/

		/* generate secret number between 1 and 10: */
        	// iSecret = rand() % 10 + 1;
		H_noise.x =  -1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0f)));//(rand()%2)-1;
		float temp = -1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.0f)));//(rand()%2)-1;
		H_noise.y = temp*sqrt(1-pow(H_noise.x,2)); 
		H_noise.z = pow(-1,(rand() % 1))*sqrt(1-pow(H_noise.x,2)-pow(H_noise.y,2));
		H_noise.x = H_noise.x*n_H_noise;
		H_noise.y = H_noise.y*n_H_noise;
		H_noise.z = H_noise.z*n_H_noise;
		//ROS_WARN("H_noise.x; H_noise.y; H_noise.z : %f ; %f ; %f", H_noise.x, H_noise.y, H_noise.z);

		Ht.x = 1/(4*PI*pow(radius,5))*Am.x;
		Ht.y = 1/(4*PI*pow(radius,5))*Am.y;
		Ht.z = 1/(4*PI*pow(radius,5))*Am.z;
		//ROS_WARN("Ht.x; Ht.y; Ht.z : %f ; %f ; %f", Ht.x, Ht.y, Ht.z);

		H.x = Ht.x+H_noise.x;
		H.y = Ht.y+H_noise.y;
		H.z = Ht.z+H_noise.z;
		//ROS_WARN("H.x; H.y; H.z : %f ; %f ; %f", H.x, H.y, H.z);

		// H = [H_x;H_y;H_z];

		// Rotation to the body frame
		// R = from body to inertial
		// Hb = transp(R)*H;
		Hb.x = R.e11*H.x+R.e21*H.y+R.e31*H.z;
		Hb.y = R.e12*H.x+R.e22*H.y+R.e32*H.z;
		Hb.z = R.e13*H.x+R.e23*H.y+R.e33*H.z;
		//ROS_WARN("Hb.x; Hb.y; Hb.z : %f ; %f ; %f", Hb.x, Hb.y, Hb.z);
		normHb = sqrt(pow(Hb.z,2)+pow(Hb.y,2)+pow(Hb.z,2));
		float temp1  = abs(Hb.x);
		float temp2  = sqrt(pow(Hb.x,2));
		//ROS_WARN("Hb.x; abs(Hb.x); my_abs(Hb.x): %f; %f ; %f", Hb.x, temp1, temp2);
		d1 = pow(1.0f/temp2*(1/(4*PI))*1.5420,1.0f/3.0f);
		d3 = pow(1.0f/normHb*(1/(4*PI))*1.5420,1.0f/3.0f);

		// ROS_WARN("d1, d3: %f - %f", d1, d3);

		if(d3 <= 3.0f)
		{
			ROS_WARN("ARVA VALID SIGNAL: PINPOINTING");
			ROS_WARN("d3: %f", d3);
			arva_msg.d = (int (d3*100+0.5f));
			arva_msg.delta = 0;			
		}
		else if (d1 <= r_noise)
		{
			ROS_WARN("ARVA VALID SIGNAL: FLUX LINE");
			ROS_WARN("d1, delta: %f - %f", d1, (atan(Hb.y/Hb.x)*180/PI));
			arva_msg.d = (int (d1*100+0.5f));
			if (Hb.y/Hb.x >= 0.0f)
			{
				arva_msg.delta = (int (atan(Hb.y/Hb.x)*180/PI+0.5f));
			}
			else
			{
				arva_msg.delta = (int (atan(Hb.y/Hb.x)*180/PI-0.5f));
			}
		}
		else
		{
			ROS_WARN("NO ARVA VALID SIGNAL");
			arva_msg.d = -1;
			arva_msg.delta = 0;
		}	

		/*// Discretization
		if(arva_msg.d != -1)
		{
			arva_msg.d = arva_msg.d*100;
			arva_msg.delta = round(arva_msg.delta*180/PI);
		}*/
	}	
	// --------------------------

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
		inputMmsStatus_ = *msg;
		currentState = msg->mms_state;
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

		/*if (counter_print>20){
			std::string query = "query_random";
			srv_query.request.query = query;
			srv_query.request.param1 = 1.0;
			srv_query.request.param2 = 2.0;
			srv_query.request.param3 = 3.0;
			srv_query.request.param4 = 4.0;
			srv_query.request.param5 = 5.0;
			srv_query.request.param6 = 6.1;
			srv_query.request.param7 = 7.0;
			if (client_query.call(srv_query))
			{
				ROS_INFO("Query test: %s - %f - %f", srv_query.response.query_answ.c_str(), srv_query.response.answ1, srv_query.response.answ2);
			}
			else
			{
				ROS_ERROR("Failed to call service query_swm");
			}
			counter_print = 0;
		}*/

		switch(currentState)
		{
			case mms_msgs::MMS_status::ON_GROUND_NO_HOME:
				safety_.safety = false;                              //HARDCODED INITIALIAZED
				sys_status_.armed = false;             //armed
				sys_status_.voltage_battery = 15000 + rand() % 1000;
				// std::cout << "batt rand: " << static_cast <int> (rand()) /( static_cast <int> (RAND_MAX/(1000)))  << std::endl;
				sys_status_.valid_ref_frame = 11;     //baro
				break;

			case mms_msgs::MMS_status::SETTING_HOME:
				break;

			case mms_msgs::MMS_status::ON_GROUND_DISARMED:
				break;

			case mms_msgs::MMS_status::ARMING:
				sys_status_.armed = true;             //armed
				pubToSystStatus_.publish(sys_status_);
				break;

			case mms_msgs::MMS_status::DISARMING:
				sys_status_.armed = false;             //armed
				pubToSystStatus_.publish(sys_status_);
				break;

			case mms_msgs::MMS_status::ON_GROUND_ARMED:
				break;

			case mms_msgs::MMS_status::PERFORMING_TAKEOFF:
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

			case mms_msgs::MMS_status::IN_FLIGHT:
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;

			case mms_msgs::MMS_status::PERFORMING_GO_TO:
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

			case mms_msgs::MMS_status::GRID:
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

			case mms_msgs::MMS_status::PERFORMING_LANDING:
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
			
			case mms_msgs::MMS_status::MANUAL_FLIGHT:
				break;

			case mms_msgs::MMS_status::LEASHING:
				if (counter_print >= 30){
					counter_print = 0;
					ROS_INFO("SIM: PERFORMING LEASHING");
				}
				position_ned_.x += (directive_.vxBody*cos(position_ned_.yaw) - directive_.vyBody*sin(position_ned_.yaw))/ 10;
				position_ned_.y += (directive_.vxBody*sin(position_ned_.yaw) + directive_.vyBody*cos(position_ned_.yaw))/ 10;
				position_ned_.alt += -directive_.vzBody / 10;
				position_ned_.yaw += directive_.yawRate / 10;
				break;
				
			case mms_msgs::MMS_status::PAUSED:
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
		
		if (position_ned_.yaw<0){    //reset yaw between [0;2pi] to be complaint with APM
			position_ned_.yaw = 2*M_PI;
		}
		if (position_ned_.yaw>2*M_PI){
			position_ned_.yaw = 0;
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
		
		//Publish TF for visualization
		static tf::TransformBroadcaster br;
		tf::Transform transform;
		transform.setOrigin(tf::Vector3(position_ned_.x, position_ned_.y, position_ned_.alt));
		tf::Quaternion q;
		q.setRPY(0, 0, 0);
		transform.setRotation(q);
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "wasp"));

		//Publish Pose
		geometry_msgs::Point p;
		p.x = position_ned_.x; p.y = position_ned_.y; p.z = position_ned_.alt;
		ned_pose.position = p;
		ned_pose.orientation = quaternion_;
		pubNedPose_.publish(ned_pose);

		//Publish State
		pubToSystStatus_.publish(sys_status_);

		//Publish Sonar
		if (globPosInt_.relative_alt <= 3000 && globPosInt_.relative_alt >= 0)
			sonar_.distance = globPosInt_.relative_alt;
		else
			sonar_.distance = 0;
		pubToSonar_.publish(sonar_);

		//Publish QoS sensors
		pubQosSensors_.publish(qos_sens_);

		// Added by NIcola 2016/12/10
		// ARTVA SIMULATION		
		/*p.x = position_ned_.x;
		p.y = position_ned_.y;
		p.z = position_ned_.alt;*/

		R.e11 = cos(position_ned_.yaw); 
		R.e22 = R.e11;
		R.e33 = 1.0f;
		R.e21 = -sin(position_ned_.yaw);
		R.e31 = 0.0f;
		R.e12 = sin(position_ned_.yaw);
		R.e13 = 0.0f;
		R.e23 = 0.0f;
		R.e32 = 0.0f;
		
		arva_sim(pt,p,Rt,R);

		// PREPARE ARTVA MESSAGE
		ARTVAread_msg.rec1_distance = arva_msg.d;
		ARTVAread_msg.rec1_direction = arva_msg.delta;
		ARTVAread_msg.rec2_distance = -1;
		ARTVAread_msg.rec2_direction = 0;
		ARTVAread_msg.rec3_distance = -1;
		ARTVAread_msg.rec3_direction = 0;
		ARTVAread_msg.rec4_distance = -1;
		ARTVAread_msg.rec4_direction = 0;		
		//Publish ARTVA
		ros::Time new_artva_time = ros::Time::now();
		// ROS_INFO("arva_time: %i", new_artva_time.sec);
		if (new_artva_time.sec-last_artva_time.sec >= 1)
		{
			last_artva_time = new_artva_time;
			if (qos_sens_.artva_present == true and qos_sens_.artva_working == true)
					artva_pub.publish(ARTVAread_msg);
		}
		// --------------------------

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
ros::Publisher pubNedPose_;

ros::ServiceClient client_query;
swm_interface::Query srv_query;

guidance_node_amsl::Directive directive_;
guidance_node_amsl::Position_nav inputPos_;
guidance_node_amsl::Reference reference_;
mms_msgs::MMS_status inputMmsStatus_;
mavros::Global_position_int globPosInt_;
mavros::Safety safety_;
mavros::Sonar sonar_;
mms_msgs::Sys_status sys_status_;
qos_sensors_autopilot::Qos_sensors qos_sens_;
geometry_msgs::Pose ned_pose;

uint16_t currentState;

geographic_msgs::GeoPose geopose_;
geographic_msgs::GeoPoint geopoint_;
geometry_msgs::Quaternion quaternion_;

int rate;

int home_lat;
int home_lon;
int home_alt;

double temp_lat;
double temp_lon;
double temp_alt;

position_ned position_ned_;

ros::Time last_artva_time;
uint16_t counter_print;

// Added by NIcola 2016/12/10
/* ARTVA DATA */ 
ros::Publisher artva_pub;
mavros::ArtvaRead ARTVAread_msg;
matrix3 Rt, R;
geometry_msgs::Point pt, p;
ortovox arva_msg;
// --------------------------
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
