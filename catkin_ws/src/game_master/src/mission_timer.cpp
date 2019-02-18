#include "game_master/mission_timer.h"
#include <ros/ros.h>

void MissionTimer::start(){
    if (state_ == State::WAIT) {
        start_time_ = ros::Time::now();
        state_ = State::RUN;
    } else {
        ROS_WARN("start called in invalid state");
    }
}

void MissionTimer::stop(){
    if (state_ == State::RUN){
        end_time_ = ros::Time::now();
        state_ = State::STOP;
    } else {
        ROS_WARN("stop called in invalid state");
    }
}

ros::Duration MissionTimer::getDuration() const {
    if (state_ == State::STOP) {
        return end_time_ - start_time_;
    }
    if (state_ == State::RUN) {
        return ros::Time::now() - start_time_;
    }

    ROS_DEBUG("getDuration called in invalid state");

    return ros::Duration(-1.f);
}


bool MissionTimer::isRunning() const {
    return state_ == State::RUN;
}

