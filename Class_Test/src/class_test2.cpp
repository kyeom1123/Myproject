#include <iostream>
#include <class_test2.h>
using namespace std;



int main() {
    shared_ptr<Animal> a = make_shared<Dog>("진돗개");
    a->introduce();

    cout << "use_count = " << a.use_count() << endl;
} // 스마트 포인터 범위 종료 → Dog → Animal 소멸자 자동 호출
