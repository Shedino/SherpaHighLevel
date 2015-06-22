; Auto-generated. Do not edit!


(cl:in-package mms-msg)


;//! \htmlinclude Ack_arm.msg.html

(cl:defclass <Ack_arm> (roslisp-msg-protocol:ros-message)
  ((mav_result
    :reader mav_result
    :initarg :mav_result
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass Ack_arm (<Ack_arm>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Ack_arm>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Ack_arm)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mms-msg:<Ack_arm> is deprecated: use mms-msg:Ack_arm instead.")))

(cl:ensure-generic-function 'mav_result-val :lambda-list '(m))
(cl:defmethod mav_result-val ((m <Ack_arm>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:mav_result-val is deprecated.  Use mms-msg:mav_result instead.")
  (mav_result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Ack_arm>) ostream)
  "Serializes a message object of type '<Ack_arm>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'mav_result) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Ack_arm>) istream)
  "Deserializes a message object of type '<Ack_arm>"
    (cl:setf (cl:slot-value msg 'mav_result) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Ack_arm>)))
  "Returns string type for a message object of type '<Ack_arm>"
  "mms/Ack_arm")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Ack_arm)))
  "Returns string type for a message object of type 'Ack_arm"
  "mms/Ack_arm")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Ack_arm>)))
  "Returns md5sum for a message object of type '<Ack_arm>"
  "3e2f901d362039bf545384d626e33dae")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Ack_arm)))
  "Returns md5sum for a message object of type 'Ack_arm"
  "3e2f901d362039bf545384d626e33dae")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Ack_arm>)))
  "Returns full string definition for message of type '<Ack_arm>"
  (cl:format cl:nil "# false = cmd not executed, true = cmd accepted and executed~%bool mav_result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Ack_arm)))
  "Returns full string definition for message of type 'Ack_arm"
  (cl:format cl:nil "# false = cmd not executed, true = cmd accepted and executed~%bool mav_result~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Ack_arm>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Ack_arm>))
  "Converts a ROS message object to a list"
  (cl:list 'Ack_arm
    (cl:cons ':mav_result (mav_result msg))
))
