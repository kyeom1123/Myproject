#include <iostream>
#include <string>

#include "getch.h"

class Edit {
  std::string data;

 public:
  std::string get_data() {
    // std::cin >> data;

    data.clear();
    while (1) {
      char c = _getch();

      if (c == 13) break;

      if (isdigit(c)) {
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
  while (1) {
    std::cout << e.get_data() << std::endl;
  }
}
