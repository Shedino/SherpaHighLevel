#include "ros/ros.h"
#include <vector>

#include "guidance_node_amsl/Directive.h"
#include "guidance_node_amsl/Position_nav.h"
#include "guidance_node_amsl/Reference.h"

#include <mavros/Safety.h>
#include <mavros/Attitude.h>

#include <mms_msgs/MMS_status.h>

#include "Model_GS.h" //qui devo stare attento a come si chiama il file.h

uint16_t counter_print;

class GuidanceNodeClass {
public:
	GuidanceNodeClass(ros::NodeHandle& node){

		n_=node;

		param_ = std::vector<double>(9);

		//frequency
		node.param("guidance_node_amsl/frequency",rate,10);

		// DEBUG param test
		node.param("guidance_node_amsl/debug/testOffset",debugParam,1.0);

		//control param
		node.param("guidance_node_amsl/param/sat_xy", param_[0], 3.0);
		node.param("guidance_node_amsl/param/sat_z", param_[1], 1.5);
		node.param("guidance_node_amsl/param/sat_yaw", param_[2], 3.14);
		node.param("guidance_node_amsl/param/gain_NE", param_[3], 0.06);
		node.param("guidance_node_amsl/param/gain_D", param_[4], 0.06);
		node.param("guidance_node_amsl/param/gain_yaw", param_[5], 0.3);
		node.param("guidance_node_amsl/param/deadzone_NE", param_[6], 1.5);
		node.param("guidance_node_amsl/param/deadzone_D", param_[7], 0.2);
		node.param("guidance_node_amsl/param/gain_integralDown", param_[8],0.025);

		//subscribers and publishers
		subFromReference_=n_.subscribe("/reference",10, &GuidanceNodeClass::readReferenceMessage, this);       //this shoud be published by reference_traj node based on flightmode
		subFromPosition_=n_.subscribe("/position_nav", 10, &GuidanceNodeClass::readPositionMessage,this);      //this shoud be published by reference_traj node based on flightmode
		pub_=n_.advertise<guidance_node_amsl::Directive>("/directive", 10);
		safety_sub = n_.subscribe("/safety_odroid",10, &GuidanceNodeClass::handle_safety, this);
		attitude_sub = n_.subscribe("/attitude",10, &GuidanceNodeClass::handle_attitude, this);
		mms_status_sub = n_.subscribe("/mms_status",10, &GuidanceNodeClass::handle_mms_status, this);

		//Initializing inputRef_
		node.param<int>("guidance_node_amsl/ref/latitude_ref", inputRef_.Latitude, 0);
		node.param<int>("guidance_node_amsl/ref/longitude_ref", inputRef_.Longitude, 0);
		node.param<int>("guidance_node_amsl/ref/altitude_ref", inputRef_.AltitudeRelative, 1000);
		double temp;
		node.param("guidance_node_amsl/ref/yaw_ref",  temp, 0.0);
		inputRef_.Yawangle= (float) temp;
		inputRef_.Mode = 0;

	}

	void readReferenceMessage(const guidance_node_amsl::Reference::ConstPtr& msg)
	{
		/*
		 * uso il campo safety per discernere se il messaggio che mi arriva è tipo relativo o assoluto
		 * Al momento è buono per testing, più avanti è da consolidare
		 */
		if(msg->Mode==100){
			//relative
			inputRef_.Latitude = position_nav_.Latitude + msg->Latitude;
			inputRef_.Longitude = position_nav_.Longitude + msg->Longitude;
			inputRef_.AltitudeRelative = position_nav_.Altitude + msg->AltitudeRelative;
			inputRef_.Yawangle = yaw_ + msg->Yawangle;
		} else {
			//absolute
			inputRef_.Latitude = msg->Latitude;
			inputRef_.Longitude = msg->Longitude;
			inputRef_.AltitudeRelative = msg->AltitudeRelative;
			inputRef_.Yawangle = msg->Yawangle;
		}
	}

	void readPositionMessage(const guidance_node_amsl::Position_nav::ConstPtr& msg)  
	{
		position_nav_.Latitude = msg->Latitude;
		position_nav_.Longitude = msg->Longitude;
		position_nav_.Altitude = msg->Altitude;
		position_nav_.YawAngle = msg->YawAngle;
		position_nav_.Timestamp = msg->Timestamp;
	}

	void handle_safety(const mavros::Safety::ConstPtr& msg)
	{
		safety_ = msg->safety;
	}
	
	void handle_mms_status(const mms_msgs::MMS_status::ConstPtr& msg)
	{
		if (msg->mms_state == 70) trigger_ = true;          //trigger integral on PERFORMING_TAKEOFF state
		else if (msg->mms_state == 30) trigger_ = false;		//disable integral on ON_GROUND_DISARMED state --> this happens after LAND
	}

	void handle_attitude(const mavros::Attitude::ConstPtr& msg)
	{
		yaw_ = msg->yaw;          //need only yaw for navigation
	}

	void process(Model_GSModelClass &guidanceClass)
	{

		guidance_node_amsl::Reference inputRef = inputRef_;             //TODO check that this is not needed, I can pass directly the private one

		guidance_node_amsl::Directive output;

		initializeParameters(guidanceClass, position_nav_, inputRef, param_, yaw_, safety_);//initialize input parameters;        
		guidanceClass.step(); //step the Simulink function
		getOutput(guidanceClass, &output); //get the output

		pub_.publish(output);

		ros::spinOnce();
	}

	void run() {
		ros::Rate loop_rate(rate);
		Model_GSModelClass guidanceClass;
		guidanceClass.initialize();
		counter_print++;
		if (counter_print >= 11){
			counter_print = 0;
		}
		while (ros::ok())
		{
			process(guidanceClass);
			loop_rate.sleep();
		}
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	ros::Subscriber subFromReference_;
	ros::Subscriber subFromPosition_;
	ros::Subscriber safety_sub;
	ros::Subscriber attitude_sub;
	ros::Subscriber mms_status_sub;

	guidance_node_amsl::Reference inputRef_;
	//guidance_node_amsl::Position inputPos_;
	guidance_node_amsl::Position_nav position_nav_;

	ros::Publisher pub_;

	bool safety_;
	bool trigger_;    //trigger for integral
	float yaw_;

	int rate;

	std::vector<double> param_;
	double debugParam;

private:
	// OLD WAS (Model_GSModelClass &guidanceClass, const guidance_node_amsl::Position &inputPos, const guidance_node_amsl::Reference &inputRef, const std::vector<double> &param)
	void initializeParameters(Model_GSModelClass &guidanceClass,
			const guidance_node_amsl::Position_nav &inputPos,
			const guidance_node_amsl::Reference &inputRef,
			const std::vector<double> &param,
			float yaw,
			bool safety) {

		guidanceClass.Model_GS_U.Actual_Pos[0] = inputPos.Latitude;
		guidanceClass.Model_GS_U.Actual_Pos[1] = inputPos.Longitude;
		guidanceClass.Model_GS_U.Actual_Pos[2] = inputPos.Altitude;     //WAS inputPos.AltitudeRelative
		guidanceClass.Model_GS_U.Actual_Pos[3] = 0; 			//WAS inputPos.AltitudeAMSL
		guidanceClass.Model_GS_U.Actual_Yaw = inputPos.YawAngle;

		guidanceClass.Model_GS_U.Reference_Pos[0]=inputRef.Latitude;
		guidanceClass.Model_GS_U.Reference_Pos[1]=inputRef.Longitude;
		guidanceClass.Model_GS_U.Reference_Pos[2]=inputRef.AltitudeRelative;
		guidanceClass.Model_GS_U.Reference_Yaw=inputRef.Yawangle;

		//Saturation, gain and deadzones
		//forced cast due to parameter server only storing double
		guidanceClass.Model_GS_U.Control_Param[0]= (float) param[0]; //sat xy
		guidanceClass.Model_GS_U.Control_Param[1]= (float) param[1]; //sat z
		guidanceClass.Model_GS_U.Control_Param[2]= (float) param[2]; //sat yaw
		guidanceClass.Model_GS_U.Control_Param[3]= (float) param[3]; //gain NE
		guidanceClass.Model_GS_U.Control_Param[4]= (float) param[4]; //gain D
		guidanceClass.Model_GS_U.Control_Param[5]= (float) param[5]; //gain yaw
		guidanceClass.Model_GS_U.Control_Param[6]= (float) param[6]; //deadzone NE
		guidanceClass.Model_GS_U.Control_Param[7]= (float) param[7]; //deadzone D
		guidanceClass.Model_GS_U.Control_Param[8]= (float) param[8]; //gain integral D

		//Delta-time, needed by the integral
		guidanceClass.Model_GS_U.dt = ((float)1.0f)/rate;

		//Time
		guidanceClass.Model_GS_U.Time=inputPos.Timestamp;

		//Integral trigger when we are ready to takeoff
		guidanceClass.Model_GS_U.Trigger = trigger_;       //if safety is 0, means ODROID on, so trigger

		//DEBUG Test for AMSL offset or not
		guidanceClass.Model_GS_U.Test = (float) debugParam;

		//DEBUG
		//if (counter_print >= 10){
			ROS_INFO("Loaded Matlab Pos: [Lat:%i, Long:%i, Alt:%i, Yaw:%f, SafeOn:%s,(Time:%u)]",         //TODO uncomment
					guidanceClass.Model_GS_U.Actual_Pos[0], guidanceClass.Model_GS_U.Actual_Pos[1],
					guidanceClass.Model_GS_U.Actual_Pos[2], guidanceClass.Model_GS_U.Actual_Yaw,
					safety_ ? "true" : "false", inputPos.Timestamp);
			ROS_INFO("Loaded Matlab Ref: [Lat:%i, Long:%i, Alt:%i, Yaw:%f]",
					guidanceClass.Model_GS_U.Reference_Pos[0], guidanceClass.Model_GS_U.Reference_Pos[1],
					guidanceClass.Model_GS_U.Reference_Pos[2], guidanceClass.Model_GS_U.Reference_Yaw);
		//}
	}

	void getOutput(const Model_GSModelClass &guidanceClass, guidance_node_amsl::Directive *output) {

		output->vxBody=guidanceClass.Model_GS_Y.FakeDirective[0];
		output->vyBody=guidanceClass.Model_GS_Y.FakeDirective[1];
		output->vzBody=guidanceClass.Model_GS_Y.FakeDirective[2];
		output->yawRate=guidanceClass.Model_GS_Y.FakeDirective[3];
		
		if (counter_print >= 10){
			ROS_INFO("Calc Matlab Direc: [vx:%f, vy:%f, vz:%f, yawRate:%f]",			//TODO uncomment
							guidanceClass.Model_GS_Y.FakeDirective[0],guidanceClass.Model_GS_Y.FakeDirective[1],
							guidanceClass.Model_GS_Y.FakeDirective[2],guidanceClass.Model_GS_Y.FakeDirective[3]);
		}
	}
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "guidance_node_amsl");
	ros::NodeHandle node;

	counter_print = 0;
	GuidanceNodeClass guidanceNode(node);
	guidanceNode.run();
	return 0;
}
