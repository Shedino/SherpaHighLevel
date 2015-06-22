; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude FilteredPosition.msg.html

(cl:defclass <FilteredPosition> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (latitude
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
   (relative_altitude
    :reader relative_altitude
    :initarg :relative_altitude
    :type cl:integer
    :initform 0)
   (ground_x_speed
    :reader ground_x_speed
    :initarg :ground_x_speed
    :type cl:fixnum
    :initform 0)
   (ground_y_speed
    :reader ground_y_speed
    :initarg :ground_y_speed
    :type cl:fixnum
    :initform 0)
   (ground_z_speed
    :reader ground_z_speed
    :initarg :ground_z_speed
    :type cl:fixnum
    :initform 0)
   (heading
    :reader heading
    :initarg :heading
    :type cl:fixnum
    :initform 0))
)

(cl:defclass FilteredPosition (<FilteredPosition>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <FilteredPosition>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'FilteredPosition)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<FilteredPosition> is deprecated: use casy_rover-msg:FilteredPosition instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:header-val is deprecated.  Use casy_rover-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'latitude-val :lambda-list '(m))
(cl:defmethod latitude-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:latitude-val is deprecated.  Use casy_rover-msg:latitude instead.")
  (latitude m))

(cl:ensure-generic-function 'longitude-val :lambda-list '(m))
(cl:defmethod longitude-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:longitude-val is deprecated.  Use casy_rover-msg:longitude instead.")
  (longitude m))

(cl:ensure-generic-function 'altitude-val :lambda-list '(m))
(cl:defmethod altitude-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:altitude-val is deprecated.  Use casy_rover-msg:altitude instead.")
  (altitude m))

(cl:ensure-generic-function 'relative_altitude-val :lambda-list '(m))
(cl:defmethod relative_altitude-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:relative_altitude-val is deprecated.  Use casy_rover-msg:relative_altitude instead.")
  (relative_altitude m))

(cl:ensure-generic-function 'ground_x_speed-val :lambda-list '(m))
(cl:defmethod ground_x_speed-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:ground_x_speed-val is deprecated.  Use casy_rover-msg:ground_x_speed instead.")
  (ground_x_speed m))

(cl:ensure-generic-function 'ground_y_speed-val :lambda-list '(m))
(cl:defmethod ground_y_speed-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:ground_y_speed-val is deprecated.  Use casy_rover-msg:ground_y_speed instead.")
  (ground_y_speed m))

(cl:ensure-generic-function 'ground_z_speed-val :lambda-list '(m))
(cl:defmethod ground_z_speed-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:ground_z_speed-val is deprecated.  Use casy_rover-msg:ground_z_speed instead.")
  (ground_z_speed m))

(cl:ensure-generic-function 'heading-val :lambda-list '(m))
(cl:defmethod heading-val ((m <FilteredPosition>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:heading-val is deprecated.  Use casy_rover-msg:heading instead.")
  (heading m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <FilteredPosition>) ostream)
  "Serializes a message object of type '<FilteredPosition>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
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
  (cl:let* ((signed (cl:slot-value msg 'relative_altitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ground_x_speed)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ground_y_speed)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ground_z_speed)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'heading)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'heading)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <FilteredPosition>) istream)
  "Deserializes a message object of type '<FilteredPosition>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
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
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'relative_altitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ground_x_speed) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ground_y_speed) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ground_z_speed) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'heading)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'heading)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<FilteredPosition>)))
  "Returns string type for a message object of type '<FilteredPosition>"
  "casy_rover/FilteredPosition")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'FilteredPosition)))
  "Returns string type for a message object of type 'FilteredPosition"
  "casy_rover/FilteredPosition")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<FilteredPosition>)))
  "Returns md5sum for a message object of type '<FilteredPosition>"
  "c085fbfb9542cd7c5704f043b5e23e5c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'FilteredPosition)))
  "Returns md5sum for a message object of type 'FilteredPosition"
  "c085fbfb9542cd7c5704f043b5e23e5c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<FilteredPosition>)))
  "Returns full string definition for message of type '<FilteredPosition>"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: GLOBAL_POSITION_INT~%# See common.xml in MAVLink Messages~%~%Header header~%~%# Latitude and Longitude given in deg * 10^7~%int32 latitude~%int32 longitude~%~%# Altitude given as mm~%int32 altitude~%int32 relative_altitude~%~%# Ground Speed along X, Y, and Z expressed as m/s * 100~%int16 ground_x_speed~%int16 ground_y_speed~%int16 ground_z_speed~%~%# Heading in degrees * 100 (set to 65536 if false)~%uint16 heading~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'FilteredPosition)))
  "Returns full string definition for message of type 'FilteredPosition"
  (cl:format cl:nil "# Based on MAVLink 1.0 Messages: GLOBAL_POSITION_INT~%# See common.xml in MAVLink Messages~%~%Header header~%~%# Latitude and Longitude given in deg * 10^7~%int32 latitude~%int32 longitude~%~%# Altitude given as mm~%int32 altitude~%int32 relative_altitude~%~%# Ground Speed along X, Y, and Z expressed as m/s * 100~%int16 ground_x_speed~%int16 ground_y_speed~%int16 ground_z_speed~%~%# Heading in degrees * 100 (set to 65536 if false)~%uint16 heading~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <FilteredPosition>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     4
     4
     4
     2
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <FilteredPosition>))
  "Converts a ROS message object to a list"
  (cl:list 'FilteredPosition
    (cl:cons ':header (header msg))
    (cl:cons ':latitude (latitude msg))
    (cl:cons ':longitude (longitude msg))
    (cl:cons ':altitude (altitude msg))
    (cl:cons ':relative_altitude (relative_altitude msg))
    (cl:cons ':ground_x_speed (ground_x_speed msg))
    (cl:cons ':ground_y_speed (ground_y_speed msg))
    (cl:cons ':ground_z_speed (ground_z_speed msg))
    (cl:cons ':heading (heading msg))
))
