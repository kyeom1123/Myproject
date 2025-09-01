#include <iostream>
#include <string>

#include "getch.h"

// 핵심 : validation 정책은 변경 가능해야 합니다.
// => Edit는 라이브러리이므로 Edit 자체 수정은 없어야 한다.

// 방법 #1. 변하는 것을 가상함수로, 변하지 않는 것은 부모 class로 한다.
//

class Edit {
  std::string data;

 public:
  // 변해야 하는 부분(validation 정책)을 가상함수로 분리
  virtual bool validate(char c) { return true; }

  std::string get_data() {
    // std::cin >> data;
    // validation을 입력 후 하지말고 입력 전 하자
    // 숫자만 입력 되도록 강제할 수 있다.
    data.clear();
    while (1) {
      char c = _getch();

      if (c == 13) break;

      if (validate(c))  // validation 정책은 가상함수에 의존
      {
        data.push_back(c);
        std::cout << c;
      }
    }
    std::cout << '\n';

    return data;
  }
};

// Edit의 Validation 정책을 변경하려면
// 파생 클래스를 만들어서 약속된 가상함수를 override하면 됩니다.
// template method design pattern

class NumEdit : public Edit {
 public:
  bool validate(char c) override { return isdigit(c); }
}

int main() {
  // Edit e;
  NumEdit e;
  while (1) {
    std::cout << e.get_data() << std::endl;
  }
}
