# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "mms: 5 messages, 0 services")

set(MSG_I_FLAGS "-Imms:/home/odroid/catkin_ws/src/mms/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(mms_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg" NAME_WE)
add_custom_target(_mms_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mms" "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg" NAME_WE)
add_custom_target(_mms_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mms" "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg" NAME_WE)
add_custom_target(_mms_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mms" "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Arm.msg" NAME_WE)
add_custom_target(_mms_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mms" "/home/odroid/catkin_ws/src/mms/msg/Arm.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg" NAME_WE)
add_custom_target(_mms_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mms" "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
)
_generate_msg_cpp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
)
_generate_msg_cpp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
)
_generate_msg_cpp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
)
_generate_msg_cpp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
)

### Generating Services

### Generating Module File
_generate_module_cpp(mms
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(mms_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(mms_generate_messages mms_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg" NAME_WE)
add_dependencies(mms_generate_messages_cpp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_cpp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_cpp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_cpp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_cpp _mms_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mms_gencpp)
add_dependencies(mms_gencpp mms_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mms_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
)
_generate_msg_lisp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
)
_generate_msg_lisp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
)
_generate_msg_lisp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
)
_generate_msg_lisp(mms
  "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
)

### Generating Services

### Generating Module File
_generate_module_lisp(mms
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(mms_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(mms_generate_messages mms_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg" NAME_WE)
add_dependencies(mms_generate_messages_lisp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_lisp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_lisp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_lisp _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_lisp _mms_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mms_genlisp)
add_dependencies(mms_genlisp mms_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mms_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(mms
  "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
)
_generate_msg_py(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
)
_generate_msg_py(mms
  "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
)
_generate_msg_py(mms
  "/home/odroid/catkin_ws/src/mms/msg/Arm.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
)
_generate_msg_py(mms
  "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
)

### Generating Services

### Generating Module File
_generate_module_py(mms
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(mms_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(mms_generate_messages mms_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Sys_status.msg" NAME_WE)
add_dependencies(mms_generate_messages_py _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_py _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Cmd.msg" NAME_WE)
add_dependencies(mms_generate_messages_py _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_py _mms_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/mms/msg/Ack_arm.msg" NAME_WE)
add_dependencies(mms_generate_messages_py _mms_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mms_genpy)
add_dependencies(mms_genpy mms_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mms_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mms
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(mms_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mms
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(mms_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mms
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(mms_generate_messages_py std_msgs_generate_messages_py)
