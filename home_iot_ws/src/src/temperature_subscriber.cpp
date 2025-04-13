#include "temperature_subscriber.hpp"
#include <rclcpp/rclcpp.hpp>

TemperatureSubscriber::TemperatureSubscriber()
: SensorSubscriberBase<sensor_msgs::msg::Temperature>("temperature_subscriber", "temperature")
{
  RCLCPP_INFO(this->get_logger(), "TemperatureSubscriber initialized");
}

void TemperatureSubscriber::handle_message(const sensor_msgs::msg::Temperature::SharedPtr msg)
{
  float temp = msg->temperature;
  RCLCPP_INFO(this->get_logger(), "Received temperature: %.2f °C", temp);
  if (temp > 30.0) {
    RCLCPP_WARN(this->get_logger(), "Warning: High temperature!");
  }
}
