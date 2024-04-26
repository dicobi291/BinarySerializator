#include <iostream>

#include "../include/serializator.h"

int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	serialize(buff, DataType::INT, 156);
	serialize(buff, DataType::FLOAT, 234.86f);
	serialize(buff, DataType::INT, 2356);

	auto dataType {DataType::UNKNOWN};
	for (int i = 0; i < buff.size();) {
		dataType = static_cast<DataType>(buff[i]);
		switch (dataType) {
			case DataType::INT:
			{
				int val {0};
				deserialize(buff.data() + i + 1, val);
				i += sizeof(val) + 1;
				std::cout << "Deserialized int val: " << val << std::endl;
				break;
			}
			case DataType::FLOAT:
			{
				float val {0};
				deserialize(buff.data() + i + 1, val);
				i += sizeof(val) + 1;
				std::cout << "Deserialized float val: " << val << std::endl;
				break;
			}
			default:
				break;
		}
	}

	return 0;
}