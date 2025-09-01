#include <iostream>
#include <vector>

// #6. 다형성 (polymorphism) 개념


class Shape{

    int color = 0;

public:
    virtual ~Shape(){};

    // 아래 함수는 virtual로 할까요?
    // => 파생 클래스가 override할 필요가 없습니다. 
    // => non-virtual 
    void set_color(int c) {color = c;}

    virtual int get_area() {return 0;}
    virtual void draw() {std::cout << "draw shape\n";}
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
        else if(cmd ==3) v.push_back( new Triangle);
        else if(cmd ==9) {
            for (auto s : v) {
                s->draw();  // 다형성 (polymorphsm)
                            // => 동일한 코드가 상황(실제 객체)에 따라
                            // => 다르게 동작하는 것
                    // 객체지향 프로그램의 3대 요소
                    // => 캡슐화, 상속, 다형성
        }
        }
    }
}

