#!/usr/bin/env python

from __future__ import print_function

import time
import random

import rospy
from dronelib import Drone

rospy.init_node("dronelib_example")

drone = Drone()
drone.activate()

drone.takeoff()

# Assume takeoff takes less than 5 seconds
time.sleep(5)

while not rospy.is_shutdown():
    # generate some random point
    x = random.randint(-5, 5)
    y = random.randint(-5, 5)
    yaw = random.randint(0, 7)

    # move to random point
    drone.set_target(x, y, yaw)

    # sleep
    time.sleep(10)


