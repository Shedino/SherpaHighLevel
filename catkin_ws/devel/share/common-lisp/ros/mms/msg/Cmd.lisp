; Auto-generated. Do not edit!


(cl:in-package mms-msg)


;//! \htmlinclude Cmd.msg.html

(cl:defclass <Cmd> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:fixnum
    :initform 0)
   (param1
    :reader param1
    :initarg :param1
    :type cl:float
    :initform 0.0)
   (param2
    :reader param2
    :initarg :param2
    :type cl:float
    :initform 0.0)
   (param3
    :reader param3
    :initarg :param3
    :type cl:float
    :initform 0.0)
   (param4
    :reader param4
    :initarg :param4
    :type cl:float
    :initform 0.0)
   (param5
    :reader param5
    :initarg :param5
    :type cl:float
    :initform 0.0)
   (param6
    :reader param6
    :initarg :param6
    :type cl:float
    :initform 0.0)
   (param7
    :reader param7
    :initarg :param7
    :type cl:float
    :initform 0.0))
)

(cl:defclass Cmd (<Cmd>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Cmd>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Cmd)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mms-msg:<Cmd> is deprecated: use mms-msg:Cmd instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:command-val is deprecated.  Use mms-msg:command instead.")
  (command m))

(cl:ensure-generic-function 'param1-val :lambda-list '(m))
(cl:defmethod param1-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param1-val is deprecated.  Use mms-msg:param1 instead.")
  (param1 m))

(cl:ensure-generic-function 'param2-val :lambda-list '(m))
(cl:defmethod param2-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param2-val is deprecated.  Use mms-msg:param2 instead.")
  (param2 m))

(cl:ensure-generic-function 'param3-val :lambda-list '(m))
(cl:defmethod param3-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param3-val is deprecated.  Use mms-msg:param3 instead.")
  (param3 m))

(cl:ensure-generic-function 'param4-val :lambda-list '(m))
(cl:defmethod param4-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param4-val is deprecated.  Use mms-msg:param4 instead.")
  (param4 m))

(cl:ensure-generic-function 'param5-val :lambda-list '(m))
(cl:defmethod param5-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param5-val is deprecated.  Use mms-msg:param5 instead.")
  (param5 m))

(cl:ensure-generic-function 'param6-val :lambda-list '(m))
(cl:defmethod param6-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param6-val is deprecated.  Use mms-msg:param6 instead.")
  (param6 m))

(cl:ensure-generic-function 'param7-val :lambda-list '(m))
(cl:defmethod param7-val ((m <Cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:param7-val is deprecated.  Use mms-msg:param7 instead.")
  (param7 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Cmd>) ostream)
  "Serializes a message object of type '<Cmd>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'command)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'command)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param1))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param2))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param3))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param4))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param5))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param6))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'param7))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Cmd>) istream)
  "Deserializes a message object of type '<Cmd>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'command)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'command)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param1) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param2) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param3) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param4) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param5) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param6) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'param7) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Cmd>)))
  "Returns string type for a message object of type '<Cmd>"
  "mms/Cmd")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Cmd)))
  "Returns string type for a message object of type 'Cmd"
  "mms/Cmd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Cmd>)))
  "Returns md5sum for a message object of type '<Cmd>"
  "5b4f669afd135795e40aa7e9dcc1c4f4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Cmd)))
  "Returns md5sum for a message object of type 'Cmd"
  "5b4f669afd135795e40aa7e9dcc1c4f4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Cmd>)))
  "Returns full string definition for message of type '<Cmd>"
  (cl:format cl:nil "# see MISSION_ITEM in MAVlink~%uint16 command~%float32 param1~%float32 param2~%float32 param3~%float32 param4~%float32 param5~%float32 param6~%float32 param7~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Cmd)))
  "Returns full string definition for message of type 'Cmd"
  (cl:format cl:nil "# see MISSION_ITEM in MAVlink~%uint16 command~%float32 param1~%float32 param2~%float32 param3~%float32 param4~%float32 param5~%float32 param6~%float32 param7~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Cmd>))
  (cl:+ 0
     2
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Cmd>))
  "Converts a ROS message object to a list"
  (cl:list 'Cmd
    (cl:cons ':command (command msg))
    (cl:cons ':param1 (param1 msg))
    (cl:cons ':param2 (param2 msg))
    (cl:cons ':param3 (param3 msg))
    (cl:cons ':param4 (param4 msg))
    (cl:cons ':param5 (param5 msg))
    (cl:cons ':param6 (param6 msg))
    (cl:cons ':param7 (param7 msg))
))
