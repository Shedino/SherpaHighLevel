## SherpaHighLevel
Sherpa high level software

## Dependancies
Mavros

## External Repos
-Delegation Framework (inside catkin_make/src):
https://gitlab.ida.liu.se
Put this repo only if we want the interface via Delegation Framework

-Zyre proxy CREATE-UNIBO (inside catkin_make/src):
https://github.com/tuloski/sherpa_proxy.git

-DCM/SWM (inside DCM folder in home)
TODO put final repo

## To compile
-Unzip MLIB.tar.gz into a "Matlab" folder in home
-cd catkin_ws and "catkin make" (if not working try "catkin_make -j1", it may be a dependancies problem)

## To run the simple simulator
-"roslaunch uav_sim wasp_simulator_camera.launch" (camera sensor)
-"roslaunch uav_sim wasp_simulator_artva.launch" (artva sensor)

## To run the complete simulator (camera + DF + location)
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_camera" (camera sensor)
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_artva" (artva sensor)

## To run the real flight software
-Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_waspX_real" (change the X in screen_waspX_real with a number between 0 and 2 to set the workspace of the wasp)
