#!/usr/bin/env python

import time
import random

import rospy

from dronelib import Drone

def main():
    rospy.init_node("dronelib_example")

    # Create a drone object, sets up necessary connections to simulator
    drone = Drone()

    # Arm the drone, which allows it to receive commands
    drone.activate()

    # Takeoff to default height setpoint
    drone.takeoff()

    # Assume takeoff takes less than 5 seconds
    time.sleep(5)

    rate = rospy.Rate(0.1)
    while not rospy.is_shutdown():
        # generate some random point and rotation
        x = random.randint(-5, 5)
        y = random.randint(-5, 5)
        yaw = random.randint(0, 7)

        # move to random point
        drone.set_target(x, y, yaw)

        # control rate of loop
        rate.sleep()


if __name__=="__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
