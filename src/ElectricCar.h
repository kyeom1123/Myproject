#ifndef ELECTRICCAR_H
#define ELECTRICCAR_H

#include "Vehicle.h"

class ElectricCar : public Vehicle {
private:
    int batteryLevel;

public:
    ElectricCar(std::string b, int s, int battery);
    ~ElectricCar();

    void chargeBattery(int amount);
};

#endif // ELECTRICCAR_H
