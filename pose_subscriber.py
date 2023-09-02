#!/usr/bin/env python3
import rospy
from turtlesim.msg import Pose

def pose_callback(msg):
    rospy.loginfo("("+ str(msg.x) +")")

if __name__ == '__main__':
    rospy.init_node("Turtle_pose_subscriber")


    sub = rospy.Subscriber("/turtle1/pose", Pose, callback=pose_callback)

    rospy.loginfo("nODE HAS BEEN STARTED")

    rospy.spin()