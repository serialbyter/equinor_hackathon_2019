#!/bin/bash

# Follow guide on http://wiki.ros.org/melodic/Installation/Ubuntu
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt update
sudo apt install -y ros-melodic-ros-base \
  ros-melodic-gazebo-ros \
  ros-melodic-gazebo-msgs \
  ros-melodic-mavros \
  ros-melodic-mavros-msgs \
  ros-melodic-vision-opencv \
  libopencv-dev 

sudo rosdep init
rosdep update

echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt install -y python-rosinstall python-rosinstall-generator python-wstool build-essential
sudo apt install -y python-pip
python -m pip install --user --upgrade pip
python -m pip install --user jinja2 numpy toml

sudo rosdep fix-permissions


curl https://raw.githubusercontent.com/mavlink/mavros/master/mavros/scripts/install_geographiclib_datasets.sh | sudo bash -
