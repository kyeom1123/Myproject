#include <iostream>
#include <type_traits>

template<typename T>
concept pointer = std::is_pointer_v<T>;

template<typename T>
concept not_pointer = !std::is_pointer_v<T>;

void foo( pointer auto p)
{
	std::cout << "pointer version\n";
}
void foo( not_pointer auto p)
{
	std::cout << "not_pointer version\n";
}

int main()
{
	int n = 0;
	foo(n);
	foo(&n);
}