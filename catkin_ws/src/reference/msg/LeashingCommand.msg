#
# Used for giving commands to a leashing TST
#
# The position that the vehicle should be leashed to is called anchor or anchor
# position.

#
# Horizontal control
#

int32 HORIZONTAL_CONTROL_MODE_NONE=0
int32 HORIZONTAL_CONTROL_MODE_KEEP_DISTANCE=1
int32 HORIZONTAL_CONTROL_MODE_DISTANCE_HEADING_ABSOLUTE=2
int32 HORIZONTAL_CONTROL_MODE_NORTH_EAST_ABSOLUTE=3
int32 HORIZONTAL_CONTROL_MODE_DISTANCE_HEADING_VEL=4
int32 HORIZONTAL_CONTROL_MODE_NORTH_EAST_VEL=5

float64 horizontal_distance
# Must be positive, given in meter. Is distance to anchor position

float64 horizontal_distance_vel
# Velocity between -1 and 1

float64 horizontal_heading
# In degrees, 0 is north, positive clockwise. The angle between north and the line
# defined by anchor position and vehicle position

float64 horizontal_heading_vel
# Velocity between -1 and 1

float64 distance_north
# distance in meter and positive in north direction.

float64 distance_north_vel
# Velocity between -1 and 1

float64 distance_east
# distance in meter and positive in east direction.

float64 distance_east_vel
# Velocity between -1 and 1


int32 horizontal_control_mode

#
# Vertical control
#

int32 VERTICAL_CONTROL_MODE_NONE=0
int32 VERTICAL_CONTROL_MODE_KEEP=1
int32 VERTICAL_CONTROL_MODE_ABSOLUTE=2
int32 VERTICAL_CONTROL_MODE_VEL=3

float64 vertical_distance
float64 vertical_distance_vel
# Velocity between -1 and 1
# Given in meter, is distance to anchor position.

int32 vertical_control_mode


#
# Yaw control mode
#

int32 YAW_CONTROL_MODE_NONE=0
int32 YAW_CONTROL_MODE_ABSOLUTE=1
int32 YAW_CONTROL_MODE_VEL=2
int32 YAW_CONTROL_MODE_TOWARDS_POINT=3
int32 YAW_CONTROL_MODE_TOWARDS_ANCHOR=4
int32 YAW_CONTROL_MODE_AWAY_FROM_ANCHOR=5

float64 yaw
float64 yaw_vel
# Velocity between -1 and 1
geographic_msgs/GeoPoint yawpoint
# In degrees, the same as heading

int32 yaw_control_mode