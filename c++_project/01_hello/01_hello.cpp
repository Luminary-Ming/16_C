#include <iostream>

int main(void)
{
	auto add = [](int a, int b) -> int { return a + b; };

	std::cout << add(22, 33) << std::endl;



	return 0;
}