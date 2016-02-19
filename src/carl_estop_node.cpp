#include <carl_estop/carl_estop_node.h>

carl_estop::carl_estop()
{
  //set the last recieve to now
  last_receive = ros::Time::now();

  //grab the parameters
  ros::NodeHandle private_nh("~");
  private_nh.param<double>("stop_time_delay", stop_time_delay, 1.5);
  private_nh.param<double>("check_frequency", check_frequency, 3.0);

  spoke = false;

  //setup the subscriber
  estop_sub = node.subscribe("carl_estop", 1000, &carl_estop::update_time, this);

  //setup the publisher to the segway
  rmp_pub = node.advertise<rmp_msgs::RMPCommand>("rmp_command", 10);

  // Connect to the move_base action server
  actionClient = new ActionClient("move_base", true); // create a thread to handle subscriptions.
  last_receive = ros::Time::now();
}

void carl_estop::estop(void)
{
  ros::Time current_time = ros::Time::now();

  //check it has not been too long without a check
  if ((current_time.toSec() - last_receive.toSec()) > stop_time_delay)
  {
    if (!spoke)
    {
      spoke = true;
      ROS_ERROR("Stopping! Estop Connection Lost.");
      system("espeak \"Stopping! Estop Connection Lost.\"");
    }
    //stop the robot
    actionClient->waitForServer();
    actionClient->cancelAllGoals();
    rmp.cmd_id = rmp_msgs::RMPCommand::RMP_CFG_CMD_ID;
    rmp.arg1 = rmp_msgs::RMPCommand::RMP_CMD_SET_OPERATIONAL_MODE;
    rmp.arg2 = rmp_msgs::RMPCommand::STANDBY_REQUEST;
    rmp_pub.publish(rmp);
  }
  else
  {
    if (spoke)
    {
      ROS_INFO("Estop Connection Resumed.");
      system("espeak \"Estop Connection Resumed.\"");
      spoke = false;
      rmp.cmd_id = rmp_msgs::RMPCommand::RMP_CFG_CMD_ID;
      rmp.arg1 = rmp_msgs::RMPCommand::RMP_CMD_SET_OPERATIONAL_MODE;
      rmp.arg2 = rmp_msgs::RMPCommand::TRACTOR_REQUEST;
      rmp_pub.publish(rmp);
    }
  }
}

void carl_estop::update_time(const std_msgs::Empty::ConstPtr& msg)
{
  //get the current time
  last_receive = ros::Time::now();
}

double carl_estop::get_frequency()
{
  return check_frequency;
}

int main(int argc, char **argv)
{
  //initialize the node
  ros::init(argc, argv, "carl_estop");

  // initialize the estop
  carl_estop carl;

  //main loop
  ros::Rate loop_rate(carl.get_frequency());
  while (ros::ok())
  {
    ros::spinOnce();
    carl.estop();
    loop_rate.sleep();
  }

  return EXIT_SUCCESS;
}
