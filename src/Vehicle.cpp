#include "Vehicle.h"

Vehicle::Vehicle(std::string b, int s) {
    brand = b;
    speed = s;
}

Vehicle::~Vehicle() {
    std::cout << brand << " 차량이 삭제됨" << std::endl;
}

void Vehicle::showInfo() {
    std::cout << "브랜드: " << brand << ", 속도: " << speed << " km/h" << std::endl;
}
