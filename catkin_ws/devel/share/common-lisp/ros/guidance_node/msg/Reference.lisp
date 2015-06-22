; Auto-generated. Do not edit!


(cl:in-package guidance_node-msg)


;//! \htmlinclude Reference.msg.html

(cl:defclass <Reference> (roslisp-msg-protocol:ros-message)
  ((Latitude
    :reader Latitude
    :initarg :Latitude
    :type cl:integer
    :initform 0)
   (Longitude
    :reader Longitude
    :initarg :Longitude
    :type cl:integer
    :initform 0)
   (AltitudeRelative
    :reader AltitudeRelative
    :initarg :AltitudeRelative
    :type cl:integer
    :initform 0)
   (Yawangle
    :reader Yawangle
    :initarg :Yawangle
    :type cl:float
    :initform 0.0)
   (Mode
    :reader Mode
    :initarg :Mode
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Reference (<Reference>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Reference>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Reference)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name guidance_node-msg:<Reference> is deprecated: use guidance_node-msg:Reference instead.")))

(cl:ensure-generic-function 'Latitude-val :lambda-list '(m))
(cl:defmethod Latitude-val ((m <Reference>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node-msg:Latitude-val is deprecated.  Use guidance_node-msg:Latitude instead.")
  (Latitude m))

(cl:ensure-generic-function 'Longitude-val :lambda-list '(m))
(cl:defmethod Longitude-val ((m <Reference>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node-msg:Longitude-val is deprecated.  Use guidance_node-msg:Longitude instead.")
  (Longitude m))

(cl:ensure-generic-function 'AltitudeRelative-val :lambda-list '(m))
(cl:defmethod AltitudeRelative-val ((m <Reference>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node-msg:AltitudeRelative-val is deprecated.  Use guidance_node-msg:AltitudeRelative instead.")
  (AltitudeRelative m))

(cl:ensure-generic-function 'Yawangle-val :lambda-list '(m))
(cl:defmethod Yawangle-val ((m <Reference>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node-msg:Yawangle-val is deprecated.  Use guidance_node-msg:Yawangle instead.")
  (Yawangle m))

(cl:ensure-generic-function 'Mode-val :lambda-list '(m))
(cl:defmethod Mode-val ((m <Reference>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node-msg:Mode-val is deprecated.  Use guidance_node-msg:Mode instead.")
  (Mode m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Reference>) ostream)
  "Serializes a message object of type '<Reference>"
  (cl:let* ((signed (cl:slot-value msg 'Latitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'Longitude)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'AltitudeRelative)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'Yawangle))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'Mode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Reference>) istream)
  "Deserializes a message object of type '<Reference>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Latitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Longitude) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'AltitudeRelative) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'Yawangle) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Mode) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Reference>)))
  "Returns string type for a message object of type '<Reference>"
  "guidance_node/Reference")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Reference)))
  "Returns string type for a message object of type 'Reference"
  "guidance_node/Reference")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Reference>)))
  "Returns md5sum for a message object of type '<Reference>"
  "993e02597e619163c0d46925fb7696ce")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Reference)))
  "Returns md5sum for a message object of type 'Reference"
  "993e02597e619163c0d46925fb7696ce")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Reference>)))
  "Returns full string definition for message of type '<Reference>"
  (cl:format cl:nil "# expressed as * 1E7~%int32 Latitude~%# expressed as * 1E7~%int32 Longitude~%# expressed as * 1000 (millimeters)~%int32 AltitudeRelative~%# (-pi...pi)~%float32 Yawangle~%#reference abs or rel~%int8 Mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Reference)))
  "Returns full string definition for message of type 'Reference"
  (cl:format cl:nil "# expressed as * 1E7~%int32 Latitude~%# expressed as * 1E7~%int32 Longitude~%# expressed as * 1000 (millimeters)~%int32 AltitudeRelative~%# (-pi...pi)~%float32 Yawangle~%#reference abs or rel~%int8 Mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Reference>))
  (cl:+ 0
     4
     4
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Reference>))
  "Converts a ROS message object to a list"
  (cl:list 'Reference
    (cl:cons ':Latitude (Latitude msg))
    (cl:cons ':Longitude (Longitude msg))
    (cl:cons ':AltitudeRelative (AltitudeRelative msg))
    (cl:cons ':Yawangle (Yawangle msg))
    (cl:cons ':Mode (Mode msg))
))
