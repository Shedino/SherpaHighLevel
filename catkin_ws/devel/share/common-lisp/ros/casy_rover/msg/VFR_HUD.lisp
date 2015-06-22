; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude VFR_HUD.msg.html

(cl:defclass <VFR_HUD> (roslisp-msg-protocol:ros-message)
  ((airspeed
    :reader airspeed
    :initarg :airspeed
    :type cl:float
    :initform 0.0)
   (groundspeed
    :reader groundspeed
    :initarg :groundspeed
    :type cl:float
    :initform 0.0)
   (heading
    :reader heading
    :initarg :heading
    :type cl:fixnum
    :initform 0)
   (throttle
    :reader throttle
    :initarg :throttle
    :type cl:fixnum
    :initform 0)
   (alt
    :reader alt
    :initarg :alt
    :type cl:float
    :initform 0.0)
   (climb
    :reader climb
    :initarg :climb
    :type cl:float
    :initform 0.0))
)

(cl:defclass VFR_HUD (<VFR_HUD>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <VFR_HUD>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'VFR_HUD)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<VFR_HUD> is deprecated: use casy_rover-msg:VFR_HUD instead.")))

(cl:ensure-generic-function 'airspeed-val :lambda-list '(m))
(cl:defmethod airspeed-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:airspeed-val is deprecated.  Use casy_rover-msg:airspeed instead.")
  (airspeed m))

(cl:ensure-generic-function 'groundspeed-val :lambda-list '(m))
(cl:defmethod groundspeed-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:groundspeed-val is deprecated.  Use casy_rover-msg:groundspeed instead.")
  (groundspeed m))

(cl:ensure-generic-function 'heading-val :lambda-list '(m))
(cl:defmethod heading-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:heading-val is deprecated.  Use casy_rover-msg:heading instead.")
  (heading m))

(cl:ensure-generic-function 'throttle-val :lambda-list '(m))
(cl:defmethod throttle-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:throttle-val is deprecated.  Use casy_rover-msg:throttle instead.")
  (throttle m))

(cl:ensure-generic-function 'alt-val :lambda-list '(m))
(cl:defmethod alt-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:alt-val is deprecated.  Use casy_rover-msg:alt instead.")
  (alt m))

(cl:ensure-generic-function 'climb-val :lambda-list '(m))
(cl:defmethod climb-val ((m <VFR_HUD>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:climb-val is deprecated.  Use casy_rover-msg:climb instead.")
  (climb m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <VFR_HUD>) ostream)
  "Serializes a message object of type '<VFR_HUD>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'airspeed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'groundspeed))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'heading)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'throttle)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'throttle)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'alt))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'climb))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <VFR_HUD>) istream)
  "Deserializes a message object of type '<VFR_HUD>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'airspeed) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'groundspeed) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'heading) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'throttle)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'throttle)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'alt) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'climb) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<VFR_HUD>)))
  "Returns string type for a message object of type '<VFR_HUD>"
  "casy_rover/VFR_HUD")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'VFR_HUD)))
  "Returns string type for a message object of type 'VFR_HUD"
  "casy_rover/VFR_HUD")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<VFR_HUD>)))
  "Returns md5sum for a message object of type '<VFR_HUD>"
  "d675e2b95b45d51cd67453c819986216")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'VFR_HUD)))
  "Returns md5sum for a message object of type 'VFR_HUD"
  "d675e2b95b45d51cd67453c819986216")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<VFR_HUD>)))
  "Returns full string definition for message of type '<VFR_HUD>"
  (cl:format cl:nil "# Metrics typically displayed on a HUD for fixed wing aircraft        ~%#airspeed                  : Current airspeed in m/s (float)~%#groundspeed               : Current ground speed in m/s (float)~%#heading                   : Current heading in degrees, in compass units (0..360, 0=north) (int16_t)  ~%#throttle                  : Current throttle setting in integer percent, 0 to 100 (uint16_t)  ~%#alt                       : Current altitude (MSL), in meters (float)~%#climb                     : Current climb rate in meters/second (float)             ~%~%float32 airspeed ~%float32 groundspeed ~%int16 heading~%uint16 throttle~%float32 alt ~%float32 climb~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'VFR_HUD)))
  "Returns full string definition for message of type 'VFR_HUD"
  (cl:format cl:nil "# Metrics typically displayed on a HUD for fixed wing aircraft        ~%#airspeed                  : Current airspeed in m/s (float)~%#groundspeed               : Current ground speed in m/s (float)~%#heading                   : Current heading in degrees, in compass units (0..360, 0=north) (int16_t)  ~%#throttle                  : Current throttle setting in integer percent, 0 to 100 (uint16_t)  ~%#alt                       : Current altitude (MSL), in meters (float)~%#climb                     : Current climb rate in meters/second (float)             ~%~%float32 airspeed ~%float32 groundspeed ~%int16 heading~%uint16 throttle~%float32 alt ~%float32 climb~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <VFR_HUD>))
  (cl:+ 0
     4
     4
     2
     2
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <VFR_HUD>))
  "Converts a ROS message object to a list"
  (cl:list 'VFR_HUD
    (cl:cons ':airspeed (airspeed msg))
    (cl:cons ':groundspeed (groundspeed msg))
    (cl:cons ':heading (heading msg))
    (cl:cons ':throttle (throttle msg))
    (cl:cons ':alt (alt msg))
    (cl:cons ':climb (climb msg))
))
