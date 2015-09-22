#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "camera_handler_SHERPA/Camera.h"
#include <mms_msgs/Cmd.h>
#include <mms_msgs/Ack_mission.h>

//#include <exiv2/exiv2.hpp>

static const std::string OPENCV_WINDOW = "Image window";
static bool take_video = false;
static char dest_video[50];
static int video_count = 0; 
cv::VideoWriter video;
ros::Publisher camera_pub;
//ros::Publisher ack_pub;
ros::Publisher mission_pub;
ros::Subscriber camera_sub;
ros::Subscriber command_sub;
camera_handler_SHERPA::Camera camera_topic;
//mms::Ack_cmd outputAckCmd_;
mms_msgs::Ack_mission outputAckMission_;
uint16_t seq_photo;
uint16_t seq_video;
//Exiv2::ExifData exifData;	//Metadata

class CameraHandler
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
  public:
  CameraHandler()
    : it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, 
      &CameraHandler::imageCb, this);
	//camera_sub = nh_.subscribe("/camera_trigger", 10, &CameraHandler::trigger_image_video, this);    //TODO not used anymore, remove
	command_sub = nh_.subscribe("/command", 10, &CameraHandler::command_handler, this);

    image_pub_ = it_.advertise("/camera_handler/output_video", 1);
	camera_pub = nh_.advertise<camera_handler_SHERPA::Camera>("/camera_trigger", 20);
	//ack_pub = nh_.advertise<mms_msgs::Ack_cmd>("/ack_cmd", 10);
	mission_pub = nh_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);

    //cv::namedWindow(OPENCV_WINDOW);
  }
  
	void command_handler(const mms_msgs::Cmd::ConstPtr& msg){
		if (msg->command == 2000){      //MAV_CMD_IMAGE_START_CAPTURE = 2000
			camera_topic.take_pic = true;
			seq_photo = msg->seq;
			//misison ack
			outputAckMission_.seq = seq_photo;
			outputAckMission_.mav_mission_accepted = true;
			outputAckMission_.mission_item_reached = false;
			mission_pub.publish(outputAckMission_);
			//cmd ack (will be made on mission loader node)
			/*outputAckCmd_.command = msg->command;
			outputAckCmd_.mav_command_accepted = true;
			ack_pub.publish(outputAckCmd_);*/
		} else if (msg->command == 2500){	//MAV_CMD_VIDEO_START_CAPTURE = 2500
			camera_topic.take_vid = true;
			seq_video = msg->seq;
			if (!take_video){
				seq_video = msg->seq;
				//misison ack
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = true;
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
				//cmd ack
				/*outputAckCmd_.command = msg->command;
				outputAckCmd_.mav_command_accepted = true;
				ack_pub.publish(outputAckCmd_);*/
				startVideo();
			} else {
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = false;  //mission not accepted because there was already a video running
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
			}
		} else if (msg->command == 2501){		//MAV_CMD_VIDEO_STOP_CAPTURE = 2501
			camera_topic.take_vid = false;
			seq_video = msg->seq;
			if (take_video){
				/*outputAckCmd_.command = msg->command;
				outputAckCmd_.mav_command_accepted = true;
				ack_pub.publish(outputAckCmd_);*/
				//mission ack
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = true;
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
				stopVideo();
			} else {
				//mission ack
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = false; //mission not accepted because there wasn't a video running
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
			}
		}
	}

	/*void trigger_image_video(const camera_handler_SHERPA::Camera::ConstPtr& msg){
		camera_topic.take_pic = msg->take_pic;
		camera_topic.take_vid = msg->take_vid;
		if (msg->take_vid && !take_video) startVideo();
		else if (!msg->take_vid && take_video) stopVideo();
	}*/

	void startVideo()
	{
		ROS_INFO("CAMERA HANDLER: starting video");
		sprintf(dest_video,"/home/odroid/Video_Mission/video_%d.mpg",video_count);
		video.open(dest_video, CV_FOURCC('P','I','M','1'), 20, cv::Size(800,600), true);             //TODO take size and fps dynamicly
		//video.open(dest_video, CV_FOURCC('M','J','P','G'), 20, cv::Size(800,600), true);             //TODO take size and fps dynamicly
		take_video = true;
		//mission ack
		outputAckMission_.seq = seq_video;
		outputAckMission_.mission_item_reached = true; //item reached: start recording
		outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
		mission_pub.publish(outputAckMission_);
		
		//outputAckCmd_.mission_item_reached = true;
		//outputAckCmd_.mav_mission_accepted = false;
		//outputAckCmd_.mav_cmd_id = 2500;         //MAV_CMD_VIDEO_START_CAPTURE
		//ack_pub.publish(outputAckCmd_);
	}
	void stopVideo()
	{
		ROS_INFO("CAMERA HANDLER: stopping video");
		take_video = false;
		
		std::ifstream  src(dest_video, std::ios::binary);
		sprintf(dest_video,"/home/odroid/Photo_Mission/video_%d.mpg",video_count);
		std::ofstream  dst(dest_video,   std::ios::binary);
		dst << src.rdbuf();
		
		video_count++;
		//remove("C:\\Temp\\somefile.txt");
		
		//mission ack
		outputAckMission_.seq = seq_video;
		outputAckMission_.mission_item_reached = true; //item reached: end stop recording
		outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
		mission_pub.publish(outputAckMission_);
		//outputAckCmd_.mission_item_reached = true;
		//outputAckCmd_.mav_mission_accepted = false;
		//outputAckCmd_.mav_cmd_id = 2501;         //MAV_CMD_VIDEO_STOP_CAPTURE
		//ack_pub.publish(outputAckCmd_);
	}

  ~CameraHandler()
  {
    //cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
		//ROS_INFO("Received image");
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

		
		if (camera_topic.take_pic){
			ROS_INFO("CAMERA HANDLER: taking picture");
			// Save Image
			static int image_count = 0;
			static char dest_img[50];                        
			sprintf(dest_img,"/home/odroid/Photo_Mission/image_%d.jpg",image_count);
			ROS_ASSERT( cv::imwrite(dest_img,  cv_ptr->image));
			image_count++;
			camera_topic.take_pic = false;
			camera_pub.publish(camera_topic);
			
			//mission ack
			outputAckMission_.seq = seq_photo;
			outputAckMission_.mission_item_reached = true; //item reached: take picture
			outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
			mission_pub.publish(outputAckMission_);
			
			//outputAckCmd_.mission_item_reached = true;
			//outputAckCmd_.mav_mission_accepted = false;
			//outputAckCmd_.mav_cmd_id = 2000;         //MAV_CMD_IMAGE_START_CAPTURE
			//ack_pub.publish(outputAckCmd_);

			//Metadata
			//exifData["Exif.DarioScemo"] = "True";                     // Ascii
			//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(dest_img);
		  //assert(image.get() != 0);
		  //image->setExifData(exifData);
		  //image->writeMetadata();
		}

		if (take_video) video << cv_ptr->image;
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "camera_handler_SHERPA");
  CameraHandler ic;
  ros::spin();
  return 0;
}
