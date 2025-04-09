#include <iostream>
#include <memory>
#include <string>
using namespace std;

// 부모 클래스
class Animal {
    protected:
        string name;
    
    public:
        Animal(const string& n) : name(n) {
            cout << "Animal 생성자" << endl;
        }
    
        virtual ~Animal() {
            cout << "Animal 소멸자" << endl;
        }
    
        virtual void introduce() {
            cout << "나는 동물입니다." << endl;
        }
    };
    
    // 자식 클래스
    class Dog : public Animal {
    public:
        Dog(const string& n) : Animal(n) {
            cout << "Dog 생성자" << endl;
        }
    
        ~Dog() {
            cout << "Dog 소멸자" << endl;
        }
    
        void introduce() override {
            cout << "나는 강아지 " << name << "입니다!" << endl;
        }
    };