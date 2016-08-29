Guidance_node_amsl

-DESCRIPTION: guidance node that generates the RC_OVERRIDE message to command the low level autopilot. It reads references and actual position and computes the 4
velocity input which are transformed into RC channels (1,2,3,4) emulating RC to control latitude, longitude, altitude and yaw. The RC channels are built in the mavros plugin. It takes position and reference in WGS84 but internally the control law is in NED.
The core is implemented in Simulink and .cpp code is generated.

-TOPIC SUBSCRIBED: /reference (type: guidance_node_amsl/Reference), /position (type: guidance_node_amsl/Position)

-TOPIC PUBLISHED: /directive (type: guidance_node_amsl/Directive)