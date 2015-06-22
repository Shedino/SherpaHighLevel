# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "casy_rover: 13 messages, 1 services")

set(MSG_I_FLAGS "-Icasy_rover:/home/odroid/catkin_ws/src/casy_rover/msg;-Igeometry_msgs:/opt/ros/indigo/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(casy_rover_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg" "casy_rover/Waypoint"
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg" ""
)

get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg" NAME_WE)
add_custom_target(_casy_rover_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "casy_rover" "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg" "std_msgs/Header"
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg"
  "${MSG_I_FLAGS}"
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)
_generate_msg_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)

### Generating Services
_generate_srv_cpp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
)

### Generating Module File
_generate_module_cpp(casy_rover
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(casy_rover_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(casy_rover_generate_messages casy_rover_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_cpp _casy_rover_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(casy_rover_gencpp)
add_dependencies(casy_rover_gencpp casy_rover_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS casy_rover_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg"
  "${MSG_I_FLAGS}"
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)
_generate_msg_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)

### Generating Services
_generate_srv_lisp(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
)

### Generating Module File
_generate_module_lisp(casy_rover
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(casy_rover_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(casy_rover_generate_messages casy_rover_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_lisp _casy_rover_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(casy_rover_genlisp)
add_dependencies(casy_rover_genlisp casy_rover_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS casy_rover_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg"
  "${MSG_I_FLAGS}"
  "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)
_generate_msg_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/indigo/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)

### Generating Services
_generate_srv_py(casy_rover
  "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
)

### Generating Module File
_generate_module_py(casy_rover
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(casy_rover_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(casy_rover_generate_messages casy_rover_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/ControllerOutput.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/CurrentMission.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/srv/APMCommand.srv" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/MissionItem.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/RC.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Control.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/State.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/FilteredPosition.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Attitude.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/WaypointList.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Waypoint.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Status.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/VFR_HUD.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/odroid/catkin_ws/src/casy_rover/msg/Mavlink_RAW_IMU.msg" NAME_WE)
add_dependencies(casy_rover_generate_messages_py _casy_rover_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(casy_rover_genpy)
add_dependencies(casy_rover_genpy casy_rover_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS casy_rover_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/casy_rover
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(casy_rover_generate_messages_cpp geometry_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/casy_rover
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(casy_rover_generate_messages_lisp geometry_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/casy_rover
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(casy_rover_generate_messages_py geometry_msgs_generate_messages_py)
