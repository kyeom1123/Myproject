// 1_추상클래스1.cpp
// 추상 클래스 : 순수가상함수가 한개 이상 있는 클래스
// 특징 : 객체를 생성할 수 없다.
// 의도 : 파생 클래스 제작자에게 약속된 함수를 반드시 만들라고 시키는 것
// 함수 이름의 약속

class Shape {
 public:
  virtual void draw() = 0;  // 순수 가상함수
                            // 구현이 없고 =0 으로 끝나는 가상함수
};
class Rect : public Shape {
  // draw()의 구현을 제공하지 않으면 Rect도 추상
  // draw()의 구현을 제공하면 추상 아님 (구체, concrete)
 public:
  void draw() {}
};
int main() {
  // Shape s; // error
  Shape* p;
  Rect r;
}
