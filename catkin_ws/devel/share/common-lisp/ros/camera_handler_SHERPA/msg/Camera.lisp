; Auto-generated. Do not edit!


(cl:in-package camera_handler_SHERPA-msg)


;//! \htmlinclude Camera.msg.html

(cl:defclass <Camera> (roslisp-msg-protocol:ros-message)
  ((take_pic
    :reader take_pic
    :initarg :take_pic
    :type cl:boolean
    :initform cl:nil)
   (take_vid
    :reader take_vid
    :initarg :take_vid
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass Camera (<Camera>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Camera>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Camera)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name camera_handler_SHERPA-msg:<Camera> is deprecated: use camera_handler_SHERPA-msg:Camera instead.")))

(cl:ensure-generic-function 'take_pic-val :lambda-list '(m))
(cl:defmethod take_pic-val ((m <Camera>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_handler_SHERPA-msg:take_pic-val is deprecated.  Use camera_handler_SHERPA-msg:take_pic instead.")
  (take_pic m))

(cl:ensure-generic-function 'take_vid-val :lambda-list '(m))
(cl:defmethod take_vid-val ((m <Camera>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader camera_handler_SHERPA-msg:take_vid-val is deprecated.  Use camera_handler_SHERPA-msg:take_vid instead.")
  (take_vid m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Camera>) ostream)
  "Serializes a message object of type '<Camera>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'take_pic) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'take_vid) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Camera>) istream)
  "Deserializes a message object of type '<Camera>"
    (cl:setf (cl:slot-value msg 'take_pic) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'take_vid) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Camera>)))
  "Returns string type for a message object of type '<Camera>"
  "camera_handler_SHERPA/Camera")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Camera)))
  "Returns string type for a message object of type 'Camera"
  "camera_handler_SHERPA/Camera")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Camera>)))
  "Returns md5sum for a message object of type '<Camera>"
  "ef44d9a6b4c587ef4ea335ff23618ea6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Camera)))
  "Returns md5sum for a message object of type 'Camera"
  "ef44d9a6b4c587ef4ea335ff23618ea6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Camera>)))
  "Returns full string definition for message of type '<Camera>"
  (cl:format cl:nil "# true when requested a picture and false after the picture is taken~%bool take_pic~%# true when starting a video and false when video is stopped~%bool take_vid~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Camera)))
  "Returns full string definition for message of type 'Camera"
  (cl:format cl:nil "# true when requested a picture and false after the picture is taken~%bool take_pic~%# true when starting a video and false when video is stopped~%bool take_vid~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Camera>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Camera>))
  "Converts a ROS message object to a list"
  (cl:list 'Camera
    (cl:cons ':take_pic (take_pic msg))
    (cl:cons ':take_vid (take_vid msg))
))
