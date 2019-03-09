#include <ros/ros.h>
#include <gazebo_msgs/ModelStates.h>
#include <geometry_msgs/PoseArray.h>

constexpr auto gazebo_topic = "/gazebo/model_states";
constexpr auto obstacle_topic = "/dynamic_obstacles";

gazebo_msgs::ModelStates::ConstPtr model_states_p;
void callback(const gazebo_msgs::ModelStates::ConstPtr msg){
  model_states_p = msg;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "dynamic_obstacle_publisher_node");

  ros::NodeHandle n;
  ros::Subscriber gazebo_sub = n.subscribe<gazebo_msgs::ModelStates>(gazebo_topic,1, &callback);
  ros::Publisher obstacle_pub = n.advertise<geometry_msgs::PoseArray>(obstacle_topic, 1);

  // make sure callback has been called
  ros::topic::waitForMessage<gazebo_msgs::ModelStates>(gazebo_topic);
  ros::spinOnce(); 

  ros::Rate rate(30.f);
  while (ros::ok()) {
    rate.sleep();
    ros::spinOnce();

    const int len = model_states_p->name.size();
    const auto& name = model_states_p->name;
    const auto& pose = model_states_p->pose;
		geometry_msgs::PoseArray obstacle_msg;
    for (int i = 0; i < len; i++) {
      if (name[i].find("obstacle") != std::string::npos) {
				// Object is obstacle
        obstacle_msg.poses.push_back(pose[i]);
      }
    }  

    obstacle_msg.header.stamp = ros::Time::now();
    obstacle_msg.header.frame_id = "map";

    obstacle_pub.publish(obstacle_msg);
  }

  return 0;
}

