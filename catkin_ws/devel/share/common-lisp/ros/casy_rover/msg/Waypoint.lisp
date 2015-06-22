; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude Waypoint.msg.html

(cl:defclass <Waypoint> (roslisp-msg-protocol:ros-message)
  ((latitude
    :reader latitude
    :initarg :latitude
    :type cl:integer
    :initform 0)
   (longitude
    :reader longitude
    :initarg :longitude
    :type cl:integer
    :initform 0)
   (altitude
    :reader altitude
    :initarg :altitude
    :type cl:integer
    :initform 0)
   (pos_acc
    :reader pos_acc
    :initarg :pos_acc
    :type cl:fixnum
    :initform 0)
   (speed_to
    :reader speed_to
    :initarg :speed_to
    :type cl:fixnum
    :initform 0)
   (hold_time
    :reader hold_time
    :initarg :hold_time
    :type cl:fixnum
    :initform 0)
   (yaw_from
    :reader yaw_from
    :initarg :yaw_from
    :type cl:integer
    :initform 0)
   (pan_angle
    :reader pan_angle
    :initarg :pan_angle
    :type cl:integer
    :initform 0)
   (tilt_angle
    :reader tilt_angle
    :initarg :tilt_angle
    :type cl:integer
    :initform 0)
   (waypoint_type
    :reader waypoint_type
    :initarg :waypoint_type
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Waypoint (<Waypoint>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Waypoint>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Waypoint)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<Waypoint> is deprecated: use casy_rover-msg:Waypoint instead.")))

(cl:ensure-generic-function 'latitude-val :lambda-list '(m))
(cl:defmethod latitude-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:latitude-val is deprecated.  Use casy_rover-msg:latitude instead.")
  (latitude m))

(cl:ensure-generic-function 'longitude-val :lambda-list '(m))
(cl:defmethod longitude-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:longitude-val is deprecated.  Use casy_rover-msg:longitude instead.")
  (longitude m))

(cl:ensure-generic-function 'altitude-val :lambda-list '(m))
(cl:defmethod altitude-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:altitude-val is deprecated.  Use casy_rover-msg:altitude instead.")
  (altitude m))

(cl:ensure-generic-function 'pos_acc-val :lambda-list '(m))
(cl:defmethod pos_acc-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:pos_acc-val is deprecated.  Use casy_rover-msg:pos_acc instead.")
  (pos_acc m))

(cl:ensure-generic-function 'speed_to-val :lambda-list '(m))
(cl:defmethod speed_to-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:speed_to-val is deprecated.  Use casy_rover-msg:speed_to instead.")
  (speed_to m))

(cl:ensure-generic-function 'hold_time-val :lambda-list '(m))
(cl:defmethod hold_time-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:hold_time-val is deprecated.  Use casy_rover-msg:hold_time instead.")
  (hold_time m))

(cl:ensure-generic-function 'yaw_from-val :lambda-list '(m))
(cl:defmethod yaw_from-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:yaw_from-val is deprecated.  Use casy_rover-msg:yaw_from instead.")
  (yaw_from m))

(cl:ensure-generic-function 'pan_angle-val :lambda-list '(m))
(cl:defmethod pan_angle-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:pan_angle-val is deprecated.  Use casy_rover-msg:pan_angle instead.")
  (pan_angle m))

(cl:ensure-generic-function 'tilt_angle-val :lambda-list '(m))
(cl:defmethod tilt_angle-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:tilt_angle-val is deprecated.  Use casy_rover-msg:tilt_angle instead.")
  (tilt_angle m))

(cl:ensure-generic-function 'waypoint_type-val :lambda-list '(m))
(cl:defmethod waypoint_type-val ((m <Waypoint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:waypoint_type-val is deprecated.  Use casy_rover-msg:waypoint_type instead.")
  (waypoint_type m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<Waypoint>)))
    "Constants for message type '<Waypoint>"
  '((:TYPE_NAV . 1)
    (:TYPE_CONDITION_YAW . 2)
    (:TYPE_CONDITION_CHANGE_ALT . 3)
    (:TYPE_TAKEOFF . 4))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'Waypoint)))
    "Constants for message type 'Waypoint"
  '((:TYPE_NAV . 1)
    (:TYPE_CONDITION_YAW . 2)
    (:TYPE_CONDITION_CHANGE_ALT . 3)
    (:TYPE_TAKEOFF . 4))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Waypoint>) ostream)
  "Serializes a message object of type '<Waypoint>"
  (cl:let* ((signed (cl:slot-value msg 'latitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'longitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'altitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pos_acc)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pos_acc)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_to)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hold_time)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'hold_time)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'yaw_from)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'pan_angle)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'tilt_angle)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'waypoint_type)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Waypoint>) istream)
  "Deserializes a message object of type '<Waypoint>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'latitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'longitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'altitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pos_acc)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pos_acc)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_to)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hold_time)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'hold_time)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'yaw_from) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'pan_angle) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'tilt_angle) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'waypoint_type)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Waypoint>)))
  "Returns string type for a message object of type '<Waypoint>"
  "casy_rover/Waypoint")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Waypoint)))
  "Returns string type for a message object of type 'Waypoint"
  "casy_rover/Waypoint")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Waypoint>)))
  "Returns md5sum for a message object of type '<Waypoint>"
  "6dac49f8cf4308a151d9b97deaca8a40")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Waypoint)))
  "Returns md5sum for a message object of type 'Waypoint"
  "6dac49f8cf4308a151d9b97deaca8a40")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Waypoint>)))
  "Returns full string definition for message of type '<Waypoint>"
  (cl:format cl:nil "# latitude and longitude given in decimal degrees (* 10^7)~%int32 latitude~%int32 longitude~%~%# height in mm~%int32 altitude~%~%# posAcc given in mm~%uint16 pos_acc~%~%# max speed to travel to waypoint in % of 100~%uint8 speed_to~%~%# time to stay at waypoint in ms~%uint16 hold_time~%~%# yawFrom angle given in degrees (* 1000)~%int32 yaw_from~%~%# camera pan and tilt angles during waypoint travel given in degrees (* 1000)~%int32 pan_angle~%int32 tilt_angle~%~%# Type of Waypoint~%uint8 waypoint_type~%~%uint8 TYPE_NAV = 1~%uint8 TYPE_CONDITION_YAW = 2~%uint8 TYPE_CONDITION_CHANGE_ALT = 3~%uint8 TYPE_TAKEOFF = 4~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Waypoint)))
  "Returns full string definition for message of type 'Waypoint"
  (cl:format cl:nil "# latitude and longitude given in decimal degrees (* 10^7)~%int32 latitude~%int32 longitude~%~%# height in mm~%int32 altitude~%~%# posAcc given in mm~%uint16 pos_acc~%~%# max speed to travel to waypoint in % of 100~%uint8 speed_to~%~%# time to stay at waypoint in ms~%uint16 hold_time~%~%# yawFrom angle given in degrees (* 1000)~%int32 yaw_from~%~%# camera pan and tilt angles during waypoint travel given in degrees (* 1000)~%int32 pan_angle~%int32 tilt_angle~%~%# Type of Waypoint~%uint8 waypoint_type~%~%uint8 TYPE_NAV = 1~%uint8 TYPE_CONDITION_YAW = 2~%uint8 TYPE_CONDITION_CHANGE_ALT = 3~%uint8 TYPE_TAKEOFF = 4~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Waypoint>))
  (cl:+ 0
     4
     4
     4
     2
     1
     2
     4
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Waypoint>))
  "Converts a ROS message object to a list"
  (cl:list 'Waypoint
    (cl:cons ':latitude (latitude msg))
    (cl:cons ':longitude (longitude msg))
    (cl:cons ':altitude (altitude msg))
    (cl:cons ':pos_acc (pos_acc msg))
    (cl:cons ':speed_to (speed_to msg))
    (cl:cons ':hold_time (hold_time msg))
    (cl:cons ':yaw_from (yaw_from msg))
    (cl:cons ':pan_angle (pan_angle msg))
    (cl:cons ':tilt_angle (tilt_angle msg))
    (cl:cons ':waypoint_type (waypoint_type msg))
))
