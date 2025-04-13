// terminal build command : g++ -std=c++17 -o ev_simulator ev_test.cpp

#include <iostream>
#include <string>
#include <algorithm>

class Vehicle{
    protected:
        std::string name;
    
    public:
        Vehicle(const std::string& n) : name(n) {}
        virtual void start()  {
            std::cout << "Vehicle" << name << "started" << std::endl;
        };
        virtual void charge(int) {}
        
};

class ElectricVehicle : public Vehicle{
    protected:
        int batteryLevel;
    public:
        ElectricVehicle(const std::string& n, int battery) : Vehicle(n), batteryLevel(battery){}

        void start() override {
            if(batteryLevel > 10){
                std::cout << "EV" << name << " started with battery" << batteryLevel <<std::endl;
            }
            else{
                std::cout << "EV" << name << " cannot start. Battery too low" << std::endl;
            }
        }

        void charge(int amount) override{
            batteryLevel = std::min(batteryLevel + amount, 100);
        }
};

int main(){
    Vehicle* a = new ElectricVehicle("IONIQ", 5);
    a->start();
    a->charge(30);
    a->start();

    return 0;
}