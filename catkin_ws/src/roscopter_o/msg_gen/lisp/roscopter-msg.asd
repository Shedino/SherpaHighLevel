
(cl:in-package :asdf)

(defsystem "roscopter-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "VFR_HUD" :depends-on ("_package_VFR_HUD"))
    (:file "_package_VFR_HUD" :depends-on ("_package"))
    (:file "Attitude" :depends-on ("_package_Attitude"))
    (:file "_package_Attitude" :depends-on ("_package"))
    (:file "Control" :depends-on ("_package_Control"))
    (:file "_package_Control" :depends-on ("_package"))
    (:file "Mavlink_RAW_IMU" :depends-on ("_package_Mavlink_RAW_IMU"))
    (:file "_package_Mavlink_RAW_IMU" :depends-on ("_package"))
    (:file "State" :depends-on ("_package_State"))
    (:file "_package_State" :depends-on ("_package"))
    (:file "RC" :depends-on ("_package_RC"))
    (:file "_package_RC" :depends-on ("_package"))
  ))