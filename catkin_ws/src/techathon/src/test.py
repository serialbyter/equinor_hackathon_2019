#!/usr/bin/env python

from __future__ import print_function

import time
import random

import rospy
from dronelib import Drone

rospy.init_node("drone_test")
drone = Drone()

drone.activate()

drone.takeoff()
time.sleep(5)

drone.set_target(5,0,3.14)
time.sleep(5)

drone.set_target(5,0,0)
time.sleep(5)

drone.set_target(3,0,0)
time.sleep(5)

drone.set_target(1,0,0)
time.sleep(5)

while not rospy.is_shutdown():
    x = random.randint(-5, 5)
    y = random.randint(-5, 5)
    yaw = random.randint(0, 7)
    drone.set_target(x, y, yaw)
    time.sleep(10)


