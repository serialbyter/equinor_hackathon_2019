#!/bin/bash

sh_file_path=$(realpath $(dirname $0))
local_models_path=${sh_file_path}/models
px4_src_path=${sh_file_path}/../../../PX4_2017
px4_build_path=${px4_src_path}/build_posix_sitl_lpe

source ${px4_src_path}/Tools/setup_gazebo.bash ${px4_src_path} ${px4_build_path} 

# Add local models
GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:${local_models_path}

# Kill previous instances 
pkill -x gzserver

if [ -z "$DONT_RUN" ]
then
    #gazebo --verbose ${sh_file_path}/worlds/flyablemap.world 
    #gazebo --verbose ${sh_file_path}/testing.world 
    TASK=task1 roslaunch equinor2018_simulator testing.launch gui:=false

#    sleep 3
#    nice -n 20 gzclient --verbose
fi

