#include <iostream>
#include <vector>

// 10. template method 패턴
// => 변하지 않은 알고리즘을 담은 메소드를 기반 클래스가 제공하고
// => 변해야 하는 부분만 가상함수로 분리해서 파생 클래스가 재정의할 수 있게
// 한다.
// => 가장 널리 사용한다.

class Shape {
  int color = 0;

 public:
  virtual ~Shape(){};
  void set_color(int c) { color = c; }

  virtual int get_area() { return 0; }
  // 코드에 변하지 않는 코드 안에, 변하는 코드가 있으면 분리해야 한다.
  // 핵심 : 공통성과 가변성의 분리
  // => 변하지 않는 코드 내부에 숨은 변하는 코드는 분리 되어야 한다.
  // => 변하는 것을 가상함수로 분리한다.
 protected:
  virtual void draw_imp() {
    std::cout << "draw shape\n";  // 변함
  }

 public:
  // 자주 쓰는 방법은 이름이 있다. Template method
  void draw() {
    std::cout << "mutex.lock\n";  // 안변함
    draw_imp();
    std::cout << "mutex.unlock\n";  // 안변함
  }

  // 자신의 복사본을 생성하는 가상함수는 아주 유용하게 사용됩니다.
  virtual Shape* clone() {
    return new Shape(*this);  // 자신 (*this)를 복사해서 생성 }
  };
};

class Rect : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw_imp() { std::cout << "draw rect\n"; }

 public:
  // 참고 : 가상함수 override 시 반환 타입 변경 가능합니다. (단,
  // 상속관계타입만)
  virtual Rect* clone() {
    return new Rect(*this);  // 자신 (*this)를 복사해서 생성 }
  };
};

class Circle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw_imp() { std::cout << "draw circle\n"; }

 public:
  virtual Circle* clone() {
    return new Circle(*this);  // 자신 (*this)를 복사해서 생성 }
  };
};

class Triangle : public Shape {
  //    int x,y,w,h; 와 생성자는 생략
 protected:
  void draw() { std::cout << "draw triangle\n"; }

 public:
  virtual Triangle* clone() {
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

      // 이제 조사할 필요 없이 요청하면 됩니다.
      v.push_back(v[k]->clone());
    }
  }
}
