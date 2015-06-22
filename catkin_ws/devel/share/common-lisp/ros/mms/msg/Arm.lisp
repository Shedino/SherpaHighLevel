; Auto-generated. Do not edit!


(cl:in-package mms-msg)


;//! \htmlinclude Arm.msg.html

(cl:defclass <Arm> (roslisp-msg-protocol:ros-message)
  ((arm_disarm
    :reader arm_disarm
    :initarg :arm_disarm
    :type cl:boolean
    :initform cl:nil)
   (new_arm_disarm
    :reader new_arm_disarm
    :initarg :new_arm_disarm
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass Arm (<Arm>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Arm>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Arm)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mms-msg:<Arm> is deprecated: use mms-msg:Arm instead.")))

(cl:ensure-generic-function 'arm_disarm-val :lambda-list '(m))
(cl:defmethod arm_disarm-val ((m <Arm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:arm_disarm-val is deprecated.  Use mms-msg:arm_disarm instead.")
  (arm_disarm m))

(cl:ensure-generic-function 'new_arm_disarm-val :lambda-list '(m))
(cl:defmethod new_arm_disarm-val ((m <Arm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:new_arm_disarm-val is deprecated.  Use mms-msg:new_arm_disarm instead.")
  (new_arm_disarm m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Arm>) ostream)
  "Serializes a message object of type '<Arm>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'arm_disarm) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'new_arm_disarm) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Arm>) istream)
  "Deserializes a message object of type '<Arm>"
    (cl:setf (cl:slot-value msg 'arm_disarm) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'new_arm_disarm) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Arm>)))
  "Returns string type for a message object of type '<Arm>"
  "mms/Arm")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Arm)))
  "Returns string type for a message object of type 'Arm"
  "mms/Arm")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Arm>)))
  "Returns md5sum for a message object of type '<Arm>"
  "694dce2fa7eb0edc65e00dea01480dec")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Arm)))
  "Returns md5sum for a message object of type 'Arm"
  "694dce2fa7eb0edc65e00dea01480dec")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Arm>)))
  "Returns full string definition for message of type '<Arm>"
  (cl:format cl:nil "# false = disarm; true = arm~%bool arm_disarm~%# false = old message; true = new message available~%bool new_arm_disarm~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Arm)))
  "Returns full string definition for message of type 'Arm"
  (cl:format cl:nil "# false = disarm; true = arm~%bool arm_disarm~%# false = old message; true = new message available~%bool new_arm_disarm~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Arm>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Arm>))
  "Converts a ROS message object to a list"
  (cl:list 'Arm
    (cl:cons ':arm_disarm (arm_disarm msg))
    (cl:cons ':new_arm_disarm (new_arm_disarm msg))
))
