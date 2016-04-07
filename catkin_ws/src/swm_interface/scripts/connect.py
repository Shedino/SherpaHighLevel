#!/usr/bin/env python
import sys
sys.path.append('/home/odroid/DCM/ubx_robotscenegraph/examples/sherpa/mission_scripts')
import math
import rospy
import tf
import swm
from geographic_msgs.msg import GeoPose
from geographic_msgs.msg import GeoPoint
from geometry_msgs.msg import Quaternion
from camera_handler_sherpa.msg import Camera

agentName = "0"
counter = 0 #runs at 10hz
cameraTopic_ = Camera()
geopoint_ = GeoPoint(0,0,0)
quaternion_ = Quaternion(0,0,0,1)
geopose_ = GeoPose(geopoint_,quaternion_)

def updateGeopose(data):
    global geopoint_
    global quaternion_
    geopoint_ = data.position
    quaternion_ = data.orientation
    #print "[dcm_com:] Geopose %.7f %.7f %.2f ..." % (geopoint_.latitude, geopoint_.longitude, geopoint_.altitude)

def updateCamera(data):
    cameraTopic_ = data
    if data.taken_photo:
        swm.run('add picture wasp_%s %s %.7f %.7f %.2f %.2f %.4f %.4f %.4f' % (agentName, data.path_photo, geopoint_.GeoPoint.latitude, geopoint_.GeoPoint.longitude, geopoint_.GeoPoint.altitude, quaternion_.Quaternion.x, quaternion_.Quaternion.y, quaternion_.Quaternion.z, quaternion_.Quaternion.w))
    if data.taken_video:
        #missing add video on SWM
        swm.run('add picture wasp_%s %s %.7f %.7f %.2f %.2f %.4f %.4f %.4f' % (agentName, data.path_video, geopoint_.GeoPoint.latitude, geopoint_.GeoPoint.longitude, geopoint_.GeoPoint.altitude, quaternion_.Quaternion.x, quaternion_.Quaternion.y, quaternion_.Quaternion.z, quaternion_.Quaternion.w))
    
if __name__ == '__main__':
    try:
        rospy.init_node('dcm_com')
        print "[dcm_com:] initialising %s data on DCM database ..." % (agentName)
        swm.run('set wasp %s geopose 0 0 0 0 0 0 1' % (agentName))
        rospy.Subscriber("geopose",GeoPose,updateGeopose)
        rospy.Subscriber("/camera_published",Camera,updateCamera)
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            counter += 1
            #currentQuaternion = tf.transformations.quaternion_from_euler(currentRoll, currentPitch, currentYaw, "sxyz")
            if counter>20:
            	swm.run('set wasp %s geopose %.7f %.7f %.2f %.2f %.4f %.4f %.4f' % (agentName, geopoint_.latitude, geopoint_.longitude, geopoint_.altitude, quaternion_.x, quaternion_.y, quaternion_.z, quaternion_.w))
            	print "[swm_interface]: the current geopose of the wasp was published %.7f %.7f %.2f" % (geopoint_.latitude, geopoint_.longitude, geopoint_.altitude)
            	counter = 0
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
