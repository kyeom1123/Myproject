#include "ElectricCar.h"

int main() {
    ElectricCar tesla("Tesla", 120, 80);

    tesla.showInfo();
    tesla.chargeBattery(10);

    return 0;
}
