#!/usr/bin/env python
from __future__ import print_function

import rospy
import pandas as pd

def main():
    #Init ROS node
    rospy.init_node('scoreboard_node', anonymous=True)

    #Create rate limiter
    rate = rospy.Rate(0.1)
    while not rospy.is_shutdown():
        rate.sleep()
        scores = pd.read_csv("../scorefile1.txt", header=None, names=["Timestamp", "Score"])
        print("Best time is", scores['Score'].min())

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
