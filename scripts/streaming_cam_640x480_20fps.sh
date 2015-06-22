cd /home/odroid/mjpg-streamer/mjpg-streamer
mjpg_streamer -i "./input_uvc.so -r 640x480 -f 20" -o "./output_http.so -p 8085 -w ./www"
