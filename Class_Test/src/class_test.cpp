#include <class_test.h>


int main() {
    Animal* a1 = new Dog();
    Animal* a2 = new Cat();

    a1->eat();     // 공통 동작
    a1->sound();   // Dog 버전
    a2->sound();   // Cat 버전
}
