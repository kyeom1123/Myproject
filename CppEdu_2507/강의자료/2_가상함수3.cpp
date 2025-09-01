// 2_가상함수재정의
class Base {
 public:
  virtual void foo() {}
  virtual void goo(int) {}
};
class Derived : public Base {
 public:
  // #1. 가상함수 override 시에는 "virtual"은 있어도 되고 없어도 된다.
  // virtual void foo() {}
  virtual void foo() {}

  // #2. 가상함수 OVERRIDE 시 실수(오타)가 있어도
  // => 에러가 아닙니다.
  // => 컴파일러는 다른 함수를 만든 것으로 생각

  // #3. 위 문제를 해결책으로 c++11에서 override 키워드 도입
  // virtual void fooo() {}
  // virtual void goo(double){}

  // virtual void fooo() override {}			// error
  // virtual void goo(double) override {}	// error

  virtual void foo() override {}
};

int main() {}
