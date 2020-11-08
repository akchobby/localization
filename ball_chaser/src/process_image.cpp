#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>


// Global Client
ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z)
{
	
	// Setting the velocities to the request
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;
	
	// Call the command service 
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command");
        
    // sleep to allow robot to move
    ros::Duration(0.1).sleep();
}

void process_image_callback(const sensor_msgs::Image img)
{
	int white_pixel = 255;
	int x_center_val = 0;
	int x_val_sum = 0;
	int mass = 0;
	float kp = 0.01; // proportional gain
	 
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.step; j+=3)
		{
			int pixel_index = (i * img.step) + j;
			
			// Detection of white,
			if (img.data[pixel_index]  == white_pixel && img.data[pixel_index+1]  == white_pixel && img.data[pixel_index+2]  == white_pixel )
			{
				x_val_sum +=  (j / 3);
				mass++;
			}
		}
	}
	

	if (mass > 0)
	{
			x_center_val = x_val_sum /  mass;
			int side = (img.width / 2) - x_center_val; 

			// dividing  image into 3 sections 
			if(side > 0)
			{
				 ROS_INFO("Ball in the Left");
			}
			else if(side < 0 )
			{
				ROS_INFO("Ball in the right");
			}
			else
			{
				ROS_INFO("Ball straight ahead");
			}
			
			drive_robot(0.3, ((float) side) * kp);
			
	}
	else
	{
			ROS_INFO("Ball not found !");
			drive_robot(0.0, 0.0);
			
	}
		
}

int main(int argc, char** argv)
{
	//Initializing node and handle
	ros::init(argc, argv, "process_image");
	ros::NodeHandle n;
	
	// Client definition
	client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
	
	// Subscriber to image
	ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 10,  process_image_callback);
	
	// Ros comm events handler
	ros::spin();
	
	return 0;
}
