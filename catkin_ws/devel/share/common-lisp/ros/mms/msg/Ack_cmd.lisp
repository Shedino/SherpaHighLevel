; Auto-generated. Do not edit!


(cl:in-package mms-msg)


;//! \htmlinclude Ack_cmd.msg.html

(cl:defclass <Ack_cmd> (roslisp-msg-protocol:ros-message)
  ((mission_item_reached
    :reader mission_item_reached
    :initarg :mission_item_reached
    :type cl:boolean
    :initform cl:nil)
   (mav_mission_accepted
    :reader mav_mission_accepted
    :initarg :mav_mission_accepted
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass Ack_cmd (<Ack_cmd>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Ack_cmd>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Ack_cmd)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name mms-msg:<Ack_cmd> is deprecated: use mms-msg:Ack_cmd instead.")))

(cl:ensure-generic-function 'mission_item_reached-val :lambda-list '(m))
(cl:defmethod mission_item_reached-val ((m <Ack_cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:mission_item_reached-val is deprecated.  Use mms-msg:mission_item_reached instead.")
  (mission_item_reached m))

(cl:ensure-generic-function 'mav_mission_accepted-val :lambda-list '(m))
(cl:defmethod mav_mission_accepted-val ((m <Ack_cmd>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader mms-msg:mav_mission_accepted-val is deprecated.  Use mms-msg:mav_mission_accepted instead.")
  (mav_mission_accepted m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Ack_cmd>) ostream)
  "Serializes a message object of type '<Ack_cmd>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'mission_item_reached) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'mav_mission_accepted) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Ack_cmd>) istream)
  "Deserializes a message object of type '<Ack_cmd>"
    (cl:setf (cl:slot-value msg 'mission_item_reached) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'mav_mission_accepted) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Ack_cmd>)))
  "Returns string type for a message object of type '<Ack_cmd>"
  "mms/Ack_cmd")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Ack_cmd)))
  "Returns string type for a message object of type 'Ack_cmd"
  "mms/Ack_cmd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Ack_cmd>)))
  "Returns md5sum for a message object of type '<Ack_cmd>"
  "39d26c985985072c3c449a1c32116e79")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Ack_cmd)))
  "Returns md5sum for a message object of type 'Ack_cmd"
  "39d26c985985072c3c449a1c32116e79")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Ack_cmd>)))
  "Returns full string definition for message of type '<Ack_cmd>"
  (cl:format cl:nil "# false = mission item not reached, true = mission item reached~%bool mission_item_reached~%# false = mission item not accepted, true = mission item accepted~%bool mav_mission_accepted~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Ack_cmd)))
  "Returns full string definition for message of type 'Ack_cmd"
  (cl:format cl:nil "# false = mission item not reached, true = mission item reached~%bool mission_item_reached~%# false = mission item not accepted, true = mission item accepted~%bool mav_mission_accepted~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Ack_cmd>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Ack_cmd>))
  "Converts a ROS message object to a list"
  (cl:list 'Ack_cmd
    (cl:cons ':mission_item_reached (mission_item_reached msg))
    (cl:cons ':mav_mission_accepted (mav_mission_accepted msg))
))
