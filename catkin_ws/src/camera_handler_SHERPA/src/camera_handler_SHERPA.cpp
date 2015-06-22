#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera_handler_SHERPA/Camera.h"

#include <exiv2/exiv2.hpp>

static const std::string OPENCV_WINDOW = "Image window";
static bool take_video = false;
static char dest_video[50];
static int video_count = 0; 
cv::VideoWriter video;
ros::Publisher camera_pub;
ros::Subscriber camera_sub;
camera_handler_SHERPA::Camera camera_topic;
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
		camera_sub = nh_.subscribe("/camera_trigger", 10, &CameraHandler::trigger_image_video, this);

    image_pub_ = it_.advertise("/camera_handler/output_video", 1);
		camera_pub = nh_.advertise<camera_handler_SHERPA::Camera>("/camera_trigger", 20);

    //cv::namedWindow(OPENCV_WINDOW);
  }

	void trigger_image_video(const camera_handler_SHERPA::Camera::ConstPtr& msg){
		camera_topic.take_pic = msg->take_pic;
		camera_topic.take_vid = msg->take_vid;
		if (msg->take_vid && !take_video) startVideo();
		else if (!msg->take_vid && take_video) stopVideo();
	}

	void startVideo()
	{
		sprintf(dest_video,"/home/odroid/Video_Mission/video_%d.avi",video_count);
		video.open(dest_video, CV_FOURCC('M','J','P','G'), 15, cv::Size(640,480), true);             //TODO take size and fps dynamicly
		take_video = true;
	}
	void stopVideo()
	{
		video_count++;
		take_video = false;
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

    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    //cv::waitKey(3);

		// Save Image
		static int image_count = 0;
		static char dest_img[50];                        
  	sprintf(dest_img,"/home/odroid/Photo_Mission/image_%d.jpg",image_count);
		
		if (camera_topic.take_pic){
			ROS_ASSERT( cv::imwrite(dest_img,  cv_ptr->image));
			image_count++;
			camera_topic.take_pic = false;
			camera_pub.publish(camera_topic);

			//Metadata
			//exifData["Exif.DarioScemo"] = "True";                     // Ascii
			//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(dest_img);
		  //assert(image.get() != 0);
		  //image->setExifData(exifData);
		  //image->writeMetadata();
		}

		if (take_video) video << cv_ptr->image;
    
    // Output modified video stream                NOT NEEDED
    //image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "camera_handler_SHERPA");
  CameraHandler ic;
  ros::spin();
  return 0;
}
