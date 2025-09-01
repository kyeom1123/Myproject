// upcasting2.cpp
#include <vector>

class Animal {};
class Dog : public Animal {};

int main()
{
	
	std::vector<Dog*>    v1;	// Dog 객체만 보관
	std::vector<Animal*> v2;	// 모든 동물(Animal 파생 클래스)를 보관하겠다는 의도

	// upcasting 활용 #1. 동종을 보관하는 컨테이너 만들기
	// (동일 기반 클래스로 부터 파생된 클래스)
	
}
