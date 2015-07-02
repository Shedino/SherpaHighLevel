@echo off
echo.
set OdroidIP=192.168.0.108
set OdroidUser=odroid
set OdroidPassWord=odroid
echo [SHERPA WASP LAUNCH:] ROS environment initialisation started

start "uniboAMSLUSB" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/launch_uniboAMSLUSB
echo [SHERPA WASP LAUNCH:] "unibo_AMSLUSB" launched in "uniboAMSLUSB" command window 
start "usb_cam_node" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/launch_usb_cam_node
echo [SHERPA WASP LAUNCH:] "usb_cam_node" launched in "usb_cam_node" command window
start "camera_handler_SHERPA" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/launch_camera_handler_SHERPA
echo [SHERPA WASP LAUNCH:] "camera_handler_SHERPA" launched in "camera_handler_SHERPA" command window
start "mms" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/launch_mms
echo [SHERPA WASP LAUNCH:] "mms" launched in "mms" command window
start "mavsys" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/launch_mavsys
echo [SHERPA WASP LAUNCH:] "mavsys" launched in "mavsys" command window
start "transfer_file" cmd /k plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/transfer_file
echo [SHERPA WASP LAUNCH:] "transfer_file" launched in "transfer_file" command window 
putty -ssh %OdroidUser%@%OdroidIP% -pw %OdroidPassWord%
echo [SHERPA WASP LAUNCH:] "unibo_GCS" launched in "unibo_GCS" command window 

set /p "escCmd=[SHERPA WASP LAUNCH:] All ROS processes started. Type 'kill' to stop their execution or 'continue' to quit this initialisation routine: "
if "%escCmd%"=="kill" (
	plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/launch_scripts/killAll
	echo [SHERPA WASP LAUNCH:] all ROS processes were killed.
) else (
	echo [SHERPA WASP LAUNCH:] initialisation routine quitting
)