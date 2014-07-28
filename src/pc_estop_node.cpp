#include "ros/ros.h"
#include "std_msgs/Empty.h"

ros::Publisher estop_pub;

double send_frequency;

int main (int argc, char **argv) 
{
	//initialize the node
	ros::init(argc, argv, "pc_estop");

	//main node handle
	ros::NodeHandle node;

	//grab the parameters
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param<double>("send_frequency", send_frequency, 2.0);

	//setup the publisher
	estop_pub = node.advertise<std_msgs::Empty>("carl_estop", 1);
	
	std_msgs::Empty msg;
	
	//main publish loop
	ros::Rate loop_rate(send_frequency);
	while (ros::ok()) 
	{
		estop_pub.publish(msg);
		loop_rate.sleep();
	}
	
	return 0;
}
