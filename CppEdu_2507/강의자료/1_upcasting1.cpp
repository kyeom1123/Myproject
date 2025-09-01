class Animal
{
public:
	int age;
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
	Dog* p1 = &d; 		// Ok
	// int* p2 = &d; 	// error
	Animal* p3 = &d; 	// Ok

	// 핵심 #2. 컴파일러가 p3가 실제 가리키는 객채가 어떤 종류인지 알 수 없다. 
	// => 오직, p3 자체의 타입만 알 수 있다. 
	// => 아래 코드를 생각해 보세요
	// if (사용자 입력 == 1) p3 = new Animal;

	// 핵심 #3. 기반 클래스 포인터로는 파생 클래스가 추가한 멤버에 접근할 수 없다. 
	// => p3로는 Animal 멤버만 접근 가능하다.
	// => 이유 : 컴파일러가 p3가 가리키는 곳이 Dog인지 확신할 수 없다?

	// static type check : 컴파일 시간에 타입이 맞는지 확인
	// => C++, C#, Java
	// dynamic type check : 실행 시간에 타입이 맞는지 확인
	// => Python

	p3->age	= 10; 	// Ok
	// p3->color = 10;	// error

	// #4. p3가 가리키느 ㄴ곳에 Dog가 있다는 확신이 있다면, 
	// => 개발자가 Dog* 타입으로 캐스팅해서 접근하면 됩니다. 

	Dog* pdog = static_cast<Dog*>(p3);
	pdog->color = 10; // Ok

}











