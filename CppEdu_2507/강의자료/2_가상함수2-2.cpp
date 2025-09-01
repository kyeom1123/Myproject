#include <iostream>

// 결국 문제는
// #1. Base* p = new Derived; delete p; 코드가 았고
// #2. Derived 소명자에 어떤 코드가 있을때만, 문제가 됩니다.
// => 위 상황이 아니면 문제 없음.
class Base {
 public:
  // ~Base() {}
 protected:
  ~Base() {}
};
class Derived : public Base {
 public:
  Derived() { std::cout << "Derived() 자원할당" << std::endl; }
  ~Derived() { std::cout << "~Derived() 자원해지" << std::endl; }
};
int main() {
  // 문제 없음
  // Derived d; c

  // 아래 코드도 문제 없음
  // Derived* p = new Derived;
  // delete p;

  // 자원 할당 후, 자원 해지를 하지 않는다.
  Base* p = new Derived;
  // delete p; // 컴파일 에러. protected 소멸자 의도는
  // 		  // Base* 상태로 delete 하지 말라는 것

  delete static_cast<Derived*>(p);  // Ok
}

// 구글 "C++ core guideline" 검색
// => C++ 창시자와 표준위원회 위원장이 만드는 가이드 라인
// => MISRA C++, Autosar C++ 도 결국 위 내용을 참고
// 1번째 링크로 이동