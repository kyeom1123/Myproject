#include <iostream>

class Animal
{
public:
	int age;
	virtual ~Animal(){} // 이제 Animal에는 가상 함수가 있고 
						// Animal의 모든 파생클래스 객체는 가상함수 테이블 생성
						// 테이블에 타입정보 추가
};
class Dog : public Animal
{
public:
	int color;
};
// 7 page
int main()
{
	Dog d;
	Animal a;

	Animal* p3 = &a;

	// static_cast : 컴파일 시간 캐스팅
	// 컴파일러는 컴파일 시간에 p3가 가리키는 곳이 어떤 객체인지 알 수 없다. 
	// 그래서 아래 코드는 항상 성공, 주소가 나옵니다. 
	// 만약 실제 객체가 Dog가 아니었다면, 버그 "사용자 책임"
	Dog* pdog = static_cast<Dog*>(p3);
	std::cout << pdog << std::endl;

	// dynamic_cast : 실시간 캐스팅
	// 		실행시간에 p3가 가리키는 곳이 Dog 라면 캐스팅 허용
	// Dog가 아닌 경우 0 반환


}











