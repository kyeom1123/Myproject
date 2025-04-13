#ifndef SENSOR_SUBSCRIBER_BASE_HPP
#define SENSOR_SUBSCRIBER_BASE_HPP

#include <rclcpp/rclcpp.hpp>

template <typename MsgType>
class SensorSubscriberBase : public rclcpp::Node
{
public:
  SensorSubscriberBase(const std::string & node_name, const std::string & topic_name)
  : rclcpp::Node(node_name)
  {
    subscription_ = this->create_subscription<MsgType>(
      topic_name, 10,
      std::bind(&SensorSubscriberBase::handle_message, this, std::placeholders::_1));
  }

protected:
  virtual void handle_message(const typename MsgType::SharedPtr msg) = 0;

private:
  typename rclcpp::Subscription<MsgType>::SharedPtr subscription_;
};

#endif // SENSOR_SUBSCRIBER_BASE_HPP
