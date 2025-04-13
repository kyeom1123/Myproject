#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/temperature.hpp>
#include "temperature_sensor.hpp"  // SensorBase와 TemperatureSensor 정의

using namespace std::chrono_literals;

// SensorPublisher 노드는 1초마다 온도 메시지를 퍼블리시함
class SensorPublisher : public rclcpp::Node
{
public:
  SensorPublisher()
  : Node("sensor_publisher"), 
    sensor_(std::make_shared<TemperatureSensor>()) // [3] 센서 객체 생성 (SensorBase를 상속한 TemperatureSensor)
  {
    // [4] "temperature" 토픽에 퍼블리셔 생성 (큐 사이즈 10)
    publisher_ = this->create_publisher<sensor_msgs::msg::Temperature>("temperature", 10);

    // [5] 1초 주기로 publish_temperature()을 호출하는 타이머 생성
    timer_ = this->create_wall_timer(1s, std::bind(&SensorPublisher::publish_temperature, this));
  }

private:
  // [6] 1초마다 호출되는 콜백 함수
  void publish_temperature()
  {
    auto msg = sensor_msgs::msg::Temperature();  // [6-1] 온도 메시지 객체 생성

    msg.temperature = sensor_->read_temperature(); // [6-2] 센서에서 온도 읽기
    msg.header.stamp = this->now();                // [6-3] 타임스탬프 추가
    msg.header.frame_id = "sensor_frame";          // [6-4] 프레임 ID 설정

    RCLCPP_INFO(this->get_logger(), "Publishing: %.2f °C", msg.temperature);  // [6-5] 콘솔 출력

    publisher_->publish(msg);  // [6-6] 퍼블리셔를 통해 메시지 전송
  }

  // 퍼블리셔, 타이머, 센서 포인터 멤버 변수들
  rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::shared_ptr<SensorBase> sensor_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);  // [1] ROS2 초기화

  // [2] SensorPublisher 노드를 생성하고 ROS 이벤트 루프 시작
  rclcpp::spin(std::make_shared<SensorPublisher>());

  rclcpp::shutdown();  // [7] 노드 종료 시 호출
  return 0;
}
