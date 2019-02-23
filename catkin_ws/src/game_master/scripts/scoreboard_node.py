#!/usr/bin/env python
from __future__ import print_function

import os
import rospy
import pandas as pd

def main():
    #Init ROS node
    rospy.init_node('scoreboard_node', anonymous=True)

    game_master_folder = os.path.join(os.path.dirname(__file__), '..')

    #Create rate limiter
    rate = rospy.Rate(1)
    while not rospy.is_shutdown():
        rate.sleep()
	print(__file__+"/../scorefile1.txt")
        scores = pd.read_csv(game_master_folder+"/scorefile1.txt", header=None, names=["Timestamp", "Score"])
	print("Best time is", scores['Score'].min())

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
