; Auto-generated. Do not edit!


(cl:in-package mavros-srv)


;//! \htmlinclude WaypointPush-request.msg.html

(cl:defclass <WaypointPush-request> (roslisp-msg-protocol:ros-message)
  ((waypoints
    :reader waypoints
    :initarg :waypoints
    :type (cl:vector mavros-msg:Waypoint)
   :initform (cl:make-array 0 :element-type 'mavros-msg:Waypoint :initial-element (cl:make-instance 'mavros-msg:Waypoint))))
)

(cl:defclass WaypointPush-request (<WaypointPush-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <WaypointPush-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'WaypointPush-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mavros-srv:<WaypointPush-request> is deprecated: use mavros-srv:WaypointPush-request instead.")))

(cl:ensure-generic-function 'waypoints-val :lambda-list '(m))
(cl:defmethod waypoints-val ((m <WaypointPush-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:waypoints-val is deprecated.  Use mavros-srv:waypoints instead.")
  (waypoints m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <WaypointPush-request>) ostream)
  "Serializes a message object of type '<WaypointPush-request>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'waypoints))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'waypoints))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <WaypointPush-request>) istream)
  "Deserializes a message object of type '<WaypointPush-request>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'waypoints) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'waypoints)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'mavros-msg:Waypoint))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<WaypointPush-request>)))
  "Returns string type for a service object of type '<WaypointPush-request>"
  "mavros/WaypointPushRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'WaypointPush-request)))
  "Returns string type for a service object of type 'WaypointPush-request"
  "mavros/WaypointPushRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<WaypointPush-request>)))
  "Returns md5sum for a message object of type '<WaypointPush-request>"
  "79e2ea4ec81cd20734a7c12cd819e866")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'WaypointPush-request)))
  "Returns md5sum for a message object of type 'WaypointPush-request"
  "79e2ea4ec81cd20734a7c12cd819e866")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<WaypointPush-request>)))
  "Returns full string definition for message of type '<WaypointPush-request>"
  (cl:format cl:nil "~%~%~%~%mavros/Waypoint[] waypoints~%~%================================================================================~%MSG: mavros/Waypoint~%# Waypoint.msg~%#~%# ROS representation of MAVLink MISSION_ITEM~%# See mavlink documentation~%~%~%~%# see enum MAV_FRAME~%uint8 frame~%uint8 FRAME_GLOBAL = 0~%uint8 FRAME_LOCAL_NED = 1~%uint8 FRAME_MISSION = 2~%uint8 FRAME_GLOBAL_REL_ALT = 3~%uint8 FRAME_LOCAL_ENU = 4~%~%# see enum MAV_CMD~%uint16 command~%uint16 NAV_WAYPOINT = 16~%uint16 NAV_LOITER_UNLIM = 17~%uint16 NAV_LOITER_TURNS = 18~%uint16 NAV_LOITER_TIME = 19~%uint16 NAV_RETURN_TO_LAUNCH = 20~%uint16 NAV_LAND = 21~%uint16 NAV_TAKEOFF = 22~%# TODO: ROI mode~%~%bool is_current~%bool autocontinue~%# meaning of this params described in enum MAV_CMD~%float32 param1~%float32 param2~%float32 param3~%float32 param4~%float64 x_lat~%float64 y_long~%float64 z_alt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'WaypointPush-request)))
  "Returns full string definition for message of type 'WaypointPush-request"
  (cl:format cl:nil "~%~%~%~%mavros/Waypoint[] waypoints~%~%================================================================================~%MSG: mavros/Waypoint~%# Waypoint.msg~%#~%# ROS representation of MAVLink MISSION_ITEM~%# See mavlink documentation~%~%~%~%# see enum MAV_FRAME~%uint8 frame~%uint8 FRAME_GLOBAL = 0~%uint8 FRAME_LOCAL_NED = 1~%uint8 FRAME_MISSION = 2~%uint8 FRAME_GLOBAL_REL_ALT = 3~%uint8 FRAME_LOCAL_ENU = 4~%~%# see enum MAV_CMD~%uint16 command~%uint16 NAV_WAYPOINT = 16~%uint16 NAV_LOITER_UNLIM = 17~%uint16 NAV_LOITER_TURNS = 18~%uint16 NAV_LOITER_TIME = 19~%uint16 NAV_RETURN_TO_LAUNCH = 20~%uint16 NAV_LAND = 21~%uint16 NAV_TAKEOFF = 22~%# TODO: ROI mode~%~%bool is_current~%bool autocontinue~%# meaning of this params described in enum MAV_CMD~%float32 param1~%float32 param2~%float32 param3~%float32 param4~%float64 x_lat~%float64 y_long~%float64 z_alt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <WaypointPush-request>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'waypoints) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <WaypointPush-request>))
  "Converts a ROS message object to a list"
  (cl:list 'WaypointPush-request
    (cl:cons ':waypoints (waypoints msg))
))
;//! \htmlinclude WaypointPush-response.msg.html

(cl:defclass <WaypointPush-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (wp_transfered
    :reader wp_transfered
    :initarg :wp_transfered
    :type cl:integer
    :initform 0))
)

(cl:defclass WaypointPush-response (<WaypointPush-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <WaypointPush-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'WaypointPush-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mavros-srv:<WaypointPush-response> is deprecated: use mavros-srv:WaypointPush-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <WaypointPush-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:success-val is deprecated.  Use mavros-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'wp_transfered-val :lambda-list '(m))
(cl:defmethod wp_transfered-val ((m <WaypointPush-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:wp_transfered-val is deprecated.  Use mavros-srv:wp_transfered instead.")
  (wp_transfered m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <WaypointPush-response>) ostream)
  "Serializes a message object of type '<WaypointPush-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'wp_transfered)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'wp_transfered)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'wp_transfered)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'wp_transfered)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <WaypointPush-response>) istream)
  "Deserializes a message object of type '<WaypointPush-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'wp_transfered)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'wp_transfered)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'wp_transfered)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'wp_transfered)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<WaypointPush-response>)))
  "Returns string type for a service object of type '<WaypointPush-response>"
  "mavros/WaypointPushResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'WaypointPush-response)))
  "Returns string type for a service object of type 'WaypointPush-response"
  "mavros/WaypointPushResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<WaypointPush-response>)))
  "Returns md5sum for a message object of type '<WaypointPush-response>"
  "79e2ea4ec81cd20734a7c12cd819e866")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'WaypointPush-response)))
  "Returns md5sum for a message object of type 'WaypointPush-response"
  "79e2ea4ec81cd20734a7c12cd819e866")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<WaypointPush-response>)))
  "Returns full string definition for message of type '<WaypointPush-response>"
  (cl:format cl:nil "bool success~%uint32 wp_transfered~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'WaypointPush-response)))
  "Returns full string definition for message of type 'WaypointPush-response"
  (cl:format cl:nil "bool success~%uint32 wp_transfered~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <WaypointPush-response>))
  (cl:+ 0
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <WaypointPush-response>))
  "Converts a ROS message object to a list"
  (cl:list 'WaypointPush-response
    (cl:cons ':success (success msg))
    (cl:cons ':wp_transfered (wp_transfered msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'WaypointPush)))
  'WaypointPush-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'WaypointPush)))
  'WaypointPush-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'WaypointPush)))
  "Returns string type for a service object of type '<WaypointPush>"
  "mavros/WaypointPush")