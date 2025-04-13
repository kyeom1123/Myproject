#ifndef TEMPERATURE_SENSOR_HPP
#define TEMPERATURE_SENSOR_HPP

#include "sensor_base.hpp"
#include <cstdlib>
#include <ctime>

class TemperatureSensor : public SensorBase
{
public:
  TemperatureSensor() {
    std::srand(std::time(nullptr));
  }

  float read_temperature() override {
    return 23.0f + static_cast<float>(std::rand() % 100) / 10.0f;
  }
};

#endif // TEMPERATURE_SENSOR_HPP
