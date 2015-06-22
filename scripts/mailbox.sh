#!/bin/bash

#---parameters
local_path=/home/jcacace
server_path=/home/jcacace
user=jcacace
server_ip=143.225.169.69
#---

#check event on the desired directory
inotifywait -m $local_path -e create -e moved_to |

	#----new event 
	while read directory action file; do
		#debug
		echo "The file '$file' appeared in directory '$directory'"
		#send file via sftp 
		scp $local_path/$file $user@$server_ip:$server_path
		#debug
		echo "File sent to: " $server_ip

		rm $local_path/$file
  done
	#---