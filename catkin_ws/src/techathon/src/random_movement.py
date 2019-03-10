#!/usr/bin/env python
"""Example moving the drone around

This code demonstrates basic control of drone.
It shows how to setup the drone, takeoff and how
to move it around. 

How to run:
    Make sure the simulator is running, and run the following 
    in a separate terminal window
    $ rosrun techathon random_movement.py
"""

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
        x = random.randint(-3, 3)
        y = random.randint(-3, 3)

        # move to random point
        drone.set_target(x, y)

        # control rate of loop
        rate.sleep()


if __name__=="__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
