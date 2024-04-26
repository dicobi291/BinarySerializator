#include <iostream>

#include "../include/serializator.h"

int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	serialize(buff, DataType::INT, 156);
	serialize(buff, DataType::FLOAT, 234.86);
	serialize(buff, DataType::INT, 2356);

	return 0;
}