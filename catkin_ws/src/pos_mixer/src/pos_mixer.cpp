#include <ros/ros.h>

#include <mms/Sys_status.h>
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
                subFromPosition_=n_.subscribe("/global_position_int", 10, &PosmixerNodeClass::readPositionMessage,this);
                subFromSonar_ = n_.subscribe("/sonar", 10, &PosmixerNodeClass::readSonarMessage,this);
                subFromSysStatus_=n_.subscribe("/system_status", 10, &PosmixerNodeClass::readSysStatusMessage,this);

		// publishers
		pubToPosNav_ = n_.advertise<guidance_node_amsl::Position_nav>("/position_nav", 10);

	}
	void readPositionMessage(const mavros::Global_position_int::ConstPtr& msg)
	{
                ROS_INFO("POSMIXER: POSITION_RECEIVED");
		inputPos_.lat = msg->lat;
		inputPos_.lon = msg->lon;
		inputPos_.relative_alt = msg->relative_alt;
		inputPos_.alt = msg->alt;
		inputPos_.hdg = msg->hdg;     
		inputPos_.time_boot_ms = msg->time_boot_ms;
	}
	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		ROS_INFO("POSMIXER: SONAR_RECEIVED");
		inputSonar_.distance = msg -> distance;
	}

	void readSysStatusMessage(const mms::Sys_status::ConstPtr& msg)
	{
		inputSysStatus_.armed=msg->armed;
		inputSysStatus_.voltage_battery=msg->voltage_battery;
                inputSysStatus_.valid_ref_frame=msg->valid_ref_frame;
		ROS_INFO("POSMIXER: SYS_STATE RECEIVED");
	}
	
	void Posmixer_Handle()
	{

		/*if (inputSysStatus_.valid_ref_frame == 6)    
		{

			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Altitude =inputPos_.relative_alt;  
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
			pubToPosNav_.publish(outputPosNav_);
			ROS_INFO("POSMIXER: RELATIVE_ALT SET");
		}
                else if(inputSysStatus_.valid_ref_frame == 11 && inputSonar_.distance > 0)
		{
			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
                        outputPosNav_.Altitude = inputSonar_ .distance;
			pubToPosNav_.publish(outputPosNav_);
			ROS_INFO("POSMIXER: TERRAIN_ALT SET");
		}
		else  
		{
			ROS_INFO("POSMIXER: MISSION_ITEM_NOT_ACCEPTED");
		}*/
                if(inputSysStatus_.valid_ref_frame == 11 && inputSonar_.distance > 0)
		{
			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
                        outputPosNav_.Altitude = inputSonar_ .distance;
			pubToPosNav_.publish(outputPosNav_);
			ROS_INFO("POSMIXER: TERRAIN_ALT SET");
		}
		else  
		{
			outputPosNav_.Latitude =inputPos_.lat ;
			outputPosNav_.Longitude =inputPos_.lon ;
			outputPosNav_.Altitude =inputPos_.relative_alt;  
			outputPosNav_.Timestamp =inputPos_.time_boot_ms;
			pubToPosNav_.publish(outputPosNav_);
			ROS_INFO("POSMIXER: RELATIVE_ALT SET");
		}








         }


	void run()
        {
		ros::Rate loop_rate(rate);

		while (ros::ok())
		{
			// ROS_INFO("MMS running");

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
	ros::Subscriber subFromSysStatus_;
	mavros::Global_position_int inputPos_;
	mms::Sys_status inputSysStatus_;
	mavros::Sonar inputSonar_;
        // publisher
	ros::Publisher pubToPosNav_;
	guidance_node_amsl::Position_nav outputPosNav_;

        int rate = 10;

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
