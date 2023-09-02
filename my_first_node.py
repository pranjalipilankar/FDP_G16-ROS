#!/usr/bin/env python3
import rospy

if __name__ == '__main__':
    rospy.init_node("test_node")

    rospy.loginfo("Hello from test node")
    rate = rospy.Rate(1)

    while not rospy.is_shutdown():
        rospy.loginfo("Hello")
        rospy.logerr("Error")
        rospy.logwarn("Warning")
        rate.sleep()


