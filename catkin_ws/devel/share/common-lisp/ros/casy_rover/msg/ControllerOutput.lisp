; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude ControllerOutput.msg.html

(cl:defclass <ControllerOutput> (roslisp-msg-protocol:ros-message)
  ((nav_roll
    :reader nav_roll
    :initarg :nav_roll
    :type cl:float
    :initform 0.0)
   (nav_pitch
    :reader nav_pitch
    :initarg :nav_pitch
    :type cl:float
    :initform 0.0)
   (nav_bearing
    :reader nav_bearing
    :initarg :nav_bearing
    :type cl:fixnum
    :initform 0)
   (alt_error
    :reader alt_error
    :initarg :alt_error
    :type cl:float
    :initform 0.0)
   (air_spd_error
    :reader air_spd_error
    :initarg :air_spd_error
    :type cl:float
    :initform 0.0)
   (xtrack_error
    :reader xtrack_error
    :initarg :xtrack_error
    :type cl:float
    :initform 0.0))
)

(cl:defclass ControllerOutput (<ControllerOutput>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ControllerOutput>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ControllerOutput)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<ControllerOutput> is deprecated: use casy_rover-msg:ControllerOutput instead.")))

(cl:ensure-generic-function 'nav_roll-val :lambda-list '(m))
(cl:defmethod nav_roll-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:nav_roll-val is deprecated.  Use casy_rover-msg:nav_roll instead.")
  (nav_roll m))

(cl:ensure-generic-function 'nav_pitch-val :lambda-list '(m))
(cl:defmethod nav_pitch-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:nav_pitch-val is deprecated.  Use casy_rover-msg:nav_pitch instead.")
  (nav_pitch m))

(cl:ensure-generic-function 'nav_bearing-val :lambda-list '(m))
(cl:defmethod nav_bearing-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:nav_bearing-val is deprecated.  Use casy_rover-msg:nav_bearing instead.")
  (nav_bearing m))

(cl:ensure-generic-function 'alt_error-val :lambda-list '(m))
(cl:defmethod alt_error-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:alt_error-val is deprecated.  Use casy_rover-msg:alt_error instead.")
  (alt_error m))

(cl:ensure-generic-function 'air_spd_error-val :lambda-list '(m))
(cl:defmethod air_spd_error-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:air_spd_error-val is deprecated.  Use casy_rover-msg:air_spd_error instead.")
  (air_spd_error m))

(cl:ensure-generic-function 'xtrack_error-val :lambda-list '(m))
(cl:defmethod xtrack_error-val ((m <ControllerOutput>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:xtrack_error-val is deprecated.  Use casy_rover-msg:xtrack_error instead.")
  (xtrack_error m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ControllerOutput>) ostream)
  "Serializes a message object of type '<ControllerOutput>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'nav_roll))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'nav_pitch))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'nav_bearing)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'alt_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'air_spd_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'xtrack_error))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ControllerOutput>) istream)
  "Deserializes a message object of type '<ControllerOutput>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'nav_roll) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'nav_pitch) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'nav_bearing) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'alt_error) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'air_spd_error) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'xtrack_error) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ControllerOutput>)))
  "Returns string type for a message object of type '<ControllerOutput>"
  "casy_rover/ControllerOutput")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ControllerOutput)))
  "Returns string type for a message object of type 'ControllerOutput"
  "casy_rover/ControllerOutput")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ControllerOutput>)))
  "Returns md5sum for a message object of type '<ControllerOutput>"
  "14e8f32f6dc13cd7db7fdae805a6677b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ControllerOutput)))
  "Returns md5sum for a message object of type 'ControllerOutput"
  "14e8f32f6dc13cd7db7fdae805a6677b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ControllerOutput>)))
  "Returns full string definition for message of type '<ControllerOutput>"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: NAV_CONTROLLER_OUTPUT~%# See common.xml in MAVLink Messages~%~%# Outputs of the ArduPilot Mega controller.~%~%# Current Desired Roll, Pitch, and Yaw in degrees~%float32 nav_roll~%float32 nav_pitch~%int16 nav_bearing~%~%# Current error in altitude, airspeed, and crosstrack~%float32 alt_error~%float32 air_spd_error~%float32 xtrack_error~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ControllerOutput)))
  "Returns full string definition for message of type 'ControllerOutput"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: NAV_CONTROLLER_OUTPUT~%# See common.xml in MAVLink Messages~%~%# Outputs of the ArduPilot Mega controller.~%~%# Current Desired Roll, Pitch, and Yaw in degrees~%float32 nav_roll~%float32 nav_pitch~%int16 nav_bearing~%~%# Current error in altitude, airspeed, and crosstrack~%float32 alt_error~%float32 air_spd_error~%float32 xtrack_error~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ControllerOutput>))
  (cl:+ 0
     4
     4
     2
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ControllerOutput>))
  "Converts a ROS message object to a list"
  (cl:list 'ControllerOutput
    (cl:cons ':nav_roll (nav_roll msg))
    (cl:cons ':nav_pitch (nav_pitch msg))
    (cl:cons ':nav_bearing (nav_bearing msg))
    (cl:cons ':alt_error (alt_error msg))
    (cl:cons ':air_spd_error (air_spd_error msg))
    (cl:cons ':xtrack_error (xtrack_error msg))
))
