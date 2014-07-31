/*!
 * \pc_estop_node.cpp
 * \brief Allows for stopping of CARL.
 *
 * pc_estop_node creates a ROS node that allows a user to stop CARL.
 * and sends messages to the /carl_estop topic.
 *
 * \author Russell Toris, WPI - rctoris@wpi.edu
 * \author Chris Dunkers, WPI - spkordell@wpi.edu
 * \date July 24, 2014
 */

#include <ros/ros.h>
#include <std_msgs/Empty.h>

/*!
 * Creates and runs the carl_estop_node.
 *
 * \param argc argument count that is passed to ros::init
 * \param argv arguments that are passed to ros::init
 * \return EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
  // initialize the node
  ros::init(argc, argv, "pc_estop");

  // main node handle
  ros::NodeHandle node;

  // grab the parameters
  ros::NodeHandle private_nh("~");
  double send_frequency;
  private_nh.param<double>("send_frequency", send_frequency, 3.0);

  //setup the publisher
  ros::Publisher estop_pub = node.advertise<std_msgs::Empty>("carl_estop", 1);
  std_msgs::Empty msg;

  //main publish loop
  ros::Rate loop_rate(send_frequency);
  while (ros::ok())
  {
    estop_pub.publish(msg);
    loop_rate.sleep();
  }

  return EXIT_SUCCESS;
}
