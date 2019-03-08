#!/usr/bin/env python

from __future__ import print_function

import rospy

import random
import math

from dronelib import Drone
from util import parse_map

from ascend_msgs.srv import GlobalMap
from geometry_msgs.msg import Pose


def goal_callback(msg):
    global goal
    goal = msg.position


def main():

    # Init ROS node
    rospy.init_node('task', anonymous=True)

    # Create subscriber for position and goal
    rospy.Subscriber('/goal', Pose, goal_callback)

    # Create map service client
    getMap = rospy.ServiceProxy('/GlobalMap', GlobalMap)
    rospy.wait_for_service('/GlobalMap')

    try:
        raw_map = getMap()
    except rospy.ServiceException as e:
        print("Map service error: " + str(e))
        return

    # Get map as 2D list
    world_map = parse_map(raw_map)

    # Initialize drone
    drone = Drone()
    drone.activate()
    drone.takeoff()

    # --For example code--
    target_x = 0
    target_y = 0
    target_yaw = 0

    rate = rospy.Rate(30)
    while not rospy.is_shutdown():
        rate.sleep()
        # -------------------------------
        # ------Replace this example-----
        # ---------with your code!-------
        # -------------------------------

        # Find how far we are away from target
        distance_to_target = ((target_x - drone.position.x)**2 +
                              (target_y - drone.position.y)**2)**0.5
        angle_to_target = target_yaw - drone.yaw
        angle_to_target = (angle_to_target + math.pi) % (2*math.pi) - math.pi # map angle between -pi and pi

        # Do special action if we are close
        if distance_to_target < 0.5 and angle_to_target < 0.1:
            # Print Current distance to goal
            distance_to_goal = ((drone.position.x - goal.x)**2 +
                                (drone.position.y - goal.y)**2)**0.5

            print("Distance to goal is now", distance_to_goal)

            # Generate some random point and rotation
            target_x = random.randint(-5, 5)
            target_y = random.randint(-5, 5)
            target_yaw = random.randint(0, 7)  # in radians

            # Move to random point
            drone.set_target(target_x, target_y, target_yaw)
        else:
            print("distance to target:", distance_to_target, "angle_to_target", angle_to_target)


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
