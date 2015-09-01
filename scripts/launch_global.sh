#!/bin/bash
source /opt/ros/indigo/setup.bash
source /home/odroid/catkin_ws/devel/setup.bash
export ROS_WORKSPACE=/home/odroid/catkin_ws
export DISPLAY=:0
sleep 20
roslaunch mavros uniboAMSLUSB.launch &
sleep 10
rosrun usb_cam usb_cam_node &
sleep 10
rosrun camera_handler_SHERPA camera_handler_SHERPA &
sleep 6
rosrun web_video_server web_video_server &
sleep 5
rosrun mavros mavsys rate --all 10 &
sleep 4
roslaunch mavros unibo_GCS.launch &
sleep 10
roslaunch mms mms.launch &
sleep 8
