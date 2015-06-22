
(cl:in-package :asdf)

(defsystem "px_comm-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :px_comm-msg
)
  :components ((:file "_package")
    (:file "SetCameraInfo" :depends-on ("_package_SetCameraInfo"))
    (:file "_package_SetCameraInfo" :depends-on ("_package"))
  ))