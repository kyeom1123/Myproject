#include <iostream>
#include <string>

#include "getch.h"

// 변하는 것을 분리하는 2가지 기술
// #1. 변하는 것을 가상함수로 => Template method
// #2. 변하는 것을 다른 클래스로 => startegy 패턴

// Validation 만 수행하는 클래스
// 교체 가능한 설계를 위해서 인터페이스 필요
struct IValidator {
  virtual bool validate(const std::string& s, char c) = 0;
  virtual bool is_complete(const std::string& s) { return true; }

  virtual ~IValidator() {}
};

// 주민 등록 번호 : 831 1   확인

class LimitDigitValidator : public IValidator {
  int value;

 public:
  LimitDigitValidator(int n) : value(n) {}
  bool validate(const std::string& s, char c) override {
    return s.size() < value && isdigit(c);
  }

  bool is_complete(const std::string& s) override { return s.size() == value; }
};

// --------------------------------------------------------
// DIP : Edit는 항상 추상(IValidator)에 의존해야 한다.

class Edit {
  std::string data;

  IValidator* val = nullptr;

 public:
  void set_validator(IValidator* p) { val = p; }
  std::string get_data() {
    // std::cin >> data;

    data.clear();
    while (1) {
      char c = _getch();

      if (c == 13 && (val == nullptr || val->is_complete(data))) break;

      if (val == nullptr ||
          val->validate(data, c))  // 유효성 확인은 다른 객체에 위임
      {
        data.push_back(c);
        std::cout << c;
      }
    }
    std::cout << '\n';

    return data;
  }
};

int main() {
  Edit e;
  LimitDigitValidator v(5);
  e.set_validator(&v);  // edit에 validation 정책 연결

  LimitDigitValidator v2(15);
  e.set_validator(&v2);  // validation 정책 변경
  while (1) {
    std::cout << e.get_data() << std::endl;
  }
}
