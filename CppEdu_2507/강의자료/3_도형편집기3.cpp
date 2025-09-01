#include <iostream>
#include <vector>

// 파워 포인트 같은 프로그램을 객체지향으로 설계해 봅시다.
// #1. 각 도형을 타입으로 설계한다. 

// #2. 기반 클래스가 있다면
// => 모든 도형의 공통의 특징을 한곳데서 관리할 수 있고 
// => 모든 도형의 객체를 하나의 컨테이너에 보관 가능하다. 

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

int main()
{
    std::vector<Shape*> v;  

    while( true){
        int cmd;
        std::cin >> cmd;

        if(cmd ==1) v.push_back( new Rect);
        else if(cmd ==2) v.push_back( new Circle);
        else if(cmd ==9) {

            for( auto s: v)
                s->draw();  // error 발생
                            // 원인 : s는 Shape* 인데, Shape에는 draw가 없다. 
                            // 해결은 다음 소스에서 
        }


    }
}

