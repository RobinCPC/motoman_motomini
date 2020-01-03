// -----------------------------------------------------------------------------
// Copyright 2019 NexCOBOT Inc.
// Author: Robin Chen
// Contact: robinchen@nexcobot.com
//
// -----------------------------------------------------------------------------
#ifndef  MOTOMINI_MOVEGROUP_CONTROL_H
#define  MOTOMINI_MOVEGROUP_CONTROL_H

#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/MultiArrayDimension.h>

class MoveGroupControl
{
  public:
    MoveGroupControl(ros::NodeHandle nh,
                     const std::string frame_name,
                     const std::string group_name);   // Constructor
    ~MoveGroupControl();                                                  // Destructor

private:
  ros::NodeHandle nh_;
  ros::Subscriber target_jnt_sub;

  void target_jnt_callback(const std_msgs::Float64MultiArray& target_jnt_msg);

  // member for move_group
  std::string frame_name_;
  moveit::planning_interface::MoveGroupInterface* move_group_ptr;
  int num_execute;
};

#endif  // MOTOMINI_MOVEGROUP_CONTROL_H
