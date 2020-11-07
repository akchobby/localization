#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <geometry_msgs/Twist.h>

// Global joint publisher variables
ros::Publisher motor_command_publisher;


// This callback function executes whenever a safe_move service is requested
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res)
{

    ROS_INFO("DriveToTarget Request received - linear:%1.2f, angular:%1.2f", (float)req.linear_x, (float)req.angular_z);


    // Publish velocties
    geometry_msgs::Twist cmd_vel;
    
    cmd_vel.linear.x = req.linear_x;
    cmd_vel.angular.z = req.angular_z;
    
    motor_command_publisher.publish(cmd_vel);

    // Return a response message
    res.msg_feedback = "Robot set in motion at a linear velocity:" + std::to_string(req.linear_x)+ " and angular velocity :" + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc, char** argv)
{
    // Initialize the drive_bot node and  a handle 
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    // Defining the publisher 
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    

    // Define a safe_move service with a handle_safe_move_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command", handle_drive_request);
    ROS_INFO("Ready to send robot commands");

    // Handle ROS communication events
    ros::spin();

    return 0;
}
