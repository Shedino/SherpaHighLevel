; Auto-generated. Do not edit!


(cl:in-package mavros-srv)


;//! \htmlinclude SetMode-request.msg.html

(cl:defclass <SetMode-request> (roslisp-msg-protocol:ros-message)
  ((base_mode
    :reader base_mode
    :initarg :base_mode
    :type cl:fixnum
    :initform 0)
   (custom_mode
    :reader custom_mode
    :initarg :custom_mode
    :type cl:string
    :initform ""))
)

(cl:defclass SetMode-request (<SetMode-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetMode-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetMode-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mavros-srv:<SetMode-request> is deprecated: use mavros-srv:SetMode-request instead.")))

(cl:ensure-generic-function 'base_mode-val :lambda-list '(m))
(cl:defmethod base_mode-val ((m <SetMode-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:base_mode-val is deprecated.  Use mavros-srv:base_mode instead.")
  (base_mode m))

(cl:ensure-generic-function 'custom_mode-val :lambda-list '(m))
(cl:defmethod custom_mode-val ((m <SetMode-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:custom_mode-val is deprecated.  Use mavros-srv:custom_mode instead.")
  (custom_mode m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<SetMode-request>)))
    "Constants for message type '<SetMode-request>"
  '((:MAV_MODE_PREFLIGHT . 0)
    (:MAV_MODE_STABILIZE_DISARMED . 80)
    (:MAV_MODE_STABILIZE_ARMED . 208)
    (:MAV_MODE_MANUAL_DISARMED . 64)
    (:MAV_MODE_MANUAL_ARMED . 192)
    (:MAV_MODE_GUIDED_DISARMED . 88)
    (:MAV_MODE_GUIDED_ARMED . 216)
    (:MAV_MODE_AUTO_DISARMED . 92)
    (:MAV_MODE_AUTO_ARMED . 220)
    (:MAV_MODE_TEST_DISARMED . 66)
    (:MAV_MODE_TEST_ARMED . 194))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'SetMode-request)))
    "Constants for message type 'SetMode-request"
  '((:MAV_MODE_PREFLIGHT . 0)
    (:MAV_MODE_STABILIZE_DISARMED . 80)
    (:MAV_MODE_STABILIZE_ARMED . 208)
    (:MAV_MODE_MANUAL_DISARMED . 64)
    (:MAV_MODE_MANUAL_ARMED . 192)
    (:MAV_MODE_GUIDED_DISARMED . 88)
    (:MAV_MODE_GUIDED_ARMED . 216)
    (:MAV_MODE_AUTO_DISARMED . 92)
    (:MAV_MODE_AUTO_ARMED . 220)
    (:MAV_MODE_TEST_DISARMED . 66)
    (:MAV_MODE_TEST_ARMED . 194))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetMode-request>) ostream)
  "Serializes a message object of type '<SetMode-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'base_mode)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'custom_mode))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'custom_mode))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetMode-request>) istream)
  "Deserializes a message object of type '<SetMode-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'base_mode)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'custom_mode) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'custom_mode) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetMode-request>)))
  "Returns string type for a service object of type '<SetMode-request>"
  "mavros/SetModeRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetMode-request)))
  "Returns string type for a service object of type 'SetMode-request"
  "mavros/SetModeRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetMode-request>)))
  "Returns md5sum for a message object of type '<SetMode-request>"
  "48ecf639e39052f8856a839dcddf826a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetMode-request)))
  "Returns md5sum for a message object of type 'SetMode-request"
  "48ecf639e39052f8856a839dcddf826a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetMode-request>)))
  "Returns full string definition for message of type '<SetMode-request>"
  (cl:format cl:nil "~%~%~%uint8 MAV_MODE_PREFLIGHT		= 0~%uint8 MAV_MODE_STABILIZE_DISARMED	= 80~%uint8 MAV_MODE_STABILIZE_ARMED		= 208~%uint8 MAV_MODE_MANUAL_DISARMED		= 64~%uint8 MAV_MODE_MANUAL_ARMED		= 192~%uint8 MAV_MODE_GUIDED_DISARMED		= 88~%uint8 MAV_MODE_GUIDED_ARMED		= 216~%uint8 MAV_MODE_AUTO_DISARMED		= 92~%uint8 MAV_MODE_AUTO_ARMED		= 220~%uint8 MAV_MODE_TEST_DISARMED		= 66~%uint8 MAV_MODE_TEST_ARMED		= 194~%~%uint8 base_mode~%string custom_mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetMode-request)))
  "Returns full string definition for message of type 'SetMode-request"
  (cl:format cl:nil "~%~%~%uint8 MAV_MODE_PREFLIGHT		= 0~%uint8 MAV_MODE_STABILIZE_DISARMED	= 80~%uint8 MAV_MODE_STABILIZE_ARMED		= 208~%uint8 MAV_MODE_MANUAL_DISARMED		= 64~%uint8 MAV_MODE_MANUAL_ARMED		= 192~%uint8 MAV_MODE_GUIDED_DISARMED		= 88~%uint8 MAV_MODE_GUIDED_ARMED		= 216~%uint8 MAV_MODE_AUTO_DISARMED		= 92~%uint8 MAV_MODE_AUTO_ARMED		= 220~%uint8 MAV_MODE_TEST_DISARMED		= 66~%uint8 MAV_MODE_TEST_ARMED		= 194~%~%uint8 base_mode~%string custom_mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetMode-request>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'custom_mode))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetMode-request>))
  "Converts a ROS message object to a list"
  (cl:list 'SetMode-request
    (cl:cons ':base_mode (base_mode msg))
    (cl:cons ':custom_mode (custom_mode msg))
))
;//! \htmlinclude SetMode-response.msg.html

(cl:defclass <SetMode-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass SetMode-response (<SetMode-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetMode-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetMode-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mavros-srv:<SetMode-response> is deprecated: use mavros-srv:SetMode-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <SetMode-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mavros-srv:success-val is deprecated.  Use mavros-srv:success instead.")
  (success m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetMode-response>) ostream)
  "Serializes a message object of type '<SetMode-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetMode-response>) istream)
  "Deserializes a message object of type '<SetMode-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetMode-response>)))
  "Returns string type for a service object of type '<SetMode-response>"
  "mavros/SetModeResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetMode-response)))
  "Returns string type for a service object of type 'SetMode-response"
  "mavros/SetModeResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetMode-response>)))
  "Returns md5sum for a message object of type '<SetMode-response>"
  "48ecf639e39052f8856a839dcddf826a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetMode-response)))
  "Returns md5sum for a message object of type 'SetMode-response"
  "48ecf639e39052f8856a839dcddf826a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetMode-response>)))
  "Returns full string definition for message of type '<SetMode-response>"
  (cl:format cl:nil "bool success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetMode-response)))
  "Returns full string definition for message of type 'SetMode-response"
  (cl:format cl:nil "bool success~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetMode-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetMode-response>))
  "Converts a ROS message object to a list"
  (cl:list 'SetMode-response
    (cl:cons ':success (success msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'SetMode)))
  'SetMode-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'SetMode)))
  'SetMode-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetMode)))
  "Returns string type for a service object of type '<SetMode>"
  "mavros/SetMode")