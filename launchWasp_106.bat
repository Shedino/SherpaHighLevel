@echo off
echo.
set OdroidIP=192.168.0.106
set OdroidUser=odroid
set OdroidPassWord=odroid
echo [SHERPA WASP LAUNCH:] ROS environment initialisation started

start "uniboAMSLUSB" cmd /c "plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_uniboAMSLUSB"
echo [SHERPA WASP LAUNCH:] "unibo_AMSLUSB" launched in "uniboAMSLUSB" command window
timeout /t 2
start "usb_cam_node" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_usb_cam_node
echo [SHERPA WASP LAUNCH:] "usb_cam_node" launched in "usb_cam_node" command window
timeout /t 2
start "camera_handler_SHERPA" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_camera_handler_SHERPA
echo [SHERPA WASP LAUNCH:] "camera_handler_SHERPA" launched in "camera_handler_SHERPA" command window
timeout /t 2
REM start "mms" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_mms
REM echo [SHERPA WASP LAUNCH:] "mms" launched in "mms" command window
REM start "unibo_GCS" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_unibo_GCS
REM echo [SHERPA WASP LAUNCH:] "unibo_GCS" launched in "unibo_GCS" command window
REM timeout /t 2
start "web_video_server" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_web_video_server
echo [SHERPA WASP LAUNCH:] "web_video_server" launched in "web_video_server" command window
timeout /t 2
start "transfer_file" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/transfer_file
echo [SHERPA WASP LAUNCH:] "transfer_file" launched in "transfer_file" command window 
timeout /t 2
start "mavsys" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/launch_mavsys
echo [SHERPA WASP LAUNCH:] "mavsys" launched in "mavsys" command window
timeout /t 2
start "bag" cmd /c plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/bag_script bag_artva
echo [SHERPA WASP LAUNCH:] "bag" launched in "bag" command window
timeout /t 1
putty -ssh %OdroidUser%@%OdroidIP% -pw %OdroidPassWord%
echo [SHERPA WASP LAUNCH:] launch unibo_GCS unibo_GCS.launch in putty please!

:escInput
set /p "escCmd=[SHERPA WASP LAUNCH:] all ROS processes started; type 'kill' when you want to stop their execution and quit this script: "
if "%escCmd%"=="kill" (
	plink -v %OdroidIP% -l %OdroidUser% -pw %OdroidPassWord% bash /home/odroid/SherpaHighLevel/scripts/killAll
	echo [SHERPA WASP LAUNCH:] all ROS processes were killed.
) else (
	goto :escInput
)
