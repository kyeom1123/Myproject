#ifndef TEMPERATURE_SUBSCRIBER_HPP
#define TEMPERATURE_SUBSCRIBER_HPP

#include "sensor_subscriber_base.hpp"
#include <sensor_msgs/msg/temperature.hpp>

class TemperatureSubscriber : public SensorSubscriberBase<sensor_msgs::msg::Temperature>
{
public:
  TemperatureSubscriber();

protected:
  void handle_message(const sensor_msgs::msg::Temperature::SharedPtr msg) override;
};

#endif // TEMPERATURE_SUBSCRIBER_HPP
