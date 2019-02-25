# Tutorial examples

This page describes two examples showing how to use the provided drone library. 
The code shown below is available in `catkin_ws/src/techathon/src` and runs without modifications. 
Note that the examples can only be run when the simulator is running aswell. 

## Takeoff and random movement

This example shows how to setup the drone, prepare it for takeoff and then fly around programatically. 

```python
#!/usr/bin/env python

import time
import random

import rospy

from dronelib import Drone

rospy.init_node("dronelib_example")

# Create a drone object, sets up necessary connections to simulator
drone = Drone()

# Arm the drone, which allows it to receive commands
drone.activate()

# Takeoff to default height setpoint
drone.takeoff()

# Assume takeoff takes less than 5 seconds and wait for it to complete
time.sleep(5)

rate = rospy.Rate(0.1)
while not rospy.is_shutdown():
    # Generate some random point and rotation
    x = random.randint(-5, 5)
    y = random.randint(-5, 5)
    yaw = random.randint(0, 7)

    # Move to random point
    drone.set_target(x, y, yaw)

    # Control rate of loop
    rate.sleep()
```

It can be run in several ways, but we recommend to run it though ROS by calling
```
rosrun techathon random_movement.py
```
Running it through ROS ensures that the correct version of python is being used. 
ROS is mostly compatible with python 3, but some packages in the community still
use python 2, so it is recommended to use python 2 for this project. 



## Controlled takeoff

This example shows how to set a custom takeoff altitude, and how to get the position of the drone and use it to monitor the takeoff.

```python
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

## Takeoff completed, do something useful!
# Generate some random point
x = random.randint(-5, 5)
y = random.randint(-5, 5)
yaw = random.randint(0, 7)

# Move to random point
drone.set_target(x, y, yaw)
```

It can be run in several ways, but we recommend to run it though ROS by calling
```
rosrun techathon controlled_takeoff.py
```
