// terminal build command : g++ -std=c++17 -o ptr_simulator ptr_test.cpp

#include <iostream>
#include <string>
#include <memory>

class Vehicle{
    protected:
        std::string name;
    public:
        Vehicle(const std::string& n) : name(n){}
        void start(){
            std::cout << "Vehicle " << name << " started" << std::endl;
        }
        ~Vehicle(){
            std::cout << "Vehicle " << name << " destroyed" << std::endl;
        }
    };

int main(){
    std::unique_ptr<Vehicle> a = std::make_unique<Vehicle>("IONIQ");
    a->start();
    std::shared_ptr<Vehicle> b = std::make_shared<Vehicle>("Tesla");
    std::shared_ptr<Vehicle> c = b;
    b->start();
    // c->start();

}