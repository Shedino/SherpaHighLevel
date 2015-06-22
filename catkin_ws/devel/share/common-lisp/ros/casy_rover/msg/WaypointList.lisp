; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude WaypointList.msg.html

(cl:defclass <WaypointList> (roslisp-msg-protocol:ros-message)
  ((waypoints
    :reader waypoints
    :initarg :waypoints
    :type (cl:vector casy_rover-msg:Waypoint)
   :initform (cl:make-array 0 :element-type 'casy_rover-msg:Waypoint :initial-element (cl:make-instance 'casy_rover-msg:Waypoint))))
)

(cl:defclass WaypointList (<WaypointList>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <WaypointList>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'WaypointList)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<WaypointList> is deprecated: use casy_rover-msg:WaypointList instead.")))

(cl:ensure-generic-function 'waypoints-val :lambda-list '(m))
(cl:defmethod waypoints-val ((m <WaypointList>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:waypoints-val is deprecated.  Use casy_rover-msg:waypoints instead.")
  (waypoints m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <WaypointList>) ostream)
  "Serializes a message object of type '<WaypointList>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'waypoints))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'waypoints))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <WaypointList>) istream)
  "Deserializes a message object of type '<WaypointList>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'waypoints) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'waypoints)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'casy_rover-msg:Waypoint))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<WaypointList>)))
  "Returns string type for a message object of type '<WaypointList>"
  "casy_rover/WaypointList")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'WaypointList)))
  "Returns string type for a message object of type 'WaypointList"
  "casy_rover/WaypointList")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<WaypointList>)))
  "Returns md5sum for a message object of type '<WaypointList>"
  "55f6e3d4649782ff855bc4699f0b61df")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'WaypointList)))
  "Returns md5sum for a message object of type 'WaypointList"
  "55f6e3d4649782ff855bc4699f0b61df")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<WaypointList>)))
  "Returns full string definition for message of type '<WaypointList>"
  (cl:format cl:nil "Waypoint[] waypoints~%~%================================================================================~%MSG: casy_rover/Waypoint~%# latitude and longitude given in decimal degrees (* 10^7)~%int32 latitude~%int32 longitude~%~%# height in mm~%int32 altitude~%~%# posAcc given in mm~%uint16 pos_acc~%~%# max speed to travel to waypoint in % of 100~%uint8 speed_to~%~%# time to stay at waypoint in ms~%uint16 hold_time~%~%# yawFrom angle given in degrees (* 1000)~%int32 yaw_from~%~%# camera pan and tilt angles during waypoint travel given in degrees (* 1000)~%int32 pan_angle~%int32 tilt_angle~%~%# Type of Waypoint~%uint8 waypoint_type~%~%uint8 TYPE_NAV = 1~%uint8 TYPE_CONDITION_YAW = 2~%uint8 TYPE_CONDITION_CHANGE_ALT = 3~%uint8 TYPE_TAKEOFF = 4~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'WaypointList)))
  "Returns full string definition for message of type 'WaypointList"
  (cl:format cl:nil "Waypoint[] waypoints~%~%================================================================================~%MSG: casy_rover/Waypoint~%# latitude and longitude given in decimal degrees (* 10^7)~%int32 latitude~%int32 longitude~%~%# height in mm~%int32 altitude~%~%# posAcc given in mm~%uint16 pos_acc~%~%# max speed to travel to waypoint in % of 100~%uint8 speed_to~%~%# time to stay at waypoint in ms~%uint16 hold_time~%~%# yawFrom angle given in degrees (* 1000)~%int32 yaw_from~%~%# camera pan and tilt angles during waypoint travel given in degrees (* 1000)~%int32 pan_angle~%int32 tilt_angle~%~%# Type of Waypoint~%uint8 waypoint_type~%~%uint8 TYPE_NAV = 1~%uint8 TYPE_CONDITION_YAW = 2~%uint8 TYPE_CONDITION_CHANGE_ALT = 3~%uint8 TYPE_TAKEOFF = 4~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <WaypointList>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'waypoints) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <WaypointList>))
  "Converts a ROS message object to a list"
  (cl:list 'WaypointList
    (cl:cons ':waypoints (waypoints msg))
))
