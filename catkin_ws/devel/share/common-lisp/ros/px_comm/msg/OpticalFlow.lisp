; Auto-generated. Do not edit!


(cl:in-package px_comm-msg)


;//! \htmlinclude OpticalFlow.msg.html

(cl:defclass <OpticalFlow> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (ground_distance
    :reader ground_distance
    :initarg :ground_distance
    :type cl:float
    :initform 0.0)
   (flow_x
    :reader flow_x
    :initarg :flow_x
    :type cl:fixnum
    :initform 0)
   (flow_y
    :reader flow_y
    :initarg :flow_y
    :type cl:fixnum
    :initform 0)
   (velocity_x
    :reader velocity_x
    :initarg :velocity_x
    :type cl:float
    :initform 0.0)
   (velocity_y
    :reader velocity_y
    :initarg :velocity_y
    :type cl:float
    :initform 0.0)
   (quality
    :reader quality
    :initarg :quality
    :type cl:fixnum
    :initform 0))
)

(cl:defclass OpticalFlow (<OpticalFlow>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <OpticalFlow>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'OpticalFlow)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name px_comm-msg:<OpticalFlow> is deprecated: use px_comm-msg:OpticalFlow instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:header-val is deprecated.  Use px_comm-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'ground_distance-val :lambda-list '(m))
(cl:defmethod ground_distance-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:ground_distance-val is deprecated.  Use px_comm-msg:ground_distance instead.")
  (ground_distance m))

(cl:ensure-generic-function 'flow_x-val :lambda-list '(m))
(cl:defmethod flow_x-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:flow_x-val is deprecated.  Use px_comm-msg:flow_x instead.")
  (flow_x m))

(cl:ensure-generic-function 'flow_y-val :lambda-list '(m))
(cl:defmethod flow_y-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:flow_y-val is deprecated.  Use px_comm-msg:flow_y instead.")
  (flow_y m))

(cl:ensure-generic-function 'velocity_x-val :lambda-list '(m))
(cl:defmethod velocity_x-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:velocity_x-val is deprecated.  Use px_comm-msg:velocity_x instead.")
  (velocity_x m))

(cl:ensure-generic-function 'velocity_y-val :lambda-list '(m))
(cl:defmethod velocity_y-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:velocity_y-val is deprecated.  Use px_comm-msg:velocity_y instead.")
  (velocity_y m))

(cl:ensure-generic-function 'quality-val :lambda-list '(m))
(cl:defmethod quality-val ((m <OpticalFlow>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-msg:quality-val is deprecated.  Use px_comm-msg:quality instead.")
  (quality m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <OpticalFlow>) ostream)
  "Serializes a message object of type '<OpticalFlow>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'ground_distance))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'flow_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'flow_y)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'velocity_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'velocity_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'quality)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <OpticalFlow>) istream)
  "Deserializes a message object of type '<OpticalFlow>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'ground_distance) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'flow_x) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'flow_y) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'velocity_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'velocity_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'quality)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<OpticalFlow>)))
  "Returns string type for a message object of type '<OpticalFlow>"
  "px_comm/OpticalFlow")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'OpticalFlow)))
  "Returns string type for a message object of type 'OpticalFlow"
  "px_comm/OpticalFlow")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<OpticalFlow>)))
  "Returns md5sum for a message object of type '<OpticalFlow>"
  "6705fe0e94fea14978a508d00cf97427")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'OpticalFlow)))
  "Returns md5sum for a message object of type 'OpticalFlow"
  "6705fe0e94fea14978a508d00cf97427")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<OpticalFlow>)))
  "Returns full string definition for message of type '<OpticalFlow>"
  (cl:format cl:nil "Header header~%~%float32 ground_distance  # distance to ground in meters~%int16   flow_x           # x-component of optical flow in pixels~%int16   flow_y           # y-component of optical flow in pixels~%float32 velocity_x       # x-component of scaled optical flow in m/s~%float32 velocity_y       # y-component of scaled optical flow in m/s~%uint8   quality          # quality of optical flow estimate~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'OpticalFlow)))
  "Returns full string definition for message of type 'OpticalFlow"
  (cl:format cl:nil "Header header~%~%float32 ground_distance  # distance to ground in meters~%int16   flow_x           # x-component of optical flow in pixels~%int16   flow_y           # y-component of optical flow in pixels~%float32 velocity_x       # x-component of scaled optical flow in m/s~%float32 velocity_y       # y-component of scaled optical flow in m/s~%uint8   quality          # quality of optical flow estimate~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <OpticalFlow>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     2
     2
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <OpticalFlow>))
  "Converts a ROS message object to a list"
  (cl:list 'OpticalFlow
    (cl:cons ':header (header msg))
    (cl:cons ':ground_distance (ground_distance msg))
    (cl:cons ':flow_x (flow_x msg))
    (cl:cons ':flow_y (flow_y msg))
    (cl:cons ':velocity_x (velocity_x msg))
    (cl:cons ':velocity_y (velocity_y msg))
    (cl:cons ':quality (quality msg))
))
