#include "ElectricCar.h"

ElectricCar::ElectricCar(std::string b, int s, int battery) : Vehicle(b, s) {
    batteryLevel = battery;
}

ElectricCar::~ElectricCar() {
    std::cout << brand << " 전기차가 삭제됨" << std::endl;
}

void ElectricCar::chargeBattery(int amount) {
    batteryLevel += amount;
    std::cout << brand << " 배터리 충전: " << batteryLevel << "%" << std::endl;
}
