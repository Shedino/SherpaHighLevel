# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "camera_handler_SHERPA: 1 messages, 0 services")

set(MSG_I_FLAGS "-Icamera_handler_SHERPA:/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg;-Isensor_msgs:/opt/ros/indigo/share/sensor_msgs/cmake/../msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(camera_handler_SHERPA_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg" NAME_WE)
add_custom_target(_camera_handler_SHERPA_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "camera_handler_SHERPA" "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(camera_handler_SHERPA
  "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/camera_handler_SHERPA
)

### Generating Services

### Generating Module File
_generate_module_cpp(camera_handler_SHERPA
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/camera_handler_SHERPA
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(camera_handler_SHERPA_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(camera_handler_SHERPA_generate_messages camera_handler_SHERPA_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg" NAME_WE)
add_dependencies(camera_handler_SHERPA_generate_messages_cpp _camera_handler_SHERPA_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(camera_handler_SHERPA_gencpp)
add_dependencies(camera_handler_SHERPA_gencpp camera_handler_SHERPA_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS camera_handler_SHERPA_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(camera_handler_SHERPA
  "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/camera_handler_SHERPA
)

### Generating Services

### Generating Module File
_generate_module_lisp(camera_handler_SHERPA
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/camera_handler_SHERPA
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(camera_handler_SHERPA_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(camera_handler_SHERPA_generate_messages camera_handler_SHERPA_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg" NAME_WE)
add_dependencies(camera_handler_SHERPA_generate_messages_lisp _camera_handler_SHERPA_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(camera_handler_SHERPA_genlisp)
add_dependencies(camera_handler_SHERPA_genlisp camera_handler_SHERPA_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS camera_handler_SHERPA_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(camera_handler_SHERPA
  "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/camera_handler_SHERPA
)

### Generating Services

### Generating Module File
_generate_module_py(camera_handler_SHERPA
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/camera_handler_SHERPA
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(camera_handler_SHERPA_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(camera_handler_SHERPA_generate_messages camera_handler_SHERPA_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/camera_handler_SHERPA/msg/Camera.msg" NAME_WE)
add_dependencies(camera_handler_SHERPA_generate_messages_py _camera_handler_SHERPA_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(camera_handler_SHERPA_genpy)
add_dependencies(camera_handler_SHERPA_genpy camera_handler_SHERPA_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS camera_handler_SHERPA_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/camera_handler_SHERPA)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/camera_handler_SHERPA
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(camera_handler_SHERPA_generate_messages_cpp sensor_msgs_generate_messages_cpp)
add_dependencies(camera_handler_SHERPA_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/camera_handler_SHERPA)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/camera_handler_SHERPA
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(camera_handler_SHERPA_generate_messages_lisp sensor_msgs_generate_messages_lisp)
add_dependencies(camera_handler_SHERPA_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/camera_handler_SHERPA)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/camera_handler_SHERPA\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/camera_handler_SHERPA
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(camera_handler_SHERPA_generate_messages_py sensor_msgs_generate_messages_py)
add_dependencies(camera_handler_SHERPA_generate_messages_py std_msgs_generate_messages_py)
