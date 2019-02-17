#ifndef MISSION_TIMER_H
#define MISSION_TIMER_H
#include <ros/ros.h>

class MissionTimer {
    public:
        MissionTimer() = default;
        void start();
        
        void stop();

        bool isRunning() const;

        ros::Duration getDuration() const;
    private:
        enum class State { WAIT, RUN, STOP };

        ros::Time start_time_;
        ros::Time end_time_;
        State state_ = State::WAIT;
};

#endif // MISSION_TIMER_H

