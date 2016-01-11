#
# Used by a leashing TST to report its status
#

int32 horizontal_control_mode
float64 horizontal_distance
float64 horizontal_heading
float64 distance_north
float64 distance_east

int32 vertical_control_mode
float64 vertical_distance

int32 yaw_control_mode
float64 yaw
geographic_msgs/GeoPoint yawpoint

int32 failure # 0-->leashing ok | 1-->initial condition failure | 2-->execution failure