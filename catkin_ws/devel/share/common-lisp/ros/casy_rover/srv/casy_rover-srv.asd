
(cl:in-package :asdf)

(defsystem "casy_rover-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "APMCommand" :depends-on ("_package_APMCommand"))
    (:file "_package_APMCommand" :depends-on ("_package"))
  ))