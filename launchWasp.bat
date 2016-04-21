@echo off
echo.
set OdroidIP=192.168.0.106
set OdroidUser=odroid
set OdroidPassWord=odroid
echo [SHERPA WASP LAUNCH:] ROS environment initialisation started

start "uniboAMSLUSB" cmd /c "plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_uniboAMSLUSB"
echo [SHERPA WASP LAUNCH:] "unibo_AMSLUSB" launched in "uniboAMSLUSB" command window 
start "usb_cam_node" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_usb_cam_node
echo [SHERPA WASP LAUNCH:] "usb_cam_node" launched in "usb_cam_node" command window
start "camera_handler_SHERPA" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_camera_handler_SHERPA
echo [SHERPA WASP LAUNCH:] "camera_handler_SHERPA" launched in "camera_handler_SHERPA" command window
REM start "mms" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_mms
REM echo [SHERPA WASP LAUNCH:] "mms" launched in "mms" command window
start "mms_group" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_mms_group
echo [SHERPA WASP LAUNCH:] "mms_group" launched in "mms_group" command window
start "mavsys" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_mavsys
echo [SHERPA WASP LAUNCH:] "mavsys" launched in "mavsys" command window
start "web_video_server" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_web_video_server
echo [SHERPA WASP LAUNCH:] "web_video_server" launched in "web_video_server" command window
start "transfer_file" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/transfer_file
echo [SHERPA WASP LAUNCH:] "transfer_file" launched in "transfer_file" command window 
putty -ssh %OdroidUser%@%OdroidIP% -pw %OdroidPassWord%
echo [SHERPA WASP LAUNCH:] "unibo_GCS" launched in "unibo_GCS" command window 

:escInput
set /p "escCmd=[SHERPA WASP LAUNCH:] all ROS processes started; type 'kill' when you want to stop their execution and quit this script: "
if "%escCmd%"=="kill" (
	plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/killAll
	echo [SHERPA WASP LAUNCH:] all ROS processes were killed.
) else (
	goto :escInput
)
