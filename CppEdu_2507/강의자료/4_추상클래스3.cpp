// 1_추상클래스3.cpp
#include <iostream>

// 예시 노트북에 여러가지 USB를 사용할 수 있다.
// USB 제품이 회사마다 다르고 미래 새로운 버전이 나와도 사용이 가능해야 한다.
// Protocol 규칙을 지키면 가능하다.

// 카메라를 무조건 만들지 말고
// 카메라 제작자가 지켜야 하는 규칙을 만들자

// 규칙 : 모든 카메라는 아래 클래스로 부터 파생되어야 한다.
class ICamera {
 public:
  virtual void take() = 0;
};

// 이제 모든 사용자는 규칙대로만 사용해야 합니다.
// 실제 카메라가 없어도 "규칙"이 있으므로 코드 작성이 가능합니다.

class People {
 public:
  // 추상클래스라도 포인터 변수는 사용 가능하다.
  void use_camera(ICamera* c) { c->take(); }
};

// 모든 카메라는 규칙을 지켜야 한다.

class Camera : public ICamera {
 public:
  void take() { std::cout << "Take Picture" << std::endl; }
};
class HDCamera : public ICamera {
 public:
  void take() { std::cout << "Take HD Picture" << std::endl; }
};
class UHDCamera : public ICamera {
 public:
  void take() { std::cout << "Take UHD Picture" << std::endl; }
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

  UHDCamera uhc;
  p.use_camera(&uhc);
}
