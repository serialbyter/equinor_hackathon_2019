#!/usr/bin/env python

from __future__ import print_function

import time
import random

import rospy
from dronelib import Drone

rospy.init_node("dronelib_example")

drone = Drone()
drone.activate()

drone.takeoff(height=3.0)

# Monitor takeoff and dont progress until we are high enough
rate = rospy.Rate(1.0)
position = drone.get_pose().position
while position.z < 2.5:
    # Control rate of while loop
    rate.sleep()

    # Update position
    position = drone.get_pose().position 

print("Takeoff complete")

# generate some random point
x = random.randint(-5, 5)
y = random.randint(-5, 5)
yaw = random.randint(0, 7)

# move to random point
drone.set_target(x, y, yaw)


