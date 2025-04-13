#ifndef SENSOR_BASE_HPP
#define SENSOR_BASE_HPP

class SensorBase
{
public:
  virtual ~SensorBase() = default;
  virtual float read_temperature() = 0;
};

#endif // SENSOR_BASE_HPP
