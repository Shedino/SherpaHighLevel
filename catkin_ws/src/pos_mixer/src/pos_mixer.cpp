#include <ros/ros.h>

#include <mavros/Global_position_int.h>
#include <guidance_node_amsl/Position_nav.h>
#include <mavros/Sonar.h>
#include <frame/Ref_system.h>

class PosmixerNodeClass
{

public:
	PosmixerNodeClass(ros::NodeHandle& node)
	{

		n_=node;

		//subscribers
		subFromPosition_=n_.subscribe("/global_position_int", 10, &PosmixerNodeClass::readPositionMessage,this);
		subFromSonar_ = n_.subscribe("/sonar", 10, &PosmixerNodeClass::readSonarMessage,this);
		subFromRefSystem_=n_.subscribe("/ref_system", 10, &PosmixerNodeClass::readFrameMessage,this);

		// publishers
		pubToPosNav_ = n_.advertise<guidance_node_amsl::Position_nav>("/position_nav", 10);

		// initialization
		inputRefSystem_.actual_ref_frame = 6;
		new_frame = false;
		old_frame = 0;
		rate = 10;
		use_global_altitude = true; //true-->use global altitude from GPS, relative from baro otherwise

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
		//Posmixer_Handle();
	}

	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		// ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
		//Posmixer_Handle();
	}

	void readFrameMessage(const frame::Ref_system::ConstPtr& msg)
	{
		inputRefSystem_.actual_ref_frame=msg->actual_ref_frame;
		inputRefSystem_.target_ref_frame=msg->target_ref_frame;
		// ROS_INFO("POS_MIXER: Ref_system received");
		// new_frame = true;
		//Posmixer_Handle();
	}

	void Posmixer_Handle()
	{

		if(inputRefSystem_.actual_ref_frame == 11)
		{
			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
			outputPosNav_.Altitude = inputSonar_.distance;
			outputPosNav_.YawAngle = inputPos_.hdg*3.14/180/100;
			outputPosNav_.frame =inputRefSystem_.actual_ref_frame;
		}
		else if (inputRefSystem_.actual_ref_frame == 6)
		{
			outputPosNav_.Latitude = inputPos_.lat;
			outputPosNav_.Longitude = inputPos_.lon;
			outputPosNav_.Altitude = inputPos_.alt;
			if (use_global_altitude) outputPosNav_.Altitude = inputPos_.alt;     //use absolute alt
			else outputPosNav_.Altitude = inputPos_.relative_alt + 785000;  //TODO check using champoluc altitude
			outputPosNav_.Timestamp = inputPos_.time_boot_ms;
			outputPosNav_.YawAngle = inputPos_.hdg*3.14/180/100;
			outputPosNav_.frame = inputRefSystem_.actual_ref_frame;
		}

		if (old_frame != inputRefSystem_.actual_ref_frame) // CHECK IF THE FRAME IS CHANGED
		{
			new_frame = true;
		}
		else
		{
			new_frame = false;
		}

		if (new_frame == true)
		{
			new_frame = false;
			if (inputRefSystem_.actual_ref_frame  == 11)
				ROS_INFO("POS_MIXER: SONAR USED");
			if (inputRefSystem_.actual_ref_frame  == 6)
				ROS_INFO("POS_MIXER: BARO USED");
		}
		pubToPosNav_.publish(outputPosNav_);
		old_frame = inputRefSystem_.actual_ref_frame;
	}


	void run()
	{
		//ros::spin();
		ros::Rate loop_rate(rate);
		while (ros::ok())
		{
			ROS_INFO_ONCE("POS_MIXER: RUNNING");
			// ROS_INFO_ONCE("POS_MIXER: RELATIVE_ALT SET");

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
	ros::Subscriber subFromRefSystem_;

	mavros::Global_position_int inputPos_;
	mavros::Sonar inputSonar_;
	frame::Ref_system inputRefSystem_;

	// publisher
	ros::Publisher pubToPosNav_;
	guidance_node_amsl::Position_nav outputPosNav_;

	int rate;
	bool new_frame;
	int old_frame;
	bool use_global_altitude;

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Posmixer");
	ros::NodeHandle node;

	PosmixerNodeClass PosmixerNode(node);

	PosmixerNode.run();
	return 0;
}
