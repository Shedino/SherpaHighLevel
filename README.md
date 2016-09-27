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
* Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_camera" (camera sensor)
* Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_wasp0_artva" (artva sensor)
* There should be already aliases (in ~/.bashrc) to run this command. The alias should be something like waspX, with X going from 0 to 2 for 3 WASPs. 

## To run the real flight software
* Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/screen_waspX_real" (change the X in screen_waspX_real with a number between 0 and 2 to set the workspace of the wasp). There should be already aliases (in ~/.bashrc) to run this command. The alias should be something like waspX_real, with X going from 0 to 2 for 3 WASPs. 

## Nodes involved in real flight software
* mms (under screen "mms"): mission managment system. The state machine to handle the state of the WASP.
* frame (under screen "mms"): decide in which frame (barometer or altitude sonar-laser) to navigate based on the height and sensors
* pos_mixer (under screen "mms"): mixes altitudes from barometer, accelerometers and sonar-laser based on the actual frame to give the correct altitude to the navigation node
* reference (under screen "mms"): builds the reference trajectory for the navigation node
* qos_sensors_autopilot (under screen "mms"): quality of service of sensors and autopilot. This node publishes on topics the state of the autopilot and the state of the sensors (sonar, artva, camera, laser).
* mavros (under screen "mavros"): node to interface the autopilot and ODROID(ROS) via MAVlink protocol. It can receive MAVlink messages and translate into topics and vice-versa. It is using the custom plugin unibo_controller_amsl.cpp. The plugin configurations is defined in unibo_pluginlists.yaml and the parameters for the parameters server are defined in unibo_config.yaml. To ask streams to the autopilot you can use the command "rosservice call /wasp0/mavros/set_stream_rate 0 12 true"
* guidance_node_amsl (under screen "mavros"): Guidance node that takes trajectory and state and generate suitable velocities for the autopilot.
* usb_cam (under screen "Wasp Camera Usb"): Node for the interface with the USB webcam. It streams the raw data into a topic.
* camera_handler_sherpa (under screen "Wasp Camera Handler"): Node that takes raw data from camera and saves pictures or video when commanded by the mms.
* swm_interface (under screen "SWM interface"): Interface with SWM. PROBABLY DEPRECATED!
* Delegation stuff (under screens "tst0", "Delegation", "roszyre" and "proxy"): 
* proxy create-unibo (under screens "Proxy UNIBO-CREATE 1" and "Proxy UNIBO-CREATE 2"): Proxy that uses zyre for communication CREATE-UNIBO (used to send pictures, bracelet commands, and more)
* bag (under screens "bag"): bag saving script. Remember to terminate with CTRL+C to close the bag, otherwise it has to be fixed manually (it remains .active).
