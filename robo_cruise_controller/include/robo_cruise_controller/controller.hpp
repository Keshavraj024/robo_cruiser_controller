#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include <vector>

class Controller : public rclcpp::Node
{
public:
    Controller();

private:
    /**
     * @brief publish to /cmd_vel topic at certain frequency
     */
    void timerCmdVelCallback();

    /**
     * @brief publish to /set_joint_trajectory topic at certain frequency
     */
    void timerTrajectoryCallback();

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr m_vel_publisher;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr m_trajectory_publisher;
    rclcpp::TimerBase::SharedPtr m_vel_timer;
    rclcpp::TimerBase::SharedPtr m_trajectory_timer;
    double m_linear_vel_x;
    double m_angular_yaw;
    double m_forearm_position;
    double m_hand_position;

    rcl_interfaces::msg::SetParametersResult paramsCallback(const std::vector<rclcpp::Parameter> &params);
    OnSetParametersCallbackHandle::SharedPtr m_params_callback_handle;
};

#endif