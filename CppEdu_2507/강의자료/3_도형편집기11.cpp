#include <iostream>
#include <vector>

// 코드 정리와 생각해 볼 문제

class not_implemented {};
class Shape {
  int color = 0;

 public:
  virtual ~Shape(){};
  void set_color(int c) { color = c; }

  void draw() {
    std::cout << "mutex.lock\n";  // 안변함
    draw_imp();
    std::cout << "mutex.unlock\n";  // 안변함
  }

  // 가상함수 : override 하지 않으면, 기본 구현 제공할 것이라는 의도
  // 순수 가상함수 : 반드시 만들라는 의미

  // "Shapre"는 추상적 개념
  // => "draw_imp"의 기본 구현을 제공할 수는 없다.
 protected:
  virtual void draw_imp() = 0;

 public:
  // clone은
  // #1. =0으로 해도 좋습니다. 엄격하고 안전한 코드
  // => 하지만, 순수가상함수가 너무 많으면 파생클래스가 해야할 작업이
  // 많아집니다.
  // => clone()이 자주 사용되는 함수가 아니면 2번도 가능
  // virtual Shape* clone() = 0;

  // #2. 기본 구현에서 예외 발생
  // => clone을 override 하지 않고 사용하지 않으며 => ok
  // => clone을 override 하지 않고 사용하면 => 예외 발생
  // => clone을 override하고 사용하면 => ok

  // 의도 : 필요하면 만들라는 것
  // => C#, Python에서 많이 사용
  // => 안전성이 부족하다.
  virtual Shape* clone() {
    // return new Shape(*this);  // 자신 (*this)를 복사해서 생성 }
    throw not_implemented();
  };

  // #1, #2도 좋은데, c 스타일의 기본 구현은 잘못된 값 반환도 역시 널리
  // 사용됩니다. 도움말 : get_area() 호출시 0이 나오면 면적구할수 없음 이라고
  // 약속
  virtual int get_area() { return 0; }
};

class Rect : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw_imp() override { std::cout << "draw rect\n"; }

 public:
  // 참고 : 가상함수 override 시 반환 타입 변경 가능합니다. (단,
  // 상속관계타입만)
  virtual Rect* clone() override {
    return new Rect(*this);  // 자신 (*this)를 복사해서 생성 }
  };
};

class Circle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw_imp() override { std::cout << "draw circle\n"; }

 public:
  virtual Circle* clone() override {
    return new Circle(*this);  // 자신 (*this)를 복사해서 생성 }
  };
};

class Triangle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw_imp() override { std::cout << "draw triangle\n"; }

 public:
  virtual Triangle* clone() override {
    return new Triangle(*this);  // 자신 (*this)를 복사해서 생성 }
  };
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

      v.push_back(v[k]->clone());
    }
  }
}
