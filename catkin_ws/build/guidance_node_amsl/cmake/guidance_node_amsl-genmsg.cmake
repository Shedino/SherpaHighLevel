# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "guidance_node_amsl: 3 messages, 0 services")

set(MSG_I_FLAGS "-Iguidance_node_amsl:/home/odroid/catkin_ws/src/guidance_node_amsl/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(guidance_node_amsl_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg" NAME_WE)
add_custom_target(_guidance_node_amsl_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "guidance_node_amsl" "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg" NAME_WE)
add_custom_target(_guidance_node_amsl_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "guidance_node_amsl" "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg" NAME_WE)
add_custom_target(_guidance_node_amsl_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "guidance_node_amsl" "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_cpp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_cpp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl
)

### Generating Services

### Generating Module File
_generate_module_cpp(guidance_node_amsl
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(guidance_node_amsl_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(guidance_node_amsl_generate_messages guidance_node_amsl_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_cpp _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_cpp _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_cpp _guidance_node_amsl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(guidance_node_amsl_gencpp)
add_dependencies(guidance_node_amsl_gencpp guidance_node_amsl_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS guidance_node_amsl_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_lisp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_lisp(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl
)

### Generating Services

### Generating Module File
_generate_module_lisp(guidance_node_amsl
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(guidance_node_amsl_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(guidance_node_amsl_generate_messages guidance_node_amsl_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_lisp _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_lisp _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_lisp _guidance_node_amsl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(guidance_node_amsl_genlisp)
add_dependencies(guidance_node_amsl_genlisp guidance_node_amsl_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS guidance_node_amsl_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_py(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl
)
_generate_msg_py(guidance_node_amsl
  "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl
)

### Generating Services

### Generating Module File
_generate_module_py(guidance_node_amsl
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(guidance_node_amsl_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(guidance_node_amsl_generate_messages guidance_node_amsl_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Position.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_py _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Directive.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_py _guidance_node_amsl_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/guidance_node_amsl/msg/Reference.msg" NAME_WE)
add_dependencies(guidance_node_amsl_generate_messages_py _guidance_node_amsl_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(guidance_node_amsl_genpy)
add_dependencies(guidance_node_amsl_genpy guidance_node_amsl_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS guidance_node_amsl_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/guidance_node_amsl
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(guidance_node_amsl_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/guidance_node_amsl
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(guidance_node_amsl_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/guidance_node_amsl
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(guidance_node_amsl_generate_messages_py std_msgs_generate_messages_py)
