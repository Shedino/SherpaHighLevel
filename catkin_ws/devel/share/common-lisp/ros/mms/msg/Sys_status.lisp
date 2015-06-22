; Auto-generated. Do not edit!


(cl:in-package mms-msg)


;//! \htmlinclude Sys_status.msg.html

(cl:defclass <Sys_status> (roslisp-msg-protocol:ros-message)
  ((armed
    :reader armed
    :initarg :armed
    :type cl:boolean
    :initform cl:nil)
   (voltage_battery
    :reader voltage_battery
    :initarg :voltage_battery
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Sys_status (<Sys_status>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Sys_status>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Sys_status)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mms-msg:<Sys_status> is deprecated: use mms-msg:Sys_status instead.")))

(cl:ensure-generic-function 'armed-val :lambda-list '(m))
(cl:defmethod armed-val ((m <Sys_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:armed-val is deprecated.  Use mms-msg:armed instead.")
  (armed m))

(cl:ensure-generic-function 'voltage_battery-val :lambda-list '(m))
(cl:defmethod voltage_battery-val ((m <Sys_status>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:voltage_battery-val is deprecated.  Use mms-msg:voltage_battery instead.")
  (voltage_battery m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Sys_status>) ostream)
  "Serializes a message object of type '<Sys_status>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'armed) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'voltage_battery)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'voltage_battery)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Sys_status>) istream)
  "Deserializes a message object of type '<Sys_status>"
    (cl:setf (cl:slot-value msg 'armed) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'voltage_battery)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'voltage_battery)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Sys_status>)))
  "Returns string type for a message object of type '<Sys_status>"
  "mms/Sys_status")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Sys_status)))
  "Returns string type for a message object of type 'Sys_status"
  "mms/Sys_status")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Sys_status>)))
  "Returns md5sum for a message object of type '<Sys_status>"
  "ca0fa588e35b052857fc871230a841ce")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Sys_status)))
  "Returns md5sum for a message object of type 'Sys_status"
  "ca0fa588e35b052857fc871230a841ce")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Sys_status>)))
  "Returns full string definition for message of type '<Sys_status>"
  (cl:format cl:nil "# true = armed with propellers spinning~%bool armed~%# voltage battery in millivolts~%uint16 voltage_battery~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Sys_status)))
  "Returns full string definition for message of type 'Sys_status"
  (cl:format cl:nil "# true = armed with propellers spinning~%bool armed~%# voltage battery in millivolts~%uint16 voltage_battery~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Sys_status>))
  (cl:+ 0
     1
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Sys_status>))
  "Converts a ROS message object to a list"
  (cl:list 'Sys_status
    (cl:cons ':armed (armed msg))
    (cl:cons ':voltage_battery (voltage_battery msg))
))
