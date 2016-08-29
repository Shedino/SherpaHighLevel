# The RAW IMU readings for the usual 9DOF sensor setup
#
# RAW_IMU message

uint64 time_usec	#Timestamp (microseconds since UNIX epoch or microseconds since system boot)
int16 xacc 			#X acceleration (raw)
int16 yacc			#Y acceleration (raw)
int16 zacc			#Z acceleration (raw)
int16 xgyro			#Angular speed around X axis (raw)
int16 ygyro			#Angular speed around Y axis (raw)
int16 zgyro			#Angular speed around Z axis (raw)
int16 xmag			#X Magnetic field (raw)
int16 ymag			#Y Magnetic field (raw)
int16 zmag			#Z Magnetic field (raw)
