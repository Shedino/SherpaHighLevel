# The IMU readings in SI units in NED body frame
#
# HIGHRES_IMU message

uint64 time_usec	#Timestamp (microseconds, synced to UNIX time or since system boot)
float32 xacc		#X acceleration (m/s^2)
float32 yacc		#Y acceleration (m/s^2)
float32 zacc		#Z acceleration (m/s^2)
float32 xgyro		#Angular speed around X axis (rad / sec)
float32 ygyro		#Angular speed around Y axis (rad / sec)
float32 zgyro		#Angular speed around Z axis (rad / sec)
float32 xmag		#X Magnetic field (Gauss)
float32 ymag		#Y Magnetic field (Gauss)
float32 zmag		#Z Magnetic field (Gauss)
float32 abs_pressure	#Absolute pressure in millibar
float32 diff_pressure	#Differential pressure in millibar
float32 pressure_alt	#Altitude calculated from pressure
float32 temperature	#Temperature in degrees celsius
uint16 fields_updated	#Bitmask for fields that have updated since last message, bit 0 = xacc, bit 12: temperature
