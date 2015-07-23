# MISSION_ACK ( #47 )
# Ack message during MISSION handling. The type field states if this message is a positive ack (type=0) or if an error happened (type=non-zero).
# MAV_MISSION_RESULT
# result in a mavlink mission ack
# 0	MAV_MISSION_ACCEPTED	mission accepted OK
# 1	MAV_MISSION_ERROR	generic error / not accepting mission commands at all right now
# 2	MAV_MISSION_UNSUPPORTED_FRAME	coordinate frame is not supported
# 3	MAV_MISSION_UNSUPPORTED	command is not supported
# 4	MAV_MISSION_NO_SPACE	mission item exceeds storage space
# 5	MAV_MISSION_INVALID	one of the parameters has an invalid value
# 6	MAV_MISSION_INVALID_PARAM1	param1 has an invalid value
# 7	MAV_MISSION_INVALID_PARAM2	param2 has an invalid value
# 8	MAV_MISSION_INVALID_PARAM3	param3 has an invalid value
# 9	MAV_MISSION_INVALID_PARAM4	param4 has an invalid value
# 10	MAV_MISSION_INVALID_PARAM5_X	x/param5 has an invalid value
# 11	MAV_MISSION_INVALID_PARAM6_Y	y/param6 has an invalid value
# 12	MAV_MISSION_INVALID_PARAM7	param7 has an invalid value
# 13	MAV_MISSION_INVALID_SEQUENCE	received waypoint out of sequence
# 14	MAV_MISSION_DENIED	not accepting any mission commands from this communication partner
uint8 target_system		
uint8 target_component	
uint8 mav_mission_result
