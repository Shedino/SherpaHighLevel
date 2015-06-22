
(cl:in-package :asdf)

(defsystem "mms-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Sys_status" :depends-on ("_package_Sys_status"))
    (:file "_package_Sys_status" :depends-on ("_package"))
    (:file "Ack_arm" :depends-on ("_package_Ack_arm"))
    (:file "_package_Ack_arm" :depends-on ("_package"))
    (:file "Ack_cmd" :depends-on ("_package_Ack_cmd"))
    (:file "_package_Ack_cmd" :depends-on ("_package"))
    (:file "Cmd" :depends-on ("_package_Cmd"))
    (:file "_package_Cmd" :depends-on ("_package"))
    (:file "Arm" :depends-on ("_package_Arm"))
    (:file "_package_Arm" :depends-on ("_package"))
  ))