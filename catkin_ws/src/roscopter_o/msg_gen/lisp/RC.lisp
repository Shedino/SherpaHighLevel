; Auto-generated. Do not edit!


(cl:in-package roscopter-msg)


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
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name roscopter-msg:<RC> is deprecated: use roscopter-msg:RC instead.")))

(cl:ensure-generic-function 'channel-val :lambda-list '(m))
(cl:defmethod channel-val ((m <RC>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader roscopter-msg:channel-val is deprecated.  Use roscopter-msg:channel instead.")
  (channel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RC>) ostream)
  "Serializes a message object of type '<RC>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'channel))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RC>) istream)
  "Deserializes a message object of type '<RC>"
  (cl:setf (cl:slot-value msg 'channel) (cl:make-array 8))
  (cl:let ((vals (cl:slot-value msg 'channel)))
    (cl:dotimes (i 8)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RC>)))
  "Returns string type for a message object of type '<RC>"
  "roscopter/RC")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RC)))
  "Returns string type for a message object of type 'RC"
  "roscopter/RC")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RC>)))
  "Returns md5sum for a message object of type '<RC>"
  "0f8285b6fdeccee6bef1d0353358d5cf")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RC)))
  "Returns md5sum for a message object of type 'RC"
  "0f8285b6fdeccee6bef1d0353358d5cf")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RC>)))
  "Returns full string definition for message of type '<RC>"
  (cl:format cl:nil "# rc channels~%~%int32[8] channel~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RC)))
  "Returns full string definition for message of type 'RC"
  (cl:format cl:nil "# rc channels~%~%int32[8] channel~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RC>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'channel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RC>))
  "Converts a ROS message object to a list"
  (cl:list 'RC
    (cl:cons ':channel (channel msg))
))
