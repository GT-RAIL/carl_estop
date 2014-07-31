/*!
 * \carl_estop_node.h
 * Allows the stopping of the segway base without losing power to peripherals
 *
 * carl_estop_node creates a ROS node that allows the stopping of the robot after a specified amount of time.
 * This node listens to a /carl_estop topic
 * and sends messages to the /move_base actionlib to stop the current execution of a goal.
 *
 * \author Russell Toris, WPI - rctoris@wpi.edu
 * \author Chris Dunkers, WPI - cmdunkers@wpi.edu
 * \date July 24, 2014
 */

#ifndef CARL_ESTOP_H_
#define CARL_ESTOP_H_

#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <ros_ethernet_rmp/RMPCommand.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ActionClient;

class carl_estop
{
public:
  carl_estop();
  double get_frequency();

  /*!
   * estop function to check the time difference to see if any goal position in move_base should be cancelled
   *
   * \param msg the empty message
   */
  void estop(void);

private:
  //main node handle
  ros::NodeHandle node;

  // the ros subscriber
  ros::Subscriber estop_sub;

  // ros publisher to rmp
  ros::Publisher rmp_pub;

  //variables for the parameter values to be stored in
  double stop_time_delay;
  double check_frequency;
  ros::Time last_receive;

  //variable used to make sure robot only says the connection lost once
  bool spoke;

  // A handle for the move_base action client thread
  ActionClient* actionClient;

  //message for the rmp
  ros_ethernet_rmp::RMPCommand rmp;

  /*!
   * carl_estop topic callback function.
   *
   * \param msg the empty message
   */
  void update_time(const std_msgs::Empty::ConstPtr& msg);

};

/*!
 * Creates and runs the carl_estop_node.
 *
 * \param argc argument count that is passed to ros::init
 * \param argv arguments that are passed to ros::init
 * \return EXIT_SUCCESS
 */
int main(int argc, char **argv);

#endif
