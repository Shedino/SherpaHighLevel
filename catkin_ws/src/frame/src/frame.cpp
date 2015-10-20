#include <ros/ros.h>

#include <mavros/Sonar.h>
#include <mms_msgs/MMS_status.h>
#include <frame/Ref_system.h>

class FrameNodeClass
{

public:

//	int rate = 10;
	int actual_frame = 6;
	int old_frame = 0;
	int old_target_frame = 0;
	bool new_frame = false;

	FrameNodeClass(ros::NodeHandle& node)
{

		n_=node;

		//subscribers
		subFromSonar_ = n_.subscribe("/sonar", 10, &FrameNodeClass::readSonarMessage,this);
		subFromMMSStatus_=n_.subscribe("/mms_status", 10, &FrameNodeClass::readMMSStatusMessage,this);

		// publishers
		pubToRefSystem_=n_.advertise<frame::Ref_system>("/ref_system", 10);
}


	void readSonarMessage(const mavros::Sonar::ConstPtr& msg)
	{
		inputSonar_.distance = msg -> distance;
		// ROS_INFO("FRAME: SONAR DISTANCE = %d", inputSonar_.distance);
		Frame_Handle();
	}

	void readMMSStatusMessage(const mms_msgs::MMS_status::ConstPtr& msg)
	{
		inputMMSStatus_.target_ref_frame=msg->target_ref_frame;
		// ROS_INFO("FRAME: MMS_STATE RECEIVED");
		Frame_Handle();
	}


	void Frame_Handle()
	{
		if (inputMMSStatus_.target_ref_frame == 6)  // if the target is barometer
			actual_frame = 6; // -> change to barometer
		if (inputMMSStatus_.target_ref_frame == 11 && actual_frame == 6) // if the target is sonar and the actual is barometer
		{
			if (inputSonar_.distance > 0) // the sonar is available
				actual_frame = 11;  // -> change to sonar
			else // and the sonar is NOT available
				actual_frame = 6; // -> change to barometer
		}
		if (inputMMSStatus_.target_ref_frame == 11 && actual_frame == 11) // if the target is sonar and the actual is sonar
		{
			if (inputSonar_.distance == 0 || inputSonar_.distance == -1 ) // the sonar is NOT available
				{
				actual_frame = 6; // -> change to barometer
				ROS_INFO("FRAME: WARNING! SONAR NOT AVAILABLE!");
				}

		}

		if (old_frame != actual_frame || old_target_frame != inputMMSStatus_.target_ref_frame) // CHECK IF THE FRAME IS CHANGED
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
			if (actual_frame == 6)
			{
				ROS_INFO("FRAME: ACTUAL SET TO BAROMETER");
			}
			else
			{
				ROS_INFO("FRAME: ACTUAL SET TO SONAR");
			}

			if (inputMMSStatus_.target_ref_frame == 6 || inputMMSStatus_.target_ref_frame == 11)
			{
				outputRefSystem_.actual_ref_frame = actual_frame;
				outputRefSystem_.target_ref_frame = inputMMSStatus_.target_ref_frame;
				pubToRefSystem_.publish(outputRefSystem_);
				if (inputMMSStatus_.target_ref_frame == 6)
				{
					ROS_INFO("FRAME: TARGET SET TO BAROMETER");
				}
				else
				{
					ROS_INFO("FRAME: TARGET SET TO SONAR");
				}
			}
			else
			{
				ROS_INFO("FRAME: WRONG TARGET FRAME");
			}
		}
		old_frame = actual_frame;
		old_target_frame = inputMMSStatus_.target_ref_frame;
	}


	void run()
	{
		// ros::Rate loop_rate(rate);

		ros::spin();
		/*while (ros::ok())
		{
			ROS_INFO_ONCE("FRAME: RUNNING");
			// ROS_INFO_ONCE("FRAME: RELATIVE_ALT SET");

			// Frame_Handle();
			ros::spinOnce();

			// loop_rate.sleep();
		}*/
	}

protected:
	/*state here*/
	ros::NodeHandle n_;

	// subscriber
	ros::Subscriber subFromSonar_;
	ros::Subscriber subFromMMSStatus_;

	mms_msgs::MMS_status inputMMSStatus_;
	mavros::Sonar inputSonar_;

	// publisher
	ros::Publisher pubToRefSystem_;

	frame::Ref_system outputRefSystem_;

//	int rate = 10;
/*	int actual_frame = 6;
	int old_frame = 0;
	int old_target_frame = 0;
	bool new_frame = false;*/

	// private:
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "Frame");
	ros::NodeHandle node;

	FrameNodeClass FrameNode(node);

	FrameNode.run();
	return 0;
}
