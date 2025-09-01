// 1_추상클래스2.cpp
#include <iostream>

// 아래 코드는 "OCP"를 만족할 수 없습니다.
// => 새로운 카메라 추가시
// => People 클래스는 수정되어야 한다.

// => 해결은 다음 소스

class Camera {
 public:
  void take() { std::cout << "Take Picture" << std::endl; }
};
class HDCamera {
 public:
  void take() { std::cout << "Take HD Picture" << std::endl; }
};

class People {
 public:
  // 아래 코드는 좋은 코드가 아니다. OCP를 만족할 수 없다.
  // 새로운 카메라가 나올때 마다, People code를 수정해야 한다.
  void use_camera(Camera* p) { p->take(); }
  void use_camera(HDCamera* p) { p->take(); }
};

int main() {
  // 사람이 태어남
  People p;
  // 카메라 삼
  Camera c;
  // 사람이 카메라를 사용해서 사진을 찍음
  p.use_camera(&c);

  HDCamera hc;
  p.use_camera(&hc);
}
