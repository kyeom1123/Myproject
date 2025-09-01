#include <iostream>
#include <vector>

// 해결책 #2 - 좋은 디자인
// #4. 모든 파생클래스(도형)의 공통의 특징은 기반 클래스(Shape)에도 있어야 한다.
// => 그래야 Shpae*로 해당 기능을 사용할 수 있다.

// #5. 기반 클래스 함수중 파생 클래스가 override 하게 되는 것은
//      반드시 virtual 이어야 한다.
// => Shape* s = new Rect;로 s->draw() 했을때 Rect를 그려야 한다.

class Shape {
  int color = 0;

 public:
  virtual ~Shape(){};

  // 아래 함수는 virtual로 할까요?
  // => 파생 클래스가 override할 필요가 없습니다.
  // => non-virtual
  void set_color(int c) { color = c; }

  // 아래 함수는 ?
  // => 모든 도형은 면적 구하는 방법이 다르므로
  // => override 필요
  // => virtual !!
  virtual int get_area() { return 0; }

  virtual void draw() { std::cout << "draw shape\n"; }
};

class Rect : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 public:
  void draw() { std::cout << "draw rect\n"; }
};

class Circle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 public:
  void draw() { std::cout << "draw circle\n"; }
};

class Triangle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 public:
  void draw() { std::cout << "draw triangle\n"; }
};

int main() {
  std::vector<Shape*> v;

  while (true) {
    int cmd;
    std::cin >> cmd;

    if (cmd == 1)
      v.push_back(new Rect);
    else if (cmd == 2)
      v.push_back(new Circle);
    else if (cmd == 9) {
      /*
      for( auto s: v){
          Rect* r;
          Circle* c;

          if(r = dynamic_cast<Rect*>(s) != nullptr)
          {
              r->draw();
          }
      }
      */
      // 위 코드는 C++17에서 나온 새로운 if 문을 사용하면 편리
      // if (초기화 구문; 조건식)
      for (auto s : v) {
        if (Rect* r = dynamic_cast<Rect*>(s); r != nullptr) {
          r->draw();
        } else if (Circle* c = dynamic_cast<Circle*>(s); c != nullptr) {
          c->draw();
        }
      }
    }
  }
}
