// QA1.cpp
#include <iostream>

struct A {int a;};
struct B {int b;};

// 핵심 : 다중 상속
struct X : public A, public B{

    int x;
};

int main()
{
    X x;
    A* pa = &x;
    // B* pb = &x;
    // B* pb = (B*)&x; // 1000 + sizeof(A) => 1004
    // B* pb = static_cast<B*>(&x);

    // 핵심
    // static_cast<B*>(&x) : &x에서 B의 모양을 찾아라
    // 반환 값은 &x가 아닌 B의 모양이 있는 주고
    // B가 없다면, 관련성 없으므로 에러

    // reinterpret_cast<B*>(&x) : &x 주소를 무조건 B타입으로 해석하겠다는 의도
    // 연관성이 없어도 됩니다. 항상 &x 주소 반환.
    B* pb = reinterpret_cast<B*>(&x); // 1000

    pb->b = 10; // x 객체의 어느 멤버가 변경될까요?
                // x.a에 10 대입!


    printf("%p\n", &x); // 1000
    printf("%p\n", pa); // &x와 동일
    printf("%p\n", pb); // 1000 + sizeof(A) => 1004


}