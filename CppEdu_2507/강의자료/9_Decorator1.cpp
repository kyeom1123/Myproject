#include <iostream>

// 인터넷에서 그림을 다운로드해서 화면에 그리는 클래스
class Image
{
public:
	void draw()	{ std::cout << "draw image\n";}
};

int main()
{
	Image img;
	img.draw();
}
