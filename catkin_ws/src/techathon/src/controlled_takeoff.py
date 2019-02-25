#!/usr/bin/env python

from __future__ import print_function

import time
import random

import rospy
from dronelib import Drone

rospy.init_node("dronelib_example")

# Create drone, activate it and send takeoff command
drone = Drone()
drone.activate()
drone.takeoff(height=3.0)

# Monitor takeoff and dont progress until we are high enough
position = drone.get_pose().position
rate = rospy.Rate(1.0)
while position.z < 2.5 and not rospy.is_shutdown():
    # Control rate of while loop
    rate.sleep()

    # Update position
    position = drone.get_pose().position

print("Takeoff complete")

## Takeoff completed, do something useful
# Generate some random point
x = random.randint(-5, 5)
y = random.randint(-5, 5)
yaw = random.randint(0, 7)

# Move to random point
drone.set_target(x, y, yaw)


