#!/usr/bin/env python

from __future__ import print_function

import rospy

from dronelib import Drone
from ascend_msgs.srv import GlobalMap

def main():
    #Init ROS node
    rospy.init_node('task', anonymous=True)

    #Create subscriber for position and goal
    #rospy.Subscriber('/goal', Pose, goalCallback)

    #Create map service client
    #getMap = rospy.ServiceProxy('/GlobalMap', GlobalMap)
    #rospy.wait_for_service('/GlobalMap')

    #try:
    #    raw_map = getMap()
    #except rospy.ServiceException as e:
    #    print("Map service error: " + str(e))
    #    return

    # Get map as 2D list
    #world_map = parseMap(raw_map)

    # Initialize drone
    drone = Drone()

    drone.activate()

    # Rate limited while loop
    rate = rospy.Rate(30)
    while not rospy.is_shutdown():
        rate.sleep()
        # --- YOUR COOL STUFF GOES HERE ---


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
