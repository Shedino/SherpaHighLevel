; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude MissionItem.msg.html

(cl:defclass <MissionItem> (roslisp-msg-protocol:ros-message)
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
   (isCurrent
    :reader isCurrent
    :initarg :isCurrent
    :type cl:fixnum
    :initform 0)
   (isAutoContinue
    :reader isAutoContinue
    :initarg :isAutoContinue
    :type cl:fixnum
    :initform 0)
   (posAcc
    :reader posAcc
    :initarg :posAcc
    :type cl:float
    :initform 0.0)
   (holdTime
    :reader holdTime
    :initarg :holdTime
    :type cl:float
    :initform 0.0)
   (loiterRadius
    :reader loiterRadius
    :initarg :loiterRadius
    :type cl:float
    :initform 0.0)
   (yawFrom
    :reader yawFrom
    :initarg :yawFrom
    :type cl:float
    :initform 0.0)
   (x_lat
    :reader x_lat
    :initarg :x_lat
    :type cl:float
    :initform 0.0)
   (y_long
    :reader y_long
    :initarg :y_long
    :type cl:float
    :initform 0.0)
   (z_alt
    :reader z_alt
    :initarg :z_alt
    :type cl:float
    :initform 0.0))
)

(cl:defclass MissionItem (<MissionItem>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MissionItem>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MissionItem)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<MissionItem> is deprecated: use casy_rover-msg:MissionItem instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:header-val is deprecated.  Use casy_rover-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'mission_num-val :lambda-list '(m))
(cl:defmethod mission_num-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:mission_num-val is deprecated.  Use casy_rover-msg:mission_num instead.")
  (mission_num m))

(cl:ensure-generic-function 'isCurrent-val :lambda-list '(m))
(cl:defmethod isCurrent-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:isCurrent-val is deprecated.  Use casy_rover-msg:isCurrent instead.")
  (isCurrent m))

(cl:ensure-generic-function 'isAutoContinue-val :lambda-list '(m))
(cl:defmethod isAutoContinue-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:isAutoContinue-val is deprecated.  Use casy_rover-msg:isAutoContinue instead.")
  (isAutoContinue m))

(cl:ensure-generic-function 'posAcc-val :lambda-list '(m))
(cl:defmethod posAcc-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:posAcc-val is deprecated.  Use casy_rover-msg:posAcc instead.")
  (posAcc m))

(cl:ensure-generic-function 'holdTime-val :lambda-list '(m))
(cl:defmethod holdTime-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:holdTime-val is deprecated.  Use casy_rover-msg:holdTime instead.")
  (holdTime m))

(cl:ensure-generic-function 'loiterRadius-val :lambda-list '(m))
(cl:defmethod loiterRadius-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:loiterRadius-val is deprecated.  Use casy_rover-msg:loiterRadius instead.")
  (loiterRadius m))

(cl:ensure-generic-function 'yawFrom-val :lambda-list '(m))
(cl:defmethod yawFrom-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:yawFrom-val is deprecated.  Use casy_rover-msg:yawFrom instead.")
  (yawFrom m))

(cl:ensure-generic-function 'x_lat-val :lambda-list '(m))
(cl:defmethod x_lat-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:x_lat-val is deprecated.  Use casy_rover-msg:x_lat instead.")
  (x_lat m))

(cl:ensure-generic-function 'y_long-val :lambda-list '(m))
(cl:defmethod y_long-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:y_long-val is deprecated.  Use casy_rover-msg:y_long instead.")
  (y_long m))

(cl:ensure-generic-function 'z_alt-val :lambda-list '(m))
(cl:defmethod z_alt-val ((m <MissionItem>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:z_alt-val is deprecated.  Use casy_rover-msg:z_alt instead.")
  (z_alt m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MissionItem>) ostream)
  "Serializes a message object of type '<MissionItem>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mission_num)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'mission_num)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isCurrent)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAutoContinue)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'posAcc))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'holdTime))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'loiterRadius))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yawFrom))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'x_lat))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'y_long))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'z_alt))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MissionItem>) istream)
  "Deserializes a message object of type '<MissionItem>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mission_num)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'mission_num)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isCurrent)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'isAutoContinue)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'posAcc) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'holdTime) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'loiterRadius) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yawFrom) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x_lat) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y_long) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'z_alt) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MissionItem>)))
  "Returns string type for a message object of type '<MissionItem>"
  "casy_rover/MissionItem")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MissionItem)))
  "Returns string type for a message object of type 'MissionItem"
  "casy_rover/MissionItem")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MissionItem>)))
  "Returns md5sum for a message object of type '<MissionItem>"
  "a4eec5db18601d2affa66a99457a442d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MissionItem)))
  "Returns md5sum for a message object of type 'MissionItem"
  "a4eec5db18601d2affa66a99457a442d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MissionItem>)))
  "Returns full string definition for message of type '<MissionItem>"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: MISSION_ITEM~%# See common.xml in MAVLink Messages~%~%# Information regarding current waypoint target~%~%Header header~%~%# Mission Item Number~%uint16 mission_num~%~%# Boolean to show if mission item is current~%uint8 isCurrent~%uint8 isAutoContinue~%~%# Position Accuracy in meters~%float32 posAcc~%~%# Hold Time in ms~%float32 holdTime~%~%# Radius in meters for Loiter Circle.  Positive = clockwise, negative = counter~%float32 loiterRadius~%~%# Desired Yaw Angle in degrees~%float32 yawFrom~%~%# Latitude, Longitude, and Altitude~%float32 x_lat~%float32 y_long~%float32 z_alt~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MissionItem)))
  "Returns full string definition for message of type 'MissionItem"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: MISSION_ITEM~%# See common.xml in MAVLink Messages~%~%# Information regarding current waypoint target~%~%Header header~%~%# Mission Item Number~%uint16 mission_num~%~%# Boolean to show if mission item is current~%uint8 isCurrent~%uint8 isAutoContinue~%~%# Position Accuracy in meters~%float32 posAcc~%~%# Hold Time in ms~%float32 holdTime~%~%# Radius in meters for Loiter Circle.  Positive = clockwise, negative = counter~%float32 loiterRadius~%~%# Desired Yaw Angle in degrees~%float32 yawFrom~%~%# Latitude, Longitude, and Altitude~%float32 x_lat~%float32 y_long~%float32 z_alt~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MissionItem>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     2
     1
     1
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MissionItem>))
  "Converts a ROS message object to a list"
  (cl:list 'MissionItem
    (cl:cons ':header (header msg))
    (cl:cons ':mission_num (mission_num msg))
    (cl:cons ':isCurrent (isCurrent msg))
    (cl:cons ':isAutoContinue (isAutoContinue msg))
    (cl:cons ':posAcc (posAcc msg))
    (cl:cons ':holdTime (holdTime msg))
    (cl:cons ':loiterRadius (loiterRadius msg))
    (cl:cons ':yawFrom (yawFrom msg))
    (cl:cons ':x_lat (x_lat msg))
    (cl:cons ':y_long (y_long msg))
    (cl:cons ':z_alt (z_alt msg))
))
