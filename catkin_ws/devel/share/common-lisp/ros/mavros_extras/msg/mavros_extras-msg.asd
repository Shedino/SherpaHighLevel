
(cl:in-package :asdf)

(defsystem "mavros_extras-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "OpticalFlowRad" :depends-on ("_package_OpticalFlowRad"))
    (:file "_package_OpticalFlowRad" :depends-on ("_package"))
  ))