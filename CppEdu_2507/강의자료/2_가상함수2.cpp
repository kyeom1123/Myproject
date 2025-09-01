#include <iostream>

// 핵심 : 반드시 알아야 한느 중요 문법
// => 업무에서 상속문법을 사용한다면
// => 기반 클래스의 소멸자는 "반드시 가상함수"로 작성해야 합니다.
// => 단, 이경우 아주 약간의 오버헤드 (가상함수테이블, 실행시간 호출에 따른
// 성능지연 )
class Base {
 public:
  ~Base() {}
};
class Derived : public Base {
 public:
  Derived() { std::cout << "Derived() 자원할당" << std::endl; }
  ~Derived() { std::cout << "~Derived() 자원해지" << std::endl; }
};
int main() {
  // 문제 없음
  // Derived d;

  // 아래 코드도 문제 없음
  // Derived* p = new Derived;
  // delete p;

  // 자원 할당 후, 자원 해지를 하지 않는다.
  Base* p = new Derived;
  delete p;  // 이 순간 소멸자를 호출해야 한다.

  // 소멸자 호출도 결국 함수 호출이다.
  // p->소멸자()가 되어야 한다.
  // #1. static binding 할지 dynamic 할지 결정해야 한다.
  // #2. 컴파일러는 p가 Base* 라는 것만 알고 있으므로, Base 클래스에서 소멸자가
  // 가상인지만 조사 (Derived 조사가 아님) 가상이 아니면 : Static binding,
  // ~Base() 호출 가상이면,    : dynamic binding, ~Derived() 호출
}
