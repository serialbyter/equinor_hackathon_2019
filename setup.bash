#!/bin/bash

git submodule update --recursive --init

bash ./software/install_ros.sh
bash ./software/install_gzweb.sh
bash ./software/install_gazebo_dev_tools.sh

source /opt/ros/melodic/setup.bash
cd catkin_ws
catkin_make
