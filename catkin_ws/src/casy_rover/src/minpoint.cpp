/*
Posizione degli assi su rviz:
# Red - x-axis (Roll About)
# Green - y-axis (Pitch About)
# Blue - z-axis (Yaw About)
Il sistema di riferimento è destrorso e il fixed frame di riferimento è depth_optical_frame (x a destra, y in basso, z in avanti)
*/

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/Point.h>

/**** COSTANTI ****/

// Distance limits
#define OBSTACLE_DETECTION  1.0
#define GO_BACK_LIMIT       0.6

// Rover dimensions
#define LEFT_WIDTH -0.25
#define RIGHT_WIDTH 0.25


/******************/

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

//Publisher per la pointcloud limitata
//ros::Publisher points_pub_;

//Publisher per il punto a minima distanza
ros::Publisher min_point_pub_;

void callback(const PointCloud::ConstPtr& msg)
{
  PointCloud tmp_cloud;		// Porzione di Pointcloud presa in considerazione dall'algoritmo
  geometry_msgs::Point min_point;

  // Inizializzazione del vettore a valori oltre il range di operativita' dell'algoritmo
  min_point.x = OBSTACLE_DETECTION + 0.1;
  min_point.y = OBSTACLE_DETECTION + 0.1;
  min_point.z = OBSTACLE_DETECTION + 0.1;

  BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)  //it works like a loop which assignes the values 1 by 1 to a variable from an array
  {

    if( !pcl_isfinite( pt.x ) || !pcl_isfinite( pt.y ) || !pcl_isfinite( pt.z ) )  // this is done so that the nan values from the pointcloud are not processed
      continue;

    // Vengono considerati solo i punti nel range di OBSTACLE_DETECTION e sopra all'orizzonte della camera
    if (pt.z <= OBSTACLE_DETECTION && pt.y < 0 && pt.x >= LEFT_WIDTH && pt.x <= RIGHT_WIDTH) {

      tmp_cloud.push_back(pt);    //Salvataggio del punto nella porzione di pointcloud

      if (pt.z < min_point.z)
      {
        min_point.x=pt.x;
	      min_point.y=pt.y;
	      min_point.z=pt.z;
      }
    }
  }//foreach

  min_point_pub_.publish(min_point);

 /* if (tmp_cloud.size() > 0) {
    sensor_msgs::PointCloud2 pc2;
    pcl::toROSMsg(tmp_cloud, pc2);
    pc2.header.frame_id = "camera_depth_optical_frame";   //ATTENZIONE: il fixed frame non viene preso dalla pointcloud di input!
    points_pub_.publish(pc2);
  }*/

}//callback()

int main(int argc, char** argv)
{
  ros::init(argc, argv, "sub_pcl");
  ros::NodeHandle nh,nh1;

  ros::Subscriber sub = nh.subscribe<PointCloud>("voxel_grid/output", 1, callback);	//subsciber alla pointcloud della camera
 // points_pub_ = nh1.advertise<sensor_msgs::PointCloud2>("limited_pointcloud", 1);
  min_point_pub_ = nh1.advertise<geometry_msgs::Point>("min_distance_point", 10);

  ros::Rate loop_rate(2);	// 2 Hz

  while (ros::ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }
}//main()
