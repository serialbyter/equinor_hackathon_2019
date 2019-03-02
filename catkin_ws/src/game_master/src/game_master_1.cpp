#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>

#include "simulator/zone.h"
#include "game_master/mission_timer.h"
#include "game_master/global_map_service.h"
#include "std_srvs/Empty.h"

#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <fstream>

// global resources
Zone zone; 
ros::Publisher goal_pub; 

template<typename T>
T loadRequired(std::shared_ptr<ros::NodeHandle> nh, const std::string& param) {
    T value;
    if (nh->getParam(param, value)){
        return value;
    }

    ROS_FATAL("missing required argument: %s", param.c_str());
    throw std::exception();
};


geometry_msgs::PoseStamped::ConstPtr drone_pose_p;


void droneStateCallback(mavros_msgs::State::ConstPtr msg) {

}

void dronePoseCallback(geometry_msgs::PoseStamped::ConstPtr msg) {
    drone_pose_p = msg;
}

void publishGoal(const ros::TimerEvent& e) {
    geometry_msgs::Pose msg;
    msg.position.x = (zone.getMaxX() + zone.getMinX())/2.f;
    msg.position.y = (zone.getMaxY() + zone.getMinY())/2.f;
    msg.position.z = 2.f;
    msg.orientation = tf::createQuaternionMsgFromRollPitchYaw(0,0,zone.getYaw());
    
    goal_pub.publish(msg);
}

int main(int argc, char* argv[]){
    ros::init(argc, argv, "game_master_1");
    std::shared_ptr<ros::NodeHandle> nh = std::make_shared<ros::NodeHandle>("~");

    ros::ServiceClient client = nh->serviceClient<std_srvs::Empty>("/publish_scores");

    const std::string csv_file = loadRequired<std::string>(nh, "csv_map");
    const std::string scorefile = loadRequired<std::string>(nh, "scorefile");

    // GlobalMapService mapService = GlobalMapService(nh, csv_file);
    // std::vector<Zone> zones = mapService.getMapHandleRef().getGoalzones();
    // if (zones.empty()) {
	// std::string errormsg = "No goal zones in game 1: ";
	// errormsg += csv_file;
    //     ROS_FATAL("%s", errormsg.c_str());
    //     return -1;
    // }
    // if (zones.size() != 1) {
	// std::string errormsg = "Multiple goal zones in game 1";
	// errormsg += csv_file;
    //     ROS_FATAL("%s", errormsg.c_str());
    //     return -1;
    // }
    // zone = zones.front();
    // std::pair<float, float> start = mapService.getMapHandleRef().getStartPosition();
    MissionTimer timer;
    
    // ros::Subscriber drone_state = nh->subscribe<mavros_msgs::State>("/mavros/state", 1, droneStateCallback);
    // ros::Subscriber drone_pose = nh->subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, dronePoseCallback);
    // goal_pub = nh->advertise<geometry_msgs::Pose>("/goal", 1);
    // ros::Timer pub_timer = nh->createTimer(ros::Duration(1.f), &publishGoal);

    ros::Rate rate(0.9f);
    ROS_INFO("Waiting for mission to start");

    // Wait for mission to start
    // while (ros::ok()){
    //     if (drone_pose_p && drone_pose_p->pose.position.z > 0.5) {
    //         break;
    //     }
        
    //     ros::spinOnce();
    //     rate.sleep();
    // }
    timer.start();

    // Wait for drone to succeed
    bool success = false;
    while (!success && ros::ok()) {
        // const auto& pos = drone_pose_p->pose.position;
        // if (success || zone.isInside(pos.x, pos.y)) {
        //     ROS_INFO_THROTTLE(0.2, "Drone is inside zone");
        //     // maybe check for more stuff here
        //     success = true;
        //     break;
        // }
        // else {
        //     ROS_INFO_THROTTLE(0.2, "Drone is not in zone: (%f, %f)", pos.x, pos.y);
        // }
        success = true;
        ros::spinOnce();
        rate.sleep();
    }

    timer.stop();
    ros::Duration duration = timer.getDuration();

    ROS_INFO_COND(success, "Success! Duration: %f", duration.toSec());
    ROS_INFO_COND(!success, "Duration: %f", duration.toSec());

    if(success){
        
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ofstream outfile;
        outfile.open(scorefile, std::ios_base::app);
        outfile << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "], " << duration.toSec() << ", " << 0 << std::endl;

        std_srvs::Empty srv;

        if(client.call(srv)){
            ROS_DEBUG("Successfully sent score to server");
        } else {
            ROS_DEBUG("Failed send score to server");
        }
    }

    return 0;
}


