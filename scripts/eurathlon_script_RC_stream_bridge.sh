rosrun mavros mavsys rate --all 10
rosrun mavros mavparam set SYSID_MYGCS 1
#rosrun mavros gcs_bridge _gcs_url:='udp://192.168.0.106:5000@192.168.0.100:5005'
