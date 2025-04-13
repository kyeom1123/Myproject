#include "temperature_subscriber.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TemperatureSubscriber>());
  rclcpp::shutdown();
  return 0;
}
