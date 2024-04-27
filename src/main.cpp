#include <iostream>

#include "../include/serializator.h"

int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	int i = 156;
	serialize(buff, i);
	serialize(buff, 234.86f);
	serialize(buff, 568324.86);
	serialize(buff, 2356u);

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
			case DataType::UINT:
			{
				unsigned int val {0};
				deserialize(buff.data() + i + 1, val);
				i += sizeof(val) + 1;
				std::cout << "Deserialized unsigned int val: " << val << std::endl;
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
			case DataType::DOUBLE:
			{
				double val {0};
				deserialize(buff.data() + i + 1, val);
				i += sizeof(val) + 1;
				std::cout << "Deserialized double val: " << val << std::endl;
				break;
			}
			default:
				break;
		}
	}

	return 0;
}