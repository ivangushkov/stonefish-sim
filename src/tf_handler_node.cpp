#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

#include "rclcpp/rclcpp.hpp"

#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"


using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class TFHandler : public rclcpp::Node
{
  public: TFHandler() : Node("stonefish_tf_handler")
    {

        /*
        Ideally we want a node with the following functionality:
        1) Gets frame names, and an odometry topic
        2) Creates a subscriber for the topic and a publisher for the tf
        3) Callback on subscriber to blast the odometry over the publisher
        */ 
        
        //The frames and odom topic.
        
        this->declare_parameter("world_frame", "odom");
        this->declare_parameter("robot_id", "robot");
        this->declare_parameter("odom_topic", "");

        RCLCPP_INFO(this->get_logger(), "World frame given: '%s'", this->get_parameter("world_frame").as_string().c_str());
        RCLCPP_INFO(this->get_logger(), "Robot id frame given: '%s'", this->get_parameter("robot_id").as_string().c_str());
        RCLCPP_INFO(this->get_logger(), "Odom topic given: '%s'", this->get_parameter("odom_topic").as_string().c_str());

        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
        
        std::ostringstream stream;
        stream << "" << this->get_parameter("robot_id").as_string().c_str() << "/base_link";
        child_frame_id = stream.str();

        subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
        this->get_parameter("odom_topic").as_string(), 10,
        std::bind(&TFHandler::handle_odom, this, std::placeholders::_1));
        
    }

  private:
    void handle_odom(const std::shared_ptr<const nav_msgs::msg::Odometry> msg)
    {
    geometry_msgs::msg::TransformStamped t;

    // Read message content and assign it to
    // corresponding tf variables
    t.header.stamp = msg->header.stamp;
    t.header.frame_id = this->get_parameter("world_frame").as_string().c_str();
    t.child_frame_id = child_frame_id.c_str();

    // Position data into transform
    t.transform.translation.x = msg->pose.pose.position.x;
    t.transform.translation.y = msg->pose.pose.position.y;
    t.transform.translation.z = msg->pose.pose.position.z;

    // Orientation data into transform
    t.transform.rotation.x = msg->pose.pose.orientation.x;
    t.transform.rotation.y = msg->pose.pose.orientation.y;
    t.transform.rotation.z = msg->pose.pose.orientation.z;
    t.transform.rotation.w = msg->pose.pose.orientation.w;
    
    // Send the transformation
    tf_broadcaster_->sendTransform(t);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    std::string child_frame_id;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TFHandler>());
  rclcpp::shutdown();
  return 0;
}