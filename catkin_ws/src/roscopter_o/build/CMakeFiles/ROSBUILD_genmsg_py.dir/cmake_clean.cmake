FILE(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/roscopter/msg"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/roscopter/msg/__init__.py"
  "../src/roscopter/msg/_VFR_HUD.py"
  "../src/roscopter/msg/_Attitude.py"
  "../src/roscopter/msg/_Control.py"
  "../src/roscopter/msg/_Mavlink_RAW_IMU.py"
  "../src/roscopter/msg/_State.py"
  "../src/roscopter/msg/_RC.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
