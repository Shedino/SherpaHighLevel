#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>

#include <mavros/Global_position_int.h>
#include "camera_handler_sherpa/Camera.h"
#include <mms_msgs/Cmd.h>
#include <mms_msgs/Ack_mission.h>

//#include <exiv2/exiv2.hpp>

//static const std::string OPENCV_WINDOW = "Image window";
static bool take_video = false;
static bool taking_photos = false;
static char dest_video[50];
static int video_count = 0; 
static int photo_taken_counter = 0;
static int FPS = 8;         //TODO not hardcoded
static int image_count = 0;
cv::VideoWriter video;
ros::Publisher camera_pub;
//ros::Publisher ack_pub;
ros::Publisher mission_pub;
ros::Subscriber camera_sub;
ros::Subscriber command_sub;
ros::Subscriber geopose_sub;
camera_handler_sherpa::Camera camera_topic;
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
	//image_transport::Publisher image_pub_;
  
	public:
	CameraHandler(): it_(nh_){
		// Subscribe to input video feed and commands
		image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, &CameraHandler::imageCb, this);
		command_sub = nh_.subscribe("/sent_command", 10, &CameraHandler::command_handler, this);
		geopose_sub = nh_.subscribe("/global_position_int", 10, &CameraHandler::geopose_handler, this);

		//image_pub_ = it_.advertise("/camera_handler/output_video", 1);      //only if we have to modify images and publish again
		camera_pub = nh_.advertise<camera_handler_sherpa::Camera>("/camera_published", 20);
		//ack_pub = nh_.advertise<mms_msgs::Ack_cmd>("/ack_cmd", 10);
		mission_pub = nh_.advertise<mms_msgs::Ack_mission>("/ack_mission", 10);

		//cv::namedWindow(OPENCV_WINDOW);
		_delay_seconds = 0;            
		_N_photo = 0;					
		_counter_frames = 0;              
	}

	void geopose_handler(const mavros::Global_position_int::ConstPtr& msg)
	{
		_lat = msg->lat;
		_lon = msg->lon;
	}
  
	void command_handler(const mms_msgs::Cmd::ConstPtr& msg){
		if (msg->command == 2000){      //MAV_CMD_IMAGE_START_CAPTURE = 2000
			if (!taking_photos){                                
				seq_photo = msg->seq;
				taking_photos = true;
				photo_taken_counter = 0;
				_delay_seconds = msg->param1;
				_N_photo = msg->param2;      //if this is 0 unlimited photos
				_compare_frames = (int)(_delay_seconds * FPS);       //how many frames to wait to have the desired delay
				ROS_INFO("CAMERA HANDLER: compare_frames: %d - delay: %f - FPS: %d", _compare_frames, _delay_seconds, FPS);
				//misison ack
				outputAckMission_.seq = seq_photo;
				outputAckMission_.mav_mission_accepted = true;
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
			} else {                        //mission not accepted because already taking photos
				//misison ack
				outputAckMission_.seq = seq_photo;
				outputAckMission_.mav_mission_accepted = false;
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
			}
		} else if (msg->command == 2500){	//MAV_CMD_VIDEO_START_CAPTURE = 2500
			seq_video = msg->seq;
			if (!take_video){
				seq_video = msg->seq;
				//misison ack
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = true;
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
				startVideo();
			} else {
				outputAckMission_.seq = seq_video;
				outputAckMission_.mav_mission_accepted = false;  //mission not accepted because there was already a video running
				outputAckMission_.mission_item_reached = false;
				mission_pub.publish(outputAckMission_);
			}
		} else if (msg->command == 2001){		//MAV_CMD_IMAGE_STOP_CAPTURE = 2001         
			if (taking_photos){
				//mission ack because photo mission is finished
				taking_photos = false;
				outputAckMission_.seq = seq_photo;
				outputAckMission_.mission_item_reached = true; //item reached: take picture
				outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
				mission_pub.publish(outputAckMission_);
			} else {
				//ACK?
			}
		} else if (msg->command == 2501){		//MAV_CMD_VIDEO_STOP_CAPTURE = 2501
			seq_video = msg->seq;
			if (take_video){
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
		dst << src.rdbuf();		//move video from /Video_Mission to /Photo_Mission
		
		camera_topic.taken_photo = false;
		camera_topic.taken_video = true;
		camera_topic.N_video_taken = video_count;
		camera_topic.path_video = dest_video;
		camera_pub.publish(camera_topic);    //publish that a video is taken
		video_count++;
		//remove("C:\\Temp\\somefile.txt");
		
		//mission ack
		outputAckMission_.seq = seq_video;
		outputAckMission_.mission_item_reached = true; //item reached: end stop recording
		outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
		mission_pub.publish(outputAckMission_);
	}

	~CameraHandler()
	{
		//cv::destroyWindow(OPENCV_WINDOW);
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg){
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
		_counter_frames++;  
		if (_counter_frames >= 10000) _counter_frames = 0;		//check for overflow, counter reset
		
		if (taking_photos && _counter_frames>=_compare_frames){           //we are taking pictures and we passed the delay (rounded)
			_counter_frames = 0;
			ROS_INFO("CAMERA HANDLER: taking picture %d", photo_taken_counter+1);
			// Save Image
			static char dest_img[50];                        
			sprintf(dest_img,"/home/odroid/Photo_Mission/image_%d_%.7f_%.7f.jpg",image_count,_lat,_lon);
			ROS_ASSERT( cv::imwrite(dest_img,  cv_ptr->image));
			photo_taken_counter++;

			camera_topic.taken_photo = true;
			camera_topic.taken_video = false;
			camera_topic.N_photo_taken = image_count;
			camera_topic.path_photo = dest_img;
			camera_pub.publish(camera_topic);
			image_count++;
		
			if (photo_taken_counter == _N_photo){   //in case _N_photo = 0 this will never enter so unlimited photos
				//mission ack because photo mission is finished
				taking_photos = false;
				outputAckMission_.seq = seq_photo;
				outputAckMission_.mission_item_reached = true; //item reached: take picture
				outputAckMission_.mav_mission_accepted = false;   //need to send only item reached
				mission_pub.publish(outputAckMission_);
			}

			//Metadata
			//exifData["Exif.DarioScemo"] = "True";                     // Ascii
			//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(dest_img);
			//assert(image.get() != 0);
			//image->setExifData(exifData);
			//image->writeMetadata();
		}

		if (take_video) video << cv_ptr->image;
	}
	private:                     
	float _delay_seconds;
	int _N_photo;
	int _counter_frames;           
	int _compare_frames;
	double _lat;
	double _lon;
};

int main(int argc, char** argv)
{
	ros::init(argc, argv, "camera_handler_sherpa");
	CameraHandler ic;
	ros::spin();
	return 0;
}
