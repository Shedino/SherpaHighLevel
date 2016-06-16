## SherpaHighLevel
Sherpa high level software

## Dependancies
Mavros

## External Repos
-Camera UMD (inside catkin_make/src):
https://github.com/ktossell/camera_umd.git

-Delegation Framework (inside catkin_make/src):
gitlab.ida.liu.se

-Zyre proxy CREATE-UNIBO (inside catkin_make/src):
https://github.com/tuloski/sherpa_proxy.git

## To compile
-Unzip MLIB.tar.gz into a "Matlab" folder
-cd catkin_ws and "catkin make"

## To run the simple simulator
-"roslaunch uav_sim wasp_simulator_camera.launch"

## To run the complete simulator (camera + DF + location)
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_camera"

## To run the real flight software
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_waspX_real" (change the X in screen_waspX_real with a number between 0 and 2 to set the workspace of the wasp)
