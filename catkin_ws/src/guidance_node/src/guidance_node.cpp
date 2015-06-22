#include "ros/ros.h"
#include <vector>

#include "guidance_node/Directive.h"
#include "guidance_node/Position.h"
#include "guidance_node/Reference.h"


#include "Model_GS.h" //qui devo stare attento a come si chiama il file.h

class GuidanceNodeClass {
public:
	GuidanceNodeClass(ros::NodeHandle& node){

		n_=node;

		param_ = std::vector<double>(9);

		//frequency
		node.param("guidance_node/frequency",rate,10);

		//control param
		node.param("guidance_node/param/sat_xy", param_[0], 3.0);
		node.param("guidance_node/param/sat_z", param_[1], 1.5);
		node.param("guidance_node/param/sat_yaw", param_[2], 3.14);
		node.param("guidance_node/param/gain_NE", param_[3], 0.06);
		node.param("guidance_node/param/gain_D", param_[4], 0.06);
		node.param("guidance_node/param/gain_yaw", param_[5], 0.3);
		node.param("guidance_node/param/deadzone_NE", param_[6], 1.5);
		node.param("guidance_node/param/deadzone_D", param_[7], 0.2);
		node.param("guidance_node/param/gain_integralDown", param_[8],0.025);

		//subscribers and publishers
		subFromReference_=n_.subscribe("/reference",10, &GuidanceNodeClass::readReferenceMessage, this);
		subFromPosition_=n_.subscribe("/position", 10, &GuidanceNodeClass::readPositionMessage,this);
		pub_=n_.advertise<guidance_node::Directive>("/directive", 10);

		//Initializing inputRef_
		node.param<int>("guidance_node/ref/latitude_ref", inputRef_.Latitude, 0);
		node.param<int>("guidance_node/ref/longitude_ref", inputRef_.Longitude, 0);
		node.param<int>("guidance_node/ref/altitude_ref", inputRef_.AltitudeRelative, 1000);
		double temp;
		node.param("guidance_node/ref/yaw_ref",  temp, 0.0);
		inputRef_.Yawangle= (float) temp;
	}

	void readReferenceMessage(const guidance_node::Reference::ConstPtr& msg)
	{
		/*
		 * uso il campo safety per discernere se il messaggio che mi arriva è tipo relativo o assoluto
		 * Al momento è buono per testing, più avanti è da consolidare
		 */
		if(msg->Mode==100){
			//relative
			inputRef_.Latitude=inputPos_.Latitude + msg->Latitude;
			inputRef_.Longitude=inputPos_.Longitude + msg->Longitude;
			inputRef_.AltitudeRelative= inputPos_.AltitudeRelative + msg->AltitudeRelative;
			inputRef_.Yawangle= inputPos_.Yawangle + msg->Yawangle;
		} else {
			//absolute
			inputRef_.Latitude=msg->Latitude;
			inputRef_.Longitude=msg->Longitude;
			inputRef_.AltitudeRelative=msg->AltitudeRelative;
			inputRef_.Yawangle=msg->Yawangle;
		}
	}

	void readPositionMessage(const guidance_node::Position::ConstPtr& msg)
	{
		inputPos_.Latitude=msg->Latitude;
		inputPos_.Longitude=msg->Longitude;
		inputPos_.AltitudeRelative=msg->AltitudeRelative;
		inputPos_.Yawangle=msg->Yawangle;
		inputPos_.Timestamp=msg->Timestamp;
		inputPos_.Safety = msg->Safety;
	}

	void process(Model_GSModelClass &guidanceClass)
	{

		guidance_node::Reference inputRef = inputRef_;
		guidance_node::Position inputPos = inputPos_;

		guidance_node::Directive output;

		initializeParameters(guidanceClass, inputPos, inputRef, param_);//initialize input parameters;
		guidanceClass.step(); //step the function
		getOutput(guidanceClass, &output); //get the output

		pub_.publish(output);

		ros::spinOnce();
	}

	void run() {
		ros::Rate loop_rate(rate);
		Model_GSModelClass guidanceClass;
		guidanceClass.initialize();
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

	guidance_node::Reference inputRef_;
	guidance_node::Position inputPos_;

	ros::Publisher pub_;

	int rate;

	std::vector<double> param_;

private:

	void initializeParameters(Model_GSModelClass &guidanceClass,
			const guidance_node::Position &inputPos,
			const guidance_node::Reference &inputRef,
			const std::vector<double> &param) {

		guidanceClass.Model_GS_U.Actual_Pos[0]=inputPos.Latitude;
		guidanceClass.Model_GS_U.Actual_Pos[1]=inputPos.Longitude;
		guidanceClass.Model_GS_U.Actual_Pos[2]=inputPos.AltitudeRelative;
		guidanceClass.Model_GS_U.Actual_Yaw=inputPos.Yawangle;

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

		//Safety trigger
		guidanceClass.Model_GS_U.Trigger = (inputPos.Safety == 0) ? true : false;

		//DEBUG
		ROS_INFO("Loaded Matlab Pos: [Lat:%i, Long:%i, Alt:%i, Yaw:%f, SafeOn:%s,(Time:%u)]",
				guidanceClass.Model_GS_U.Actual_Pos[0], guidanceClass.Model_GS_U.Actual_Pos[1],
				guidanceClass.Model_GS_U.Actual_Pos[2], guidanceClass.Model_GS_U.Actual_Yaw,
				!guidanceClass.Model_GS_U.Trigger ? "true" : "false", inputPos.Timestamp);
		ROS_INFO("Loaded Matlab Ref: [Lat:%i, Long:%i, Alt:%i, Yaw:%f]",
				guidanceClass.Model_GS_U.Reference_Pos[0], guidanceClass.Model_GS_U.Reference_Pos[1],
				guidanceClass.Model_GS_U.Reference_Pos[2], guidanceClass.Model_GS_U.Reference_Yaw);
	}

	void getOutput(const Model_GSModelClass &guidanceClass, guidance_node::Directive *output) {

		output->vxBody=guidanceClass.Model_GS_Y.FakeDirective[0];
		output->vyBody=guidanceClass.Model_GS_Y.FakeDirective[1];
		output->vzBody=guidanceClass.Model_GS_Y.FakeDirective[2];
		output->yawRate=guidanceClass.Model_GS_Y.FakeDirective[3];

		ROS_INFO("Calc Matlab Direc: [vx:%f, vy:%f, vz:%f, yawRate:%f]",
		guidanceClass.Model_GS_Y.FakeDirective[0],guidanceClass.Model_GS_Y.FakeDirective[1],
		guidanceClass.Model_GS_Y.FakeDirective[2],guidanceClass.Model_GS_Y.FakeDirective[3]);
	}
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "guidance_node");
	ros::NodeHandle node;

	GuidanceNodeClass guidanceNode(node);
	guidanceNode.run();
	return 0;
}
