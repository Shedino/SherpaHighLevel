
(cl:in-package :asdf)

(defsystem "px_comm-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "CameraInfo" :depends-on ("_package_CameraInfo"))
    (:file "_package_CameraInfo" :depends-on ("_package"))
    (:file "Mavlink" :depends-on ("_package_Mavlink"))
    (:file "_package_Mavlink" :depends-on ("_package"))
    (:file "OpticalFlow" :depends-on ("_package_OpticalFlow"))
    (:file "_package_OpticalFlow" :depends-on ("_package"))
  ))