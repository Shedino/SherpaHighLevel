; Auto-generated. Do not edit!


(cl:in-package px_comm-srv)


;//! \htmlinclude SetCameraInfo-request.msg.html

(cl:defclass <SetCameraInfo-request> (roslisp-msg-protocol:ros-message)
  ((camera_info
    :reader camera_info
    :initarg :camera_info
    :type px_comm-msg:CameraInfo
    :initform (cl:make-instance 'px_comm-msg:CameraInfo)))
)

(cl:defclass SetCameraInfo-request (<SetCameraInfo-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetCameraInfo-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetCameraInfo-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name px_comm-srv:<SetCameraInfo-request> is deprecated: use px_comm-srv:SetCameraInfo-request instead.")))

(cl:ensure-generic-function 'camera_info-val :lambda-list '(m))
(cl:defmethod camera_info-val ((m <SetCameraInfo-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-srv:camera_info-val is deprecated.  Use px_comm-srv:camera_info instead.")
  (camera_info m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetCameraInfo-request>) ostream)
  "Serializes a message object of type '<SetCameraInfo-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'camera_info) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetCameraInfo-request>) istream)
  "Deserializes a message object of type '<SetCameraInfo-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'camera_info) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetCameraInfo-request>)))
  "Returns string type for a service object of type '<SetCameraInfo-request>"
  "px_comm/SetCameraInfoRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetCameraInfo-request)))
  "Returns string type for a service object of type 'SetCameraInfo-request"
  "px_comm/SetCameraInfoRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetCameraInfo-request>)))
  "Returns md5sum for a message object of type '<SetCameraInfo-request>"
  "b7b33d05bd0d56b83943d2370771da4c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetCameraInfo-request)))
  "Returns md5sum for a message object of type 'SetCameraInfo-request"
  "b7b33d05bd0d56b83943d2370771da4c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetCameraInfo-request>)))
  "Returns full string definition for message of type '<SetCameraInfo-request>"
  (cl:format cl:nil "~%~%~%~%~%~%~%~%CameraInfo camera_info~%~%================================================================================~%MSG: px_comm/CameraInfo~%#######################################################################~%#                     Image acquisition info                          #~%#######################################################################~%~%# Time of image acquisition, camera coordinate frame ID~%Header header    # Header timestamp should be acquisition time of image~%                 # Header frame_id should be optical frame of camera~%                 # origin of frame should be optical center of camera~%                 # +x should point to the right in the image~%                 # +y should point down in the image~%                 # +z should point into the plane of the image~%~%~%#######################################################################~%#                      Calibration Parameters                         #~%#######################################################################~%# These are fixed during camera calibration. Their values will be the #~%# same in all messages until the camera is recalibrated. Note that    #~%# self-calibrating systems may \"recalibrate\" frequently.              #~%#######################################################################~%~%# The camera model used.~%string camera_model~%~%# The name of the camera.~%string camera_name~%~%# The type of the camera.~%string camera_type~%~%# The image dimensions with which the camera was calibrated. Normally~%# this will be the full camera resolution in pixels.~%uint32 image_width~%uint32 image_height~%~%# The distortion parameters, size depending on the distortion model.~%float64[] D~%~%# The projection parameters, size depending on the projection model.~%float64[] P~%~%# Other parameters which are not defined by either the distortion or~%# projection model.~%float64[] M~%~%# Pose of camera with respect to a specific reference frame.~%geometry_msgs/Pose pose~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetCameraInfo-request)))
  "Returns full string definition for message of type 'SetCameraInfo-request"
  (cl:format cl:nil "~%~%~%~%~%~%~%~%CameraInfo camera_info~%~%================================================================================~%MSG: px_comm/CameraInfo~%#######################################################################~%#                     Image acquisition info                          #~%#######################################################################~%~%# Time of image acquisition, camera coordinate frame ID~%Header header    # Header timestamp should be acquisition time of image~%                 # Header frame_id should be optical frame of camera~%                 # origin of frame should be optical center of camera~%                 # +x should point to the right in the image~%                 # +y should point down in the image~%                 # +z should point into the plane of the image~%~%~%#######################################################################~%#                      Calibration Parameters                         #~%#######################################################################~%# These are fixed during camera calibration. Their values will be the #~%# same in all messages until the camera is recalibrated. Note that    #~%# self-calibrating systems may \"recalibrate\" frequently.              #~%#######################################################################~%~%# The camera model used.~%string camera_model~%~%# The name of the camera.~%string camera_name~%~%# The type of the camera.~%string camera_type~%~%# The image dimensions with which the camera was calibrated. Normally~%# this will be the full camera resolution in pixels.~%uint32 image_width~%uint32 image_height~%~%# The distortion parameters, size depending on the distortion model.~%float64[] D~%~%# The projection parameters, size depending on the projection model.~%float64[] P~%~%# Other parameters which are not defined by either the distortion or~%# projection model.~%float64[] M~%~%# Pose of camera with respect to a specific reference frame.~%geometry_msgs/Pose pose~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetCameraInfo-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'camera_info))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetCameraInfo-request>))
  "Converts a ROS message object to a list"
  (cl:list 'SetCameraInfo-request
    (cl:cons ':camera_info (camera_info msg))
))
;//! \htmlinclude SetCameraInfo-response.msg.html

(cl:defclass <SetCameraInfo-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (status_message
    :reader status_message
    :initarg :status_message
    :type cl:string
    :initform ""))
)

(cl:defclass SetCameraInfo-response (<SetCameraInfo-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetCameraInfo-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetCameraInfo-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name px_comm-srv:<SetCameraInfo-response> is deprecated: use px_comm-srv:SetCameraInfo-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <SetCameraInfo-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-srv:success-val is deprecated.  Use px_comm-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'status_message-val :lambda-list '(m))
(cl:defmethod status_message-val ((m <SetCameraInfo-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader px_comm-srv:status_message-val is deprecated.  Use px_comm-srv:status_message instead.")
  (status_message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetCameraInfo-response>) ostream)
  "Serializes a message object of type '<SetCameraInfo-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'status_message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'status_message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetCameraInfo-response>) istream)
  "Deserializes a message object of type '<SetCameraInfo-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'status_message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'status_message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetCameraInfo-response>)))
  "Returns string type for a service object of type '<SetCameraInfo-response>"
  "px_comm/SetCameraInfoResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetCameraInfo-response)))
  "Returns string type for a service object of type 'SetCameraInfo-response"
  "px_comm/SetCameraInfoResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetCameraInfo-response>)))
  "Returns md5sum for a message object of type '<SetCameraInfo-response>"
  "b7b33d05bd0d56b83943d2370771da4c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetCameraInfo-response)))
  "Returns md5sum for a message object of type 'SetCameraInfo-response"
  "b7b33d05bd0d56b83943d2370771da4c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetCameraInfo-response>)))
  "Returns full string definition for message of type '<SetCameraInfo-response>"
  (cl:format cl:nil "bool success~%string status_message~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetCameraInfo-response)))
  "Returns full string definition for message of type 'SetCameraInfo-response"
  (cl:format cl:nil "bool success~%string status_message~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetCameraInfo-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'status_message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetCameraInfo-response>))
  "Converts a ROS message object to a list"
  (cl:list 'SetCameraInfo-response
    (cl:cons ':success (success msg))
    (cl:cons ':status_message (status_message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'SetCameraInfo)))
  'SetCameraInfo-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'SetCameraInfo)))
  'SetCameraInfo-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetCameraInfo)))
  "Returns string type for a service object of type '<SetCameraInfo>"
  "px_comm/SetCameraInfo")