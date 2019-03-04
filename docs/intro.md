# Introduction

## Overview
The challenge is developed using Robotics Operating System (ROS). 
ROS is a framework for making robots. 
We will use its abilities for simulation, and a physics engine.
The ROS setup contains a virtual world in which the drone can move.

Read more about ROS here: https://wiki.ros.org)

## The simulation environmenet
Each group will its own virtual server in Azure. Name and address for the server will be provided in advance. 
This server will have the complete setup for running the drone and completing the challenge.
It consists of the following parts:
- ROS installed on the server.
- A web server that lets you see the virtual world (the maze) from a web browser.
- `rosrun`, a program used for running your code inside the ROS environment.
- `Catkin`, a build system for ROS (https://wiki.ros.org/catkin). 

To operate the drone, you need to write a Python program that starts the drone and tells it how to move through 
the maze. You can use these [examples](docs/examples.md) as a starting point.
