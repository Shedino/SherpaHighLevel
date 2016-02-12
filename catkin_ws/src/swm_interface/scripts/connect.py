#!/usr/bin/env python
import sys
sys.path.append('/home/odroid/pySWM')
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
geopose_ = GeoPose()
geopoint_ = GeoPoint()
quaternion_ = Quaternion()

def updateGeopose(data):
    geopoint_ = data.GeoPoint
    quaternion_ = data.Quaternion

if __name__ == '__main__':
    try:
        rospy.init_node('dcm_com')
        print "[dcm_com:] initialising %s data on DCM database ..." % (agentName)
        swm.run('set wasp %s geopose 0 0 0 0 0 0 1' % (agentName))
        rospy.Subscriber("geopose",GeoPose,updateGeopose)
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            counter += 1
            #currentQuaternion = tf.transformations.quaternion_from_euler(currentRoll, currentPitch, currentYaw, "sxyz")
            if counter>30:
            	swm.run('set wasp %s geopose %.7f %.7f %.2f %.2f %.4f %.4f %.4f' % (agentName, geopoint_.GeoPoint.latitude, geopoint_.GeoPoint.longitude, geopoint_.GeoPoint.altitude, quaternion_.Quaternion.x, quaternion_.Quaternion.y, quaternion_.Quaternion.z, quaternion_.Quaternion.w))
            	counter = 0
            	print "[swm_interface]: the current geopose of the wasp was published"
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
