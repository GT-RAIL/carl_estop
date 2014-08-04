/*!
 * \pc_estop_node.h
 * \brief Allows for stopping of CARL.
 *
 * pc_estop_node creates a ROS node that allows a user to stop CARL. 
 * and sends messages to the /carl_estop topic.
 *
 * \author Russell Toris, WPI - rctoris@wpi.edu
 * \author Chris Dunkers, WPI - spkordell@wpi.edu
 * \date July 24, 2014
 */

#ifndef CARL_ESTOP_TELEOP_H_
#define CARL_ESTOP_TELEOP_H_

#include <ros/ros.h>
#include <std_msgs/Empty.h>

// the ros subscriber
ros::Publisher estop_pub;

//variables for the parameter values to be stored in
double send_frequency;

/*!
 * Creates and runs the carl_estop_node.
 *
 * \param argc argument count that is passed to ros::init
 * \param argv arguments that are passed to ros::init
 * \return 0 
 */
int main(int argc, char **argv);

#endif
