FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/roscopter/msg"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "../msg_gen/lisp/VFR_HUD.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_VFR_HUD.lisp"
  "../msg_gen/lisp/Attitude.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Attitude.lisp"
  "../msg_gen/lisp/Control.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Control.lisp"
  "../msg_gen/lisp/Mavlink_RAW_IMU.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_Mavlink_RAW_IMU.lisp"
  "../msg_gen/lisp/State.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_State.lisp"
  "../msg_gen/lisp/RC.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_RC.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
