# Examples and template explanation

This page describes two examples showing how to use the provided drone library. 
The code shown below is available in `catkin_ws/src/techathon/src` and runs without modifications. 
Note that the examples can only be run when the simulator is running aswell. 

Below the examples we describe important snippets from the template file `main.py` in more detail. 

## Takeoff and random movement

This example shows how to setup the drone, prepare it for takeoff and then fly around programatically. 

```python
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
    
    # Main loop
    rate = rospy.Rate(0.1)
    while not rospy.is_shutdown():
        # control rate of loop
        rate.sleep()
        
        # generate some random point and rotation
        x = random.randint(-3, 3)
        y = random.randint(-3, 3)

        # move to random point
        drone.set_target(x, y)

if __name__=="__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
```

Run it through ROS by calling
```
rosrun techathon random_movement.py
```
Running it through ROS ensures that the correct version of python is being used. 
ROS is mostly compatible with python 3, but some packages in the community still
use python 2.7, so it is highly recommended to use python 2.7 for this project. 

## Controlled takeoff

This example shows how to set a custom takeoff altitude, and how to get the position of the drone and use it to monitor the takeoff.

```python
#!/usr/bin/env python
from __future__ import print_function

import time
import random
import rospy
from dronelib import Drone

def main():
    rospy.init_node("dronelib_example")

    # Create drone, activate it and send takeoff command
    drone = Drone()
    drone.activate()

    # Takeoff and override default takeoff height (3 meters)
    drone.takeoff(height=2.0)

    # Monitor takeoff and dont progress until we are high enough
    rate = rospy.Rate(1.0)
    while not rospy.is_shutdown():
        # Control rate of while loop
        rate.sleep()
        
        # Monitor height
        if drone.position.z > 1.5:
            break

    print("Takeoff complete")

    ## Takeoff completed, do something useful
    # Generate some random point
    x = random.randint(-3, 3)
    y = random.randint(-3, 3)

    # Move to random point
    drone.set_target(x, y)

if __name__=="__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass
```

Run it through ROS by calling
```
rosrun techathon controlled_takeoff.py
```

## A closer look at main.py
Here is the main.py template in its entirety. 
```python
#!/usr/bin/env python
from __future__ import print_function
import rospy
import random
import math
from dronelib import Drone
import util
from ascend_msgs.srv import GlobalMap
from geometry_msgs.msg import Pose, PoseArray

def goal_callback(msg):
    global goal
    goal = msg.position

def dynamic_obstacles_callback(msg):
    global obstacles
    obstacles = msg.poses

def boost_callback(msg):
    global boosts
    boosts = msg.poses

def main():
    # Init ROS node
    rospy.init_node('task', anonymous=True)

    # Create subscriber for position, goal, boost points, and obstacles
    rospy.Subscriber('/goal', Pose, goal_callback)
    rospy.Subscriber('/boost', PoseArray, boost_callback)
    rospy.Subscriber("/dynamic_obstacles", PoseArray, dynamic_obstacles_callback)

    # Wait for resources to become active
    goal = rospy.wait_for_message("/goal", Pose).position
    boosts = rospy.wait_for_message("/boost", PoseArray).poses
    obstacles = rospy.wait_for_message("/dynamic_obstacles", PoseArray).poses

    # Create map service client
    getMap = rospy.ServiceProxy('/GlobalMap', GlobalMap)
    rospy.wait_for_service('/GlobalMap')

    try:
        raw_map = getMap()
    except rospy.ServiceException as e:
        print("Map service error: " + str(e))
        return

    # Get map as 2D list
    world_map = util.parse_map(raw_map)

    # Print resources
    print("Wall layout:")
    util.print_map(world_map)
    print("Boost points:")
    util.print_positions(boosts)
    print("Obstacles at start:")
    util.print_positions(obstacles)

    # Initialize drone
    drone = Drone()
    drone.activate()
    drone.takeoff()

    # -- For example code --
    target_x = 0
    target_y = 0

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

        # Do special action if we are close
        if distance_to_target < 0.5:
            # Print current distance to goal. Note that we 
            # wont reach the goal, since we just move randomly
            distance_to_goal = ((drone.position.x - goal.x)**2 +
                                (drone.position.y - goal.y)**2)**0.5

            print("Distance to goal is now", distance_to_goal)

            # Generate some random point and rotation
            target_x = random.randint(-3, 3)
            target_y = random.randint(-3, 3)

            # Move to random point
            drone.set_target(target_x, target_y)
        else:
            print("distance to target:", distance_to_target)


if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
```

`main.py` is a decently long file, but most of it is setup for interacting with the simulator and getting resources. We'll skip how this is done and focus on what you need to know to get started. After loading the map information, we print that information to the console.
```python
# Print resources
print("Wall layout:")
util.print_map(world_map)
print("Boost points:")
util.print_positions(boosts)
print("Obstacles at start:")
util.print_positions(obstacles)
```
This allows you to make sure the datastructures are set up before starting the drone. The drone interface is set up in the following lines.
```python
# Initialize drone
drone = Drone()
drone.activate()
drone.takeoff()
```

Note how the main while loop is defined.
```python
rate = rospy.Rate(30)
while not rospy.is_shutdown():
    rate.sleep()
    # .....
```
It is important to keep the `rate` object and `rate.sleep()`, since this will limit how fast this while loop spins, freeing up computing resources for use in the physics simulator. Everything inside the while loop except `rate.sleep()` can be removed when solving the problem. 

Before closing this section we'll look at how we can get the position of the drone. It is done in the following lines in main.py.
```python
# Find how far we are away from target
distance_to_target = ((target_x - drone.position.x)**2 +
                      (target_y - drone.position.y)**2)**0.5
```
`target_x` and `target_y` are varibles defined above. `drone.position` is how we access the position of the drone. It continously updates behind the scenes so it is always up to date. 


