#include "carl_estop/carl_estop_node.h"
/*
 * Constructor to inialize the node
 */
carl_estop::carl_estop() 
{
	//set the last recieve to now
	last_receive = ros::Time::now();

	//grab the parameters
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param<double>("stop_time_delay", stop_time_delay, 1.0);
	private_node_handle_.param<double>("check_frequency", check_frequency, 2.0);
	
	spoke = false;

	//setup the subscriber
    estop_sub = node.subscribe("carl_estop", 1000, &carl_estop::update_time, this);
    
    // Connect to the move_base action server
    actionClient = new ActionClient("move_base", true); // create a thread to handle subscriptions.
	
}

/*
   * helper function to determine if calcel goal actions should be taken
   *
   * \param msg the empty message
   */
void carl_estop::estop(void)
{
	ros::Time current_time = ros::Time::now();
	
	//check it has not been too long without a check
	if((current_time.toSec() - last_receive.toSec()) > stop_time_delay){
		if(!spoke)
		{
			spoke = true; 
			ROS_ERROR("Stopping! Connection Lost.");
			system("espeak \"Stopping! Connection Lost.\"");
		}
		//stop the robot
		actionClient->waitForServer();
		actionClient->cancelAllGoals();
	} else {
		if(spoke)
		{
			ROS_INFO("Connection Resumed.");
			system("espeak \"Resuming Normal Operation.\"");	
			spoke = false;
		}
	}
}

/*
   * carl_estop topic callback function.
   *
   * \param msg the empty message
   */
void carl_estop::update_time(const std_msgs::Empty::ConstPtr& msg)
{	
	//get the current time
	last_receive = ros::Time::now();
} 

double carl_estop::get_frequency()
{
	return check_frequency;
}

/*
 * main function
 */
int main(int argc, char **argv)
{
	//initialize the node
	ros::init(argc, argv, "carl_estop");

	// initialize the joystick controller
	carl_estop carl;

	//main loop
	ros::Rate loop_rate(carl.get_frequency());
	while (ros::ok()) 
	{
		carl.estop();
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;

	return EXIT_SUCCESS;
}
