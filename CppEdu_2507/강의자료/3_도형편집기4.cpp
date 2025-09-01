#include <iostream>
#include <vector>

// #3-1. 해결책 #1. 캐스팅
// => 아래 코드는 에러 없이 예상대로 동작합니다. 

// 그런데!!!
// => 새로운 도형 Triangle이 추가 되면 
// => main 함수가 변경(if 문 추가) 되어야 합니다. 

// OCP 이론
// => 기능 확장에 열려 있고 (Open, 나중에 코드가 추가되어도)
// => 코드수정에 닫혀 있어야 한다. (Close, 기존 코드가 수정되지 않게)
// => 하라는 이론 (Principle)

// Open-Close Principle
// 아래 코드는 OCP 위반
class Shape{

    int color = 0;

public:
    virtual ~Shape(){};
};

class Rect : public Shape {
//    int x,y,w,h; 와 생성자는 생략
public:
    void draw() {std::cout << "draw rect\n";}
};

class Circle : public Shape{
    //    int x,y,w,h; 와 생성자는 생략
public:
    void draw() {std::cout << "draw circle\n";}
};

class Triangle : public Shape{
    //    int x,y,w,h; 와 생성자는 생략
public:
    void draw() {std::cout << "draw triangle\n";}
};

int main()
{
    std::vector<Shape*> v;  

    while( true){
        int cmd;
        std::cin >> cmd;

        if(cmd ==1) v.push_back( new Rect);
        else if(cmd ==2) v.push_back( new Circle);
        else if(cmd ==9) {

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
                if ( Rect* r = dynamic_cast<Rect*>(s); r != nullptr)
                {
                    r->draw();
                }
                else if ( Circle* c = dynamic_cast<Circle*>(s); c != nullptr){

                    c->draw();
               }   
        }
        }
    }
}

