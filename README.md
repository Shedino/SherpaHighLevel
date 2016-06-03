## SherpaHighLevel
Sherpa high level software

## Dependancies
Mavros

## External Repos
-Camera UMD repo:
https://github.com/ktossell/camera_umd.git

-Delegation Framework repo:
gitlab.ida.liu.se

## To compile
-Unzip MLIB.tar.gz into a "Matlab" folder
-cd catkin_ws and "catkin make"

## To run the simple simulator
-"roslaunch uav_sim wasp_simulator_camera.launch"

## To run the complete simulator (camera + DF + location)
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_camera"