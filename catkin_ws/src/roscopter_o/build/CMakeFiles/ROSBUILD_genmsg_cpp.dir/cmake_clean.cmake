FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/roscopter/msg"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/roscopter/VFR_HUD.h"
  "../msg_gen/cpp/include/roscopter/Attitude.h"
  "../msg_gen/cpp/include/roscopter/Control.h"
  "../msg_gen/cpp/include/roscopter/Mavlink_RAW_IMU.h"
  "../msg_gen/cpp/include/roscopter/State.h"
  "../msg_gen/cpp/include/roscopter/RC.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
