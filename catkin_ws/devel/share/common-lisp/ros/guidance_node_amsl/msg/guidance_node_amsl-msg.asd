
(cl:in-package :asdf)

(defsystem "guidance_node_amsl-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Position" :depends-on ("_package_Position"))
    (:file "_package_Position" :depends-on ("_package"))
    (:file "Directive" :depends-on ("_package_Directive"))
    (:file "_package_Directive" :depends-on ("_package"))
    (:file "Reference" :depends-on ("_package_Reference"))
    (:file "_package_Reference" :depends-on ("_package"))
  ))