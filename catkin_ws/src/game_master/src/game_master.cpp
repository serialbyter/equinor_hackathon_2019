#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/ParamSet.h>
#include <mavros_msgs/ParamGet.h>
#include <mavros_msgs/ParamValue.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>

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
std::vector<Zone> boosts;
ros::Publisher goal_pub;
ros::Publisher boost_pub;

template<typename T>
T loadRequired(std::shared_ptr<ros::NodeHandle> nh, const std::string& param) {
    T value;
    if (nh->getParam(param, value)){
        return value;
    }

    ROS_FATAL("missing required argument: %s", param.c_str());
    throw std::exception();
};

void droneStateCallback(mavros_msgs::State::ConstPtr msg) {

}

geometry_msgs::PoseStamped::ConstPtr drone_pose_p;
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

void publishBoost(const ros::TimerEvent& e){
    geometry_msgs::PoseArray msg;
    geometry_msgs::Pose pose;
    if(!boosts.empty()){
        for (auto it = boosts.begin(); it != boosts.end(); it++){
            pose.position.x = (it->getMaxX() + it->getMinX()) / 2.f;
            pose.position.y = (it->getMaxY() + it->getMinY()) / 2.f;
            pose.position.z = 2.f;
            pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0, 0, it->getYaw());
            msg.poses.push_back(pose);
        }
    }
    boost_pub.publish(msg);

}

int main(int argc, char* argv[]){
    ros::init(argc, argv, "game_master");
    std::shared_ptr<ros::NodeHandle> nh = std::make_shared<ros::NodeHandle>("~");

    ros::ServiceClient client = nh->serviceClient<std_srvs::Empty>("/publish_scores");

    const std::string csv_file = loadRequired<std::string>(nh, "csv_map");
    const std::string scorefile = loadRequired<std::string>(nh, "scorefile");

    GlobalMapService mapService = GlobalMapService(nh, csv_file);
    std::vector<Zone> zones = mapService.getMapHandleRef().getGoalzones();
    if (zones.empty()) {
	      std::string errormsg = "No goal zones: ";
	      errormsg += csv_file;
        ROS_FATAL("%s", errormsg.c_str());
        return -1;
    }
    if (zones.size() != 1) {
	      std::string errormsg = "Multiple goal zones: ";
	      errormsg += csv_file;
        ROS_FATAL("%s", errormsg.c_str());
        return -1;
    }
    zone = zones.front();

    boosts = mapService.getMapHandleRef().getBoostZones();

    ros::ServiceClient mavparam_get = nh->serviceClient<mavros_msgs::ParamGet>("mavros/param/get");
    ros::ServiceClient mavparam_set = nh->serviceClient<mavros_msgs::ParamSet>("mavros/param/set");

    MissionTimer timer;

    ros::Subscriber drone_pose = nh->subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, dronePoseCallback);
    goal_pub = nh->advertise<geometry_msgs::Pose>("/goal", 1);
    
    boost_pub = nh->advertise<geometry_msgs::PoseArray>("/boost", 1);

    ros::Timer pub_timer_1 = nh->createTimer(ros::Duration(1.f), &publishGoal);
    ros::Timer pub_timer_2 = nh->createTimer(ros::Duration(1.f), &publishBoost);

    ros::Rate rate(1.f);
    ROS_INFO("Waiting for mission to start");

    //Wait for mission to start
    while (ros::ok()){
        if (drone_pose_p && drone_pose_p->pose.position.z > 0.5) {
            break;
        }
        
        ros::spinOnce();
        rate.sleep();
    }
    timer.start();

    // Wait for drone to succeed
    double max_z = drone_pose_p->pose.position.z;
    bool success = false;
    while (!success && ros::ok()) {
        const auto& pos = drone_pose_p->pose.position;

        max_z = std::max(max_z, pos.z);

        if (success || zone.isInside(pos.x, pos.y)) {
            ROS_INFO("Drone is inside zone");
            // maybe check for more stuff here
            success = true;
            break;
        }
        else {
            ROS_INFO_THROTTLE(0.1, "Drone is not in zone: (%f, %f)", pos.x, pos.y);
        }

        for(auto it=boosts.begin(); it!=boosts.end(); it++){
            if(it->isInside(pos.x, pos.y)){

                mavros_msgs::ParamGet get_srv;
                get_srv.request.param_id = "MPC_XY_VEL_MAX";
                mavparam_get.call(get_srv);
                
                float current_max_speed = float(get_srv.response.value.real);
                mavros_msgs::ParamSet set_srv;
                set_srv.request.param_id = "MPC_XY_VEL_MAX";
                
                mavros_msgs::ParamValue new_max_speed;
                new_max_speed.integer = 0;
                new_max_speed.real = current_max_speed+1.0;

                set_srv.request.value = new_max_speed;
                mavparam_set.call(set_srv);

                boosts.erase(it);
            }
        }

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


