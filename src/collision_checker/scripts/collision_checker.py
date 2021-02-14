#!/usr/bin/env python
import rospy
from std_msgs.msg import Bool
from std_msgs.msg import Float64MultiArray
# https://shapely.readthedocs.io/en/stable/manual.html#geometric-objects
import numpy as np
from shapely import affinity
from shapely.geometry import Point
from shapely.geometry import Polygon
from math import pi

#rectangular dimensions (in mm)
rect_length = 81
rect_width = 25
#circle dimensions (in mm)
circle_radius = 8
circle_distance_to_servo = 60
#Servo mounting points
xo1 = -57
xo2 = 57
yo1 = 0
yo2 = 0


def detect_collision(left_angle, right_angle):
    p1 = Polygon([(xo1-rect_width/2 ,yo1), (xo1-rect_width/2, yo1+rect_length), (xo1+rect_width/2, yo1+rect_length), (xo1+rect_width/2, yo1)])
    c1 = Point(xo2, circle_distance_to_servo).buffer(circle_radius)
    p1_rotated = affinity.rotate(p1, left_angle*180/pi, Point(xo1,0))
    c1_rotated = affinity.rotate(c1, -right_angle* 180/pi, Point(xo2,0))
    return not p1_rotated.intersects(c1_rotated) # true if there is no collision


def calc_angles_array_callback(floatArray):
	try:
		rospy.loginfo("inside calc_angles_array_callback: received [" + str(round(floatArray.data[0], 1)) + " , " + str(round(floatArray.data[1],1)) + "]")
		rospy.loginfo("detect_collision(floatArray.data[0], floatArray.data[1]) = " + str(detect_collision(floatArray.data[0], floatArray.data[1])))
		pub_pose_ok.publish(detect_collision(floatArray.data[0], floatArray.data[1]))
		if(detect_collision(floatArray.data[0], floatArray.data[1]) == True):
			pub_checked_angles_array.publish(floatArray)
	except:
		pass


def set_angle_array_callback(floatArray):
	try:
		rospy.loginfo("inside set_angle_array_callback: received [" + str(round(floatArray.data[0], 1)) + " , " + str(round(floatArray.data[1],1)) + "]")
		rospy.loginfo("detect_collision(floatArray.data[0], floatArray.data[1]) = " + str(detect_collision(floatArray.data[0], floatArray.data[1])))
		pub_pose_ok.publish(detect_collision(floatArray.data[0], floatArray.data[1]))
		if(detect_collision(floatArray.data[0], floatArray.data[1]) == True):
			pub_checked_angles_array.publish(floatArray)
	except:
		pass



if __name__ == '__main__':
    try:
	rospy.init_node('collision_checker', anonymous=True)

	pub_pose_ok = rospy.Publisher('pose_ok', Bool, queue_size=10)
	pub_checked_angles_array = rospy.Publisher('checked_angles_array', Float64MultiArray, queue_size=10)
	rospy.Subscriber("calc_angles_array", Float64MultiArray, calc_angles_array_callback)
	rospy.Subscriber("set_angles_array", Float64MultiArray, set_angle_array_callback)
	rospy.spin()

    except rospy.ROSInterruptException:
        pass
