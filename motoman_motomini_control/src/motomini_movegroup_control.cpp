// -----------------------------------------------------------------------------
// Copyright 2019 NexCOBOT Inc.
// Author: Robin Chen
// Contact: robinchen@nexcobot.com
//
// -----------------------------------------------------------------------------
//#include <motoman_motomini_control/motomini_movegroup_control.h>
#include "../include/motoman_motomini_control/motomini_movegroup_control.h"

MoveGroupControl::MoveGroupControl(ros::NodeHandle nh,
                                   const std::string frame_name,
                                   const std::string group_name) :
  nh_(nh), frame_name_(frame_name)
{
  // Initialize move_group and set reference frame
  move_group_ptr = new moveit::planning_interface::MoveGroupInterface(group_name);
  move_group_ptr->setPoseReferenceFrame(frame_name);
  num_execute = 0;
  // Initialize subscriber to listen target joint position from mqtt_monitor
  target_jnt_sub = nh_.subscribe("/yk/target_jnt_data", 1, &MoveGroupControl::target_jnt_callback ,this);
}

MoveGroupControl::~MoveGroupControl()
{
  delete  move_group_ptr;
}

void MoveGroupControl::target_jnt_callback(const std_msgs::Float64MultiArray& target_jnt_msg)
{
  std::vector<double> joint_group_positions {};
  // Get target joint positions
  for (auto data : target_jnt_msg.data)
  {
    joint_group_positions.push_back(data);
    ROS_INFO_STREAM(data);
  }

  // Create an Asynchronized thread
  //ros::AsyncSpinner async_spinner2(1);
  //async_spinner2.start();

  // set goal and execute
  this->move_group_ptr->setJointValueTarget(joint_group_positions);
  ROS_INFO_STREAM("move_group before move()");
  this->move_group_ptr->move();
  ROS_INFO_STREAM("move_group after move()");

  //ROS_INFO_STREAM("move_group before stop()");
  //this->move_group_ptr->stop();  // use in another subscriber call_back
  //ROS_INFO_STREAM("move_group after stop()");

  //async_spinner2.stop();

  num_execute += 1;
  ROS_INFO_STREAM("Before sleep 1 sec");
  ros::Duration(1).sleep();

  ROS_INFO_STREAM("number of execute: "<< num_execute <<" Ready to leave target_jnt_callback");
  return;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motomini_movegroup_control");
  ros::NodeHandle private_node_handle("~");
  // find the name of base frame for planning
  std::string base_frame;
  private_node_handle.param<std::string>("base_frame", base_frame, "world");

  // Create an Asynchronized thread
  ros::AsyncSpinner async_spinner(2);
  async_spinner.start();

  ros::NodeHandle nh;
  MoveGroupControl app(nh, base_frame, "manipulator");

  ROS_INFO("motomini_movegroup_control node has been initialized");

  //wait for shutdown signal
  ros::waitForShutdown();

  return 0;
}
