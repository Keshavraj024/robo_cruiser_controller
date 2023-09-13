#include <robo_cruise_controller/controller.hpp>
#include <thread>

Controller::Controller() : Node("robo_cruise_controller_node")
{
    this->declare_parameter("linear_vel_x", 0.0);
    this->declare_parameter("angular_yaw", 0.0);
    this->declare_parameter("left_wheel_position", 0.0);
    this->declare_parameter("right_wheel_position", 0.0);

    m_linear_vel_x = this->get_parameter("linear_vel_x").as_double();
    m_angular_yaw = this->get_parameter("angular_yaw").as_double();
    m_forearm_position = this->get_parameter("left_wheel_position").as_double();
    m_hand_position = this->get_parameter("right_wheel_position").as_double();

    m_vel_publisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    m_vel_timer = this->create_wall_timer(std::chrono::seconds(1), std::bind(&Controller::timerCmdVelCallback, this));

    m_trajectory_publisher = this->create_publisher<trajectory_msgs::msg::JointTrajectory>("set_joint_trajectory", 10);
    m_trajectory_timer = this->create_wall_timer(std::chrono::seconds(10), std::bind(&Controller::timerTrajectoryCallback, this));

    m_params_callback_handle = this->add_on_set_parameters_callback(std::bind(&Controller::paramsCallback, this, std::placeholders::_1));
}

void Controller::timerCmdVelCallback()
{
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = m_linear_vel_x;
    msg.angular.z = m_angular_yaw;
    m_vel_publisher->publish(msg);
}

void Controller::timerTrajectoryCallback()
{
    auto msg = trajectory_msgs::msg::JointTrajectory();
    msg.header.frame_id = "base_footprint";
    msg.joint_names = {"base_front_left_steering_wheel_joint", "base_front_right_steering_wheel_joint"};

    trajectory_msgs::msg::JointTrajectoryPoint point;
    point.positions = {m_forearm_position, m_hand_position};
    msg.points.push_back(point);
    m_trajectory_publisher->publish(msg);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    point.positions = {0.0, 0.0};
    msg.points.push_back(point);
    m_trajectory_publisher->publish(msg);
}

rcl_interfaces::msg::SetParametersResult Controller::paramsCallback(const std::vector<rclcpp::Parameter> &params)
{
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    result.reason = "success";

    for (const auto &param : params)
    {
        if (param.get_name() == "linear_vel_x")
        {
            if (param.as_double() >= -2.0 && param.as_double() <= 2.0)
            {
                m_linear_vel_x = param.as_double();
            }
            else
            {
                result.successful = false;
                result.reason = "Linear Velocity is out of bounds";
                return result;
            }
        }
        if (param.get_name() == "angular_yaw")
        {
            m_angular_yaw = param.as_double();
        }
    }
    return result;
}


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Controller>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}