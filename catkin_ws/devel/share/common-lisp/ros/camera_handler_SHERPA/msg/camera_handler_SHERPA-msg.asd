
(cl:in-package :asdf)

(defsystem "camera_handler_SHERPA-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Camera" :depends-on ("_package_Camera"))
    (:file "_package_Camera" :depends-on ("_package"))
  ))