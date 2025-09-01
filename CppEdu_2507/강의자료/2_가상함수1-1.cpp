#include <iostream>

class Animal {
 public:
  void cry1() { std::cout << "Animal Cry1" << std::endl; }
  virtual void cry2() { std::cout << "Animal Cry2" << std::endl; }
};
class Dog : public Animal {
 public:
  // override : 기반 클래스 멤버 함수를 파생클래스가 다시 만드는 것
  void cry1() { std::cout << "Dog Cry1" << std::endl; }
  virtual void cry2() { std::cout << "Dog Cry2" << std::endl; }
};
int main() {
  Dog d;
  Animal* p = &d;

  // 핵심
  // p 타입 : Animal*
  // p가 가리키는 타입 : Dog

  p->cry1();  // 1. 클래스 선언을 보고 가상함수인지 아닌지 조사
              // 2. 가상이 아니므로 포인터 타입으로 호출 결정
  // Animal cry1

  p->cry2();  // 가상이므로 dynamic binding
              // Dog cry2 호출
}

// p->cry() 를 어떤 함수와 연결할 것인가?
// => 함수 바인딩(funciton binding) 이라고 합니다.
// => 2가지 방법이 있다.

// 1. static binding : 컴파일러가 함수를 결정
// 핵심 : 컴파일러는 컴파일 시간에 p가 가리키는 객체 종류는 알 수 없다.
// 		 알고 있는 것은 p자체의 타입이 Animal* 라는 것, 따라서
// 컴파일러가 결정하면 Animal cry 호출 논리적이지 않지만, 빠르다 C++ 기본 바인딩

// 2. dynamic binding : 컴파일 시간에는 p가 가리키는 메모리를 조사하는 기계어
// 생성 						실행시 메모리 조사 결과에 따라 홤수 결정 						p 가 가리키는 곳에 Dog가
// 있었다면, Dog cry 호출 논리적인데 느리다. Java, Python, swift, kotlin 등의
// 대부분의객체지향 언어 방식 C++, C#의 가상함수
