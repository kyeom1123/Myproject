#include <iostream>
#include <vector>

// #7.

class Shape {
  int color = 0;

 public:
  virtual ~Shape(){};
  void set_color(int c) { color = c; }

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
    else if (cmd == 3)
      v.push_back(new Triangle);
    else if (cmd == 9) {
      for (auto s : v) {
        s->draw();
      }
    } else if (cmd = 8) {
      std::cout << "몇번째 도형을 복제할까요? >> ";

      int k;
      std::cin >> k;

      // k 번째 만든 도형을 복사해서 한 개 더 만든 후 v에 추가
      // => 어떻게 구현해야할까?
      // => k번째 만든 도형이 어떤 도형일까?

      // 방법#1. 조사 하자
      // => OCP를 만족할 수 없는 안 좋은 디자인이다.
      // => 해결책은 다음 소스에서
      if (Rect* r = dynamic_cast<Rect*>(v[k]); r != nullptr) {
      }
    }
  }
}
