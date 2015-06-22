; Auto-generated. Do not edit!


(cl:in-package casy_rover-msg)


;//! \htmlinclude RC.msg.html

(cl:defclass <RC> (roslisp-msg-protocol:ros-message)
  ((channel
    :reader channel
    :initarg :channel
    :type (cl:vector cl:integer)
   :initform (cl:make-array 8 :element-type 'cl:integer :initial-element 0)))
)

(cl:defclass RC (<RC>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RC>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RC)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name casy_rover-msg:<RC> is deprecated: use casy_rover-msg:RC instead.")))

(cl:ensure-generic-function 'channel-val :lambda-list '(m))
(cl:defmethod channel-val ((m <RC>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader casy_rover-msg:channel-val is deprecated.  Use casy_rover-msg:channel instead.")
  (channel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RC>) ostream)
  "Serializes a message object of type '<RC>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) ele) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) ele) ostream))
   (cl:slot-value msg 'channel))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RC>) istream)
  "Deserializes a message object of type '<RC>"
  (cl:setf (cl:slot-value msg 'channel) (cl:make-array 8))
  (cl:let ((vals (cl:slot-value msg 'channel)))
    (cl:dotimes (i 8)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:aref vals i)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:aref vals i)) (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RC>)))
  "Returns string type for a message object of type '<RC>"
  "casy_rover/RC")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RC)))
  "Returns string type for a message object of type 'RC"
  "casy_rover/RC")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RC>)))
  "Returns md5sum for a message object of type '<RC>"
  "a40d8c478f8d8fdeb5a2f988302e5696")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RC)))
  "Returns md5sum for a message object of type 'RC"
  "a40d8c478f8d8fdeb5a2f988302e5696")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RC>)))
  "Returns full string definition for message of type '<RC>"
  (cl:format cl:nil "# rc channels~%~%uint32[8] channel~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RC)))
  "Returns full string definition for message of type 'RC"
  (cl:format cl:nil "# rc channels~%~%uint32[8] channel~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RC>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'channel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RC>))
  "Converts a ROS message object to a list"
  (cl:list 'RC
    (cl:cons ':channel (channel msg))
))
