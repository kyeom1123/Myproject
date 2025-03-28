#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>

class Vehicle {
protected:
    std::string brand;
    int speed;

public:
    Vehicle(std::string b, int s);
    virtual ~Vehicle();

    void showInfo();
};

#endif // VEHICLE_H
