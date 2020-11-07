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
	bool found =  false;
	int index = 0;
	 
	for(int i = 0; i<img.width*img.height;i++)
	{
		// Detection of white, Note the ball has to be very close to the camera for detection.
		if (img.data[i]  == white_pixel && img.data[i+1]  == white_pixel && img.data[i+2]  == white_pixel )
		{
			found = true;
			index = i;
			break;
		}
	}
	if (found)
	{
				// dividing  image into 3 sections 
			if(std::abs(index - ((int)(index /img.width) * (int)(img.width))) < (img.width/3))
			{
				 ROS_INFO("Ball in the Left");
				 drive_robot(0.25,0.5);
			}
			else if(std::abs(index - ((int)(index /img.width) * (int)(img.width))) < (img.width/3)*2)
			{
				ROS_INFO("Ball in the straight ahead");
				drive_robot(0.25, 0.0);
			}
			else
			{
				ROS_INFO("Ball in the right");
				drive_robot(0.25, -0.5);
			}
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
	client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command");
	
	// Subscriber to image
	ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 10,  process_image_callback);
	
	// Ros comm events handler
	ros::spin();
	
	return 0;
}
