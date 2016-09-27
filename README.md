# SherpaHighLevel
Sherpa high level software for Forest Flight

# Differences from standard WASP (SHERPA)
- Repository branch for SHERPA standard is /devel
- APM code is custom one for laser
- Arduino code is different (z axis is sent from RC also with ODROID on)
- Front camera has to be connected again on the connector going from ODROID to pixhawk usb
- APM params are different

## To run the real flight software
* Need to use screen: "env PORT=11312 ROS_MASTER_URI=http://localhost:11312/ screen -c /home/odroid/SherpaHighLevel/scripts/screen/laser_wasp". There should be already aliases (in ~/.bashrc) to run this command on the WASP108. The alias should be something like laser_wasp.

## Procedure and safety
* Once everything is started (remember the radio switch on BRIDGE for ODROID), wait a few seconds for all the node to start.
* Check on mission planner that the (fake) GPS is present with 9-10 satellites and HDOP 0.3
* Put the stick of channel5 on LOITER (medium position).
* Do take-off manually.
* Put stick of channel6 on ODROID (it will enable ODROID and planner and autonomous mode).
* From RVIZ (you need to share roscore and run RVIZ on ground station computer) you can give a goal with the "2D Nav Goal" button.
* Be sure the goal is not on an obstacle (the planner is not that smart yet)
