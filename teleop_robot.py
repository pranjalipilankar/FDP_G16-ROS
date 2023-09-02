#!/usr/bin/env python3

import rospy
from std_msgs.msg import String

if __name__=="__main__":
    rospy.init_node('keyboard_input_publisher')
    pub = rospy.Publisher('/keyboard_input', String, queue_size=10)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        rospy.loginfo("Waiting for keyboard input...")
        data = input()
        rospy.loginfo("Received keyboard input: %s" % data)
        pub.publish(data)
        rate.sleep()
