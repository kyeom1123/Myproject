// upcasting3.cpp
class Animal 
{
public: 
	int age;
};
class Cat : public Animal 
{
};
class Dog : public Animal
{
public:
	int color;
};

void NewYear(Dog* pDog)  // Dog 객체만 받을 수 있다. 
{
	++(p->age); 
	
}
int main()
{
	Animal a; NewYear(&a);
	Dog    d; NewYear(&d);
}











