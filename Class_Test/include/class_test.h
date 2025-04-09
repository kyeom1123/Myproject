#include <iostream>
#include <ostream>

using namespace std;

class Animal {
    public:
        void eat() {
            cout << "먹는다" << endl;
        }
    
        virtual void sound() {
            cout << "동물 소리" << endl;
        }
    };
    
    class Dog : public Animal {
    public:
        void sound() override {
            cout << "멍멍" << endl;
        }
    };
    
    class Cat : public Animal {
    public:
        void sound() override {
            cout << "야옹" << endl;
        }
    };
    