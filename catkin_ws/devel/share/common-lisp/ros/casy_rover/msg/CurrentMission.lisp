; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude CurrentMission.msg.html

(cl:defclass <CurrentMission> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (mission_num
    :reader mission_num
    :initarg :mission_num
    :type cl:fixnum
    :initform 0)
   (wp_dist
    :reader wp_dist
    :initarg :wp_dist
    :type cl:fixnum
    :initform 0)
   (target_bearing
    :reader target_bearing
    :initarg :target_bearing
    :type cl:fixnum
    :initform 0))
)

(cl:defclass CurrentMission (<CurrentMission>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CurrentMission>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CurrentMission)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<CurrentMission> is deprecated: use casy_rover-msg:CurrentMission instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <CurrentMission>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:header-val is deprecated.  Use casy_rover-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'mission_num-val :lambda-list '(m))
(cl:defmethod mission_num-val ((m <CurrentMission>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:mission_num-val is deprecated.  Use casy_rover-msg:mission_num instead.")
  (mission_num m))

(cl:ensure-generic-function 'wp_dist-val :lambda-list '(m))
(cl:defmethod wp_dist-val ((m <CurrentMission>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:wp_dist-val is deprecated.  Use casy_rover-msg:wp_dist instead.")
  (wp_dist m))

(cl:ensure-generic-function 'target_bearing-val :lambda-list '(m))
(cl:defmethod target_bearing-val ((m <CurrentMission>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:target_bearing-val is deprecated.  Use casy_rover-msg:target_bearing instead.")
  (target_bearing m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CurrentMission>) ostream)
  "Serializes a message object of type '<CurrentMission>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mission_num)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'mission_num)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'wp_dist)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'wp_dist)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'target_bearing)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CurrentMission>) istream)
  "Deserializes a message object of type '<CurrentMission>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mission_num)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'mission_num)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'wp_dist)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'wp_dist)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'target_bearing) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CurrentMission>)))
  "Returns string type for a message object of type '<CurrentMission>"
  "casy_rover/CurrentMission")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CurrentMission)))
  "Returns string type for a message object of type 'CurrentMission"
  "casy_rover/CurrentMission")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CurrentMission>)))
  "Returns md5sum for a message object of type '<CurrentMission>"
  "deaf93654547b1e516f7011234618190")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CurrentMission)))
  "Returns md5sum for a message object of type 'CurrentMission"
  "deaf93654547b1e516f7011234618190")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CurrentMission>)))
  "Returns full string definition for message of type '<CurrentMission>"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: NAV_CONTROLLER_OUTPUT, MISSION_CURRENT~%# See common.xml in MAVLink Messages~%~%# Information regarding current waypoint target~%~%Header header~%~%# Mission Number (sequence)~%uint16 mission_num~%~%# Distance to WP in meters~%uint16 wp_dist~%~%# Bearing to Current target in degrees~%int16 target_bearing~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CurrentMission)))
  "Returns full string definition for message of type 'CurrentMission"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: NAV_CONTROLLER_OUTPUT, MISSION_CURRENT~%# See common.xml in MAVLink Messages~%~%# Information regarding current waypoint target~%~%Header header~%~%# Mission Number (sequence)~%uint16 mission_num~%~%# Distance to WP in meters~%uint16 wp_dist~%~%# Bearing to Current target in degrees~%int16 target_bearing~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CurrentMission>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CurrentMission>))
  "Converts a ROS message object to a list"
  (cl:list 'CurrentMission
    (cl:cons ':header (header msg))
    (cl:cons ':mission_num (mission_num msg))
    (cl:cons ':wp_dist (wp_dist msg))
    (cl:cons ':target_bearing (target_bearing msg))
))
