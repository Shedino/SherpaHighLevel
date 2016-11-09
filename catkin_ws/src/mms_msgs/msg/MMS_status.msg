# MMS state
uint16 mms_state
uint16 ON_GROUND_NO_HOME = 10
uint16 SETTING_HOME = 20
uint16 ON_GROUND_DISARMED = 30
uint16 ARMING = 40
uint16 DISARMING = 45
uint16 ON_GROUND_ARMED = 50
uint16 PERFORMING_TAKEOFF = 70
uint16 HAND_TAKEOFF_WAITING = 71
uint16 HAND_TAKEOFF_WAITING_FIRST = 711  #FirstsSub state machine state for hand deployment
uint16 HAND_TAKEOFF_WAITING_SECOND = 712 #FirstsSub state machine state for hand deployment
uint16 IN_FLIGHT = 80
uint16 GRID = 90
uint16 PERFORMING_GO_TO = 100
uint16 PERFORMING_LANDING = 120
uint16 LEASHING = 140
uint16 PAUSED = 150
uint16 MANUAL_FLIGHT = 1000
# Target Reference frame
uint8 target_ref_frame