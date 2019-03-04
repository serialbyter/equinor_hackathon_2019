#include <ros/ros.h>
#include <mutex>
#include <geometry_msgs/PoseStamped.h>
#include <gazebo_msgs/ModelStates.h>


gazebo_msgs::ModelStates::ConstPtr model_states_p = nullptr;
void callback(const gazebo_msgs::ModelStates::ConstPtr msg){
  model_states_p = msg;
}

constexpr auto gazebo_topic = "gazebo/model_states";
constexpr auto mocap_topic = "mavros/mocap/pose";
constexpr auto drone_name = "equidrone";


int main(int argc, char** argv){
  ros::init(argc, argv, "mocap_node");

  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<gazebo_msgs::ModelStates>(gazebo_topic,1, &callback);
  ros::Publisher mocap_pub = n.advertise<geometry_msgs::PoseStamped>(mocap_topic, 1);

  ros::Rate rate(30.f);
  while (ros::ok()){
    rate.sleep();
    ros::spinOnce();

    if (model_states_p) {
      const auto drone_state_p = std::find(model_states_p->name.cbegin(), model_states_p->name.cend(), drone_name);
      if (drone_state_p == model_states_p->name.cend()){
        ROS_ERROR_STREAM("Cant locate " << drone_name << " in gazebo/model_states");
        continue;
      }
      const int index = static_cast<int>(drone_state_p - model_states_p->name.cbegin()); 

      const auto& position = model_states_p->pose[index].position;
      const auto& orientation = model_states_p->pose[index].orientation;

      geometry_msgs::PoseStamped mocap_data;
      mocap_data.header.frame_id = "map";
      mocap_data.header.stamp = ros::Time::now();
      mocap_data.pose.position = position;
      mocap_data.pose.orientation = orientation;
   
      mocap_pub.publish(mocap_data);
    }
  }
}



