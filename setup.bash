#!/bin/bash

git submodule update --recursive --init

bash ./software/install_utils.sh
bash ./software/install_ros.sh
bash ./software/install_gzweb.sh
bash ./software/install_gazebo_dev_tools.sh
bash ./software/install_scoreboard_deps.sh

source /opt/ros/melodic/setup.bash
cd catkin_ws
catkin_make

echo "source $HOME/equinor_hackathon_2019/catkin_ws/devel/setup.bash" >> $HOME/.bashrc
echo "cd $HOME/equinor_hackathon_2019/catkin_ws/src/techathon/src" >> $HOME/.bashrc
