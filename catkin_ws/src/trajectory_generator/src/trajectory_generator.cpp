#include "ros/ros.h"
#include "math.h"
#include <cstddef>

#include "nav_msgs/Path.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Quaternion.h"
#include <visualization_msgs/Marker.h>
#include <tf/transform_broadcaster.h>
#include "nav_msgs/Odometry.h"
#include "mavros/Safety.h"
#include "mavros/PositionTarget.h"


#define max_speed 0.8


class TrajectoryGeneratorClass {
public:
	TrajectoryGeneratorClass(ros::NodeHandle& node)
	{
		n_ = node;

		//subscribers
		subPlan=n_.subscribe("/move_base_node/NavfnROS/plan", 1, &TrajectoryGeneratorClass::readPlan,this); 
		subPosFilter = n_.subscribe("/odom", 1, &TrajectoryGeneratorClass::readOdometry,this);
		subSafety = n_.subscribe("/safety_odroid", 1, &TrajectoryGeneratorClass::readSafety,this);
		// publishers
        //marker_pub = n_.advertise<visualization_msgs::Marker>("visualization_marker", 10);
        //cmd_pose_pub = n_.advertise<geometry_msgs::Pose>("/cmd_pose", 10);
        //cmd_vel_pub = n_.advertise<geometry_msgs::Twist>("/cmd_velocities", 10);
        cmd_vel_pose_pub = n_.advertise<nav_msgs::Odometry>("/cmd_vel_pose", 10);
		
		//Initializing states
        rate = 50;
        dt = 1.0/rate;
        target_speed = 1;
        flag_received_new_path = false;
        N_points_in_path = 0;
        index_actual_segment = 0;
        s = 0;
        ds = 0;
        dds = 0.3;
        position_xy.reserve(2);
        position_xy[0] = 0;
        position_xy[1] = 0;
        speed_xy.reserve(2);
        speed_xy[0] = 0;
        speed_xy[1] = 0;
        counter_print = 0;
        yaw = 0;

        cmd_vel_pose.header.frame_id="map";
        //-------------- LINES RVIZ -----------//
		/*points.header.frame_id = "/map";
		points.header.stamp = ros::Time::now();
		points.ns = "points_and_lines";
		points.action = visualization_msgs::Marker::ADD;
		points.pose.orientation.w = 1.0;

		points.id = 0;

		points.type = visualization_msgs::Marker::POINTS;

		// POINTS markers use x and y scale for width/height respectively
		points.scale.x = 0.02;	//Points width
		points.scale.y = 0.02;	//Points height
		points.color.g = 1.0f;	//Points color green
        points.color.b = 1.0f;	//Points color green
		points.color.a = 1.0;*/
	}
	
	void readPlan(const nav_msgs::Path::ConstPtr& msg)
	{
        path = *msg;
        // ------ reset ---------
        distances.clear();
        adapted_times.clear();
        ds_vector.clear();
        delta_x_vector.clear();
        delta_y_vector.clear();
        s = 0;
        ds = 0;
        dds = 0;
        index_actual_segment = 0;
        //RVIZ
        //points.points.clear();
        //marker_pub.publish(points);
        // ----------------------
        N_points_in_path = path.poses.size();
        if (N_points_in_path > 15){
        	N_points_in_path -= 4;	//Removing last 4 stupid points from global planner
        }

        ROS_INFO("TrajectoryGenerator: received plan. N points: %d",N_points_in_path);
        double temp_distance;
        double temp_time;
        double total_distance = 0;
        for (int i=0; i<N_points_in_path-1; i++){
            temp_distance = sqrt(pow(path.poses[i+1].pose.position.x-path.poses[i].pose.position.x,2)+
                                 pow(path.poses[i+1].pose.position.y-path.poses[i].pose.position.y,2));
            distances.push_back(temp_distance);
            //ROS_INFO("TrajectoryGenerator: distances: %f",temp_distance);
            total_distance += temp_distance;
        }
        if (total_distance < 0.4){
			flag_received_new_path = false;		//don't generate too short trajectories
			position_xy[0] = path.poses[N_points_in_path-1].pose.position.x;
			position_xy[1] = path.poses[N_points_in_path-1].pose.position.y;
			speed_xy[0] = 0;
			speed_xy[1] = 0;
			return;
		}
        target_speed = std::min(max_speed,total_distance/5*max_speed);    //TODO make better and not heuristic
        ROS_INFO("TrajectoryGenerator: target speed: %f",target_speed);
        for (int i=0; i<N_points_in_path-1; i++){
            temp_time = round(distances[i]/target_speed/dt)*dt;
            //ROS_INFO("TrajectoryGenerator: rounded time: %f",temp_time);  
            adapted_times.push_back(temp_time);
            ds_vector.push_back(1/temp_time);
            delta_x_vector.push_back(path.poses[i+1].pose.position.x-path.poses[i].pose.position.x);
            delta_y_vector.push_back(path.poses[i+1].pose.position.y-path.poses[i].pose.position.y);
            //ROS_INFO("TrajectoryGenerator: i: %d, x(i+1): %f, x(i): %f,y(i+1): %f, y(i): %f, deltax: %f, deltay: %f",i,path.poses[i+1].pose.position.x,path.poses[i].pose.position.x,path.poses[i+1].pose.position.y,path.poses[i].pose.position.y,delta_x_vector[i],delta_y_vector[i]);
        }
        ROS_INFO("TrajectoryGenerator: finish reading plan");
        flag_received_new_path = true;
	}
	
	void readSafety(const mavros::Safety::ConstPtr& msg){
		if (msg->safety){
			position_xy[0] = _odom.pose.pose.position.x;	//With safety gives the position of the WASP as trajectory
			position_xy[1] = _odom.pose.pose.position.y;
			yaw = tf::getYaw(_odom.pose.pose.orientation);
			speed_xy[0] = 0;
			speed_xy[1] = 0;
		}
	}

	void readOdometry(const nav_msgs::Odometry::ConstPtr& msg){
		//ROS_INFO("Received POSFILTER");
		_odom = *msg;
	}

	void loop()
	{
        //ROS_INFO("TrajectoryGenerator: LOOP");
        if (flag_received_new_path){
            /*ds += dds*dt;
            if (ds >= ds_vector[index_actual_segment]){
                ds = ds_vector[index_actual_segment];
            }*/
            if (index_actual_segment < (int)(N_points_in_path/7)){   //Acceleration
                ds = ds_vector[index_actual_segment]*(1.0/((int)(N_points_in_path/7)+1))*(index_actual_segment+1);
                /*if (counter_print >= 10){
                    ROS_INFO("TrajectoryGenerator: in acceleration: %f",ds);
                }*/
            } else if (index_actual_segment >= N_points_in_path - (int)(N_points_in_path/7)){   //Deceleration
                ds = ds_vector[index_actual_segment]*(1.0/(int)(N_points_in_path/7+1))*(N_points_in_path-index_actual_segment);
                /*if (counter_print >= 10){
                    ROS_INFO("TrajectoryGenerator: in deceleration: %f",ds);
                }*/
            } else {
                ds = ds_vector[index_actual_segment];
                /*if (counter_print >= 10){
                    ROS_INFO("TrajectoryGenerator: in normal: %f",ds);
                }*/
            }
            //Trajectory build 
            s += ds*dt;
            position_xy[0] = delta_x_vector[index_actual_segment]*s+path.poses[index_actual_segment].pose.position.x;
            position_xy[1] = delta_y_vector[index_actual_segment]*s+path.poses[index_actual_segment].pose.position.y;
            speed_xy[0] = delta_x_vector[index_actual_segment]*ds;
            speed_xy[1] = delta_y_vector[index_actual_segment]*ds;
            yaw = atan2(delta_y_vector[index_actual_segment],delta_x_vector[index_actual_segment]);
            /*if (index_actual_segment == N_points_in_path-2){
				ROS_INFO("TrajectoryGenerator: YAW DEBUG BEFORE LAST: %f, %f, %f",yaw, delta_y_vector[index_actual_segment],delta_x_vector[index_actual_segment]);
			}
            if (index_actual_segment == N_points_in_path-1){
				ROS_INFO("TrajectoryGenerator: YAW DEBUG LAST: %f, %f, %f",yaw, delta_y_vector[index_actual_segment],delta_x_vector[index_actual_segment]);
			}*/
            if (s >= 1){
                s = 0; 
                if (index_actual_segment == N_points_in_path-2){
                    flag_received_new_path = false;
                } else {
                    index_actual_segment++;
                }
                //ROS_INFO("TrajectoryGenerator: Next segment: %d", index_actual_segment);
            }
            //ROS_INFO("TrajectoryGenerator: speeds: %f , %f", speed_xy[0], speed_xy[1]);
            /* (counter_print >= 50){
                counter_print = 0;
                ROS_INFO("TrajectoryGenerator: ds_index: %f, ds: %f, s: %f , position_x: %f, position_y: %f",ds_vector[index_actual_segment], ds, s, position_xy[0], position_xy[1]);
            }*/

            //RVIZ
            /*if (counter_rviz_print > 5){
                counter_rviz_print = 0;
                geometry_msgs::Point p;
                p.x = position_xy[0];
                p.y = position_xy[1];
                p.z = 0;
                points.points.push_back(p);
		        marker_pub.publish(points);
            }*/

        } else {
            speed_xy[0] = 0;
            speed_xy[1] = 0;
            if (counter_print >= 100){
                counter_print = 0;
                ROS_INFO("TrajectoryGenerator: NO FLAG");
            }
            
        }
        /*cmd_vel.linear.x = speed_xy[0];
        cmd_vel.linear.y = speed_xy[1];      
        cmd_pose.position.x = position_xy[0];
        cmd_pose.position.y = position_xy[1];
        quaternion = tf::createQuaternionMsgFromRollPitchYaw(0.0, 0.0, yaw);
        cmd_pose.orientation = quaternion;
        cmd_pose_pub.publish(cmd_pose);*/
        cmd_vel_pose.pose.pose.position.x = position_xy[0];
        cmd_vel_pose.pose.pose.position.y = position_xy[1];
        cmd_vel_pose.pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);
        cmd_vel_pose.twist.twist.linear.x = speed_xy[0];
        cmd_vel_pose.twist.twist.linear.y = speed_xy[1];
        cmd_vel_pose.header.stamp = ros::Time::now();
        cmd_vel_pose_pub.publish(cmd_vel_pose);
	}

    void run()
    {
	    ROS_INFO("TrajectoryGenerator: RUNNING");
	    //ros::spin();
	    ros::Rate loop_rate(rate);

	    while (ros::ok())
	    {
            ros::spinOnce();
            counter_print++;
            //counter_rviz_print++;
		    loop();

		    loop_rate.sleep();
	    }
    }

protected:
ros::NodeHandle n_;

// Subscribers
ros::Subscriber subPlan;
ros::Subscriber subPosFilter;
ros::Subscriber subSafety;

// Publishers
ros::Publisher marker_pub;
//ros::Publisher cmd_pose_pub;
//ros::Publisher cmd_vel_pub;
ros::Publisher cmd_vel_pose_pub;

int rate;
double target_speed;
//double adapted_speed;
//double adapted_time;
double dt;
int index_actual_segment;
double s;
double ds;
double dds;
std::vector<double> speed_xy;
std::vector<double> position_xy;
double yaw;
nav_msgs::Path path;
bool flag_received_new_path;
int N_points_in_path;
std::vector<double> distances;
std::vector<double> adapted_times;
std::vector<double> ds_vector;
std::vector<double> delta_x_vector;
std::vector<double> delta_y_vector;
//geometry_msgs::Twist cmd_vel;
//geometry_msgs::Pose cmd_pose;
nav_msgs::Odometry cmd_vel_pose;
geometry_msgs::Quaternion quaternion;
nav_msgs::Odometry _odom;

//RVIZ
//visualization_msgs::Marker points;

private:

uint16_t counter_print;
//uint16_t counter_rviz_print;
uint16_t seq_number;
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "TrajectoryGenerator");
	ros::NodeHandle node;

	TrajectoryGeneratorClass TrajectoryGenerator(node);
	TrajectoryGenerator.run();
	
	return 0;
}
