; Auto-generated. Do not edit!


(cl:in-package guidance_node_amsl-msg)


;//! \htmlinclude Directive.msg.html

(cl:defclass <Directive> (roslisp-msg-protocol:ros-message)
  ((vxBody
    :reader vxBody
    :initarg :vxBody
    :type cl:float
    :initform 0.0)
   (vyBody
    :reader vyBody
    :initarg :vyBody
    :type cl:float
    :initform 0.0)
   (vzBody
    :reader vzBody
    :initarg :vzBody
    :type cl:float
    :initform 0.0)
   (yawRate
    :reader yawRate
    :initarg :yawRate
    :type cl:float
    :initform 0.0))
)

(cl:defclass Directive (<Directive>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Directive>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Directive)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name guidance_node_amsl-msg:<Directive> is deprecated: use guidance_node_amsl-msg:Directive instead.")))

(cl:ensure-generic-function 'vxBody-val :lambda-list '(m))
(cl:defmethod vxBody-val ((m <Directive>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node_amsl-msg:vxBody-val is deprecated.  Use guidance_node_amsl-msg:vxBody instead.")
  (vxBody m))

(cl:ensure-generic-function 'vyBody-val :lambda-list '(m))
(cl:defmethod vyBody-val ((m <Directive>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node_amsl-msg:vyBody-val is deprecated.  Use guidance_node_amsl-msg:vyBody instead.")
  (vyBody m))

(cl:ensure-generic-function 'vzBody-val :lambda-list '(m))
(cl:defmethod vzBody-val ((m <Directive>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node_amsl-msg:vzBody-val is deprecated.  Use guidance_node_amsl-msg:vzBody instead.")
  (vzBody m))

(cl:ensure-generic-function 'yawRate-val :lambda-list '(m))
(cl:defmethod yawRate-val ((m <Directive>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader guidance_node_amsl-msg:yawRate-val is deprecated.  Use guidance_node_amsl-msg:yawRate instead.")
  (yawRate m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Directive>) ostream)
  "Serializes a message object of type '<Directive>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'vxBody))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'vyBody))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'vzBody))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'yawRate))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Directive>) istream)
  "Deserializes a message object of type '<Directive>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'vxBody) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'vyBody) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'vzBody) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'yawRate) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Directive>)))
  "Returns string type for a message object of type '<Directive>"
  "guidance_node_amsl/Directive")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Directive)))
  "Returns string type for a message object of type 'Directive"
  "guidance_node_amsl/Directive")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Directive>)))
  "Returns md5sum for a message object of type '<Directive>"
  "f93f58e453a8eaf566deb1c6335e59ce")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Directive)))
  "Returns md5sum for a message object of type 'Directive"
  "f93f58e453a8eaf566deb1c6335e59ce")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Directive>)))
  "Returns full string definition for message of type '<Directive>"
  (cl:format cl:nil "float32 vxBody~%float32 vyBody~%float32 vzBody~%float32 yawRate~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Directive)))
  "Returns full string definition for message of type 'Directive"
  (cl:format cl:nil "float32 vxBody~%float32 vyBody~%float32 vzBody~%float32 yawRate~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Directive>))
  (cl:+ 0
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Directive>))
  "Converts a ROS message object to a list"
  (cl:list 'Directive
    (cl:cons ':vxBody (vxBody msg))
    (cl:cons ':vyBody (vyBody msg))
    (cl:cons ':vzBody (vzBody msg))
    (cl:cons ':yawRate (yawRate msg))
))
