#include <iostream>
#include <vector>

// 파워 포인트 같은 프로그램을 객체지향으로 설계해 봅시다.
// #1. 각 도형을 타입으로 설계한다. 

class Rect{
//    int x,y,w,h; 와 생성자는 생략
public:
    void draw() {std::cout << "draw rect\n";}
};

class Circle{
    //    int x,y,w,h; 와 생성자는 생략
public:
    void draw() {std::cout << "draw circle\n";}
};

int main()
{

}

