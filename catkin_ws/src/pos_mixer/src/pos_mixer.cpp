#include <ros/ros.h>

//#include <mms/Sys_status.h>
#include <mms/Cmd.h>
#include <mavros/Global_position_int.h>
#include <guidance_node_amsl/Position_nav.h>
#include <mavros/Sonar.h>

class PosmixerNodeClass
{

public:
	PosmixerNodeClass(ros::NodeHandle& node)
{

		n_=node;

		//subscribers
		subFromCmd_=n_.subscribe("/command", 10, &PosmixerNodeClass::readCmdMessage,this);
		subFromPosition_=n_.subscribe("/global_position_int", 10, &PosmixerNodeClass::readPositionMessage,this);
		subFromSonar_ = n_.subscribe("/sonar", 10, &PosmixerNodeClass::readSonarMessage,this);
		// subFromSysStatus_=n_.subscribe("/system_status", 10, &PosmixerNodeClass::readSysStatusMessage,this);

		// publishers
		pubToPosNav_ = n_.advertise<guidance_node_amsl::Position_nav>("/position_nav", 10);
		// pubToSysStatus_=n_.advertise<mms::Sys_status>("/system_status", 10);

}
	void readPositionMessage(const mavros::Global_position_int::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: POSITION_RECEIVED");
		inputPos_.lat = msg->lat;
		inputPos_.lon = msg->lon;
		inputPos_.relative_alt = msg->relative_alt;
		inputPos_.alt = msg->alt;
		inputPos_.hdg = msg->hdg;     
		inputPos_.time_boot_ms = msg->time_boot_ms;
	}
	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	/*void readSysStatusMessage(const mms::Sys_status::ConstPtr& msg)
	{
		inputSysStatus_.armed=msg->armed;
		inputSysStatus_.voltage_battery=msg->voltage_battery;
		inputSysStatus_.valid_ref_frame=msg->valid_ref_frame;
		ROS_INFO("POSMIXER: SYS_STATE RECEIVED");
	}*/
	void readCmdMessage(const mms::Cmd::ConstPtr& msg)
	{
		ROS_INFO("POSMIXER: CMD_RECEIVED");
		inputCmd_.command = msg -> command;
		inputCmd_.param1  = msg -> param1;
		inputCmd_.param2  = msg -> param2;
		inputCmd_.param3  = msg -> param3;
		inputCmd_.param4  = msg -> param4;
		inputCmd_.param5  = msg -> param5;
		inputCmd_.param6  = msg -> param6;
		inputCmd_.param7  = msg -> param7;
		inputCmd_.frame  = msg -> frame;

		if(inputCmd_.command == 16 || inputCmd_.command == 21|| inputCmd_.command == 22)
		{// 16 = WAYPOINT; 22 = TAKEOFF; 21 = LAND
			if(inputCmd_.frame == 11 && inputSonar_.distance > 0)
			{
				ref_frame = 11;
				ROS_INFO("POSMIXER: TERRAIN_ALT SET");
			}
			else
			{
				ref_frame = 6;
				ROS_INFO("POSMIXER: RELATIVE_ALT SET");
			}
		}

	}
	void Posmixer_Handle()
	{

		if(ref_frame == 11)
		{
				outputPosNav_.Latitude =inputPos_.lat ;
				outputPosNav_.Longitude =inputPos_.lon ;
				outputPosNav_.Timestamp =inputPos_.time_boot_ms;
				outputPosNav_.Altitude = inputSonar_ .distance;
				outputPosNav_.YawAngle = inputPos_.hdg*3.14/180/100;
		}
		else  
		{
			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Altitude =inputPos_.relative_alt;  
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
			outputPosNav_.YawAngle = inputPos_.hdg*3.14/180/100;
		}
		pubToPosNav_.publish(outputPosNav_);
		//outputSysStatus_.valid_ref_frame = ref_frame;
		//pubToSysStatus_.publish(outputSysStatus_);
		//ROS_INFO("POSMIXER: VALID_MAV_FRAME = %d", ref_frame);
	}


	void run()
	{
		ros::Rate loop_rate(rate);

		while (ros::ok())
		{
			ROS_INFO_ONCE("POSMIXER: RUNNING");
			ROS_INFO_ONCE("POSMIXER: RELATIVE_ALT SET");

			Posmixer_Handle();
			ros::spinOnce();

			loop_rate.sleep();
		}
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	// subscriber
	ros::Subscriber subFromPosition_;
	ros::Subscriber subFromSonar_;
	// ros::Subscriber subFromSysStatus_;
	ros::Subscriber subFromCmd_;

	mavros::Global_position_int inputPos_;
	// mms::Sys_status inputSysStatus_;
	mavros::Sonar inputSonar_;
	mms::Cmd inputCmd_;

	// publisher
	ros::Publisher pubToPosNav_;
	guidance_node_amsl::Position_nav outputPosNav_;

	//ros::Publisher pubToSysStatus_;
	//mms::Sys_status outputSysStatus_;

	int rate = 10;
	int ref_frame = 6;

	// private:
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Posmixer");
	ros::NodeHandle node;

	PosmixerNodeClass PosmixerNode(node);

	PosmixerNode.run();
	return 0;
}
