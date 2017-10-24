#include <iostream>

#include "DiscreteDistribution.hpp"
#include "IterableDiscreteDistribution.hpp"

int main()
{
	DiscreteDistribution<char> myDistn{};

	myDistn.add('R', 71);
	myDistn.add('J', 43);
	myDistn.add('X', 24);

	std::cout << myDistn.toString() << "\n\n";

	std::cout << "----------- Now working with IDD -------------\n\n";

	IterableDiscreteDistribution<char> aDistn{};

	aDistn.add('R', 71);
	aDistn.add('J', 43);
	aDistn.add('X', 24);

	std::cout << aDistn.toString() << "\n\n";

	IterableDiscreteDistribution<char>::Iterator myIterator{aDistn.begin()};

	system("pause");

	return 0;
}