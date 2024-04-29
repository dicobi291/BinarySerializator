#include <iostream>

#include "../include/serializator.h"

struct UserData : public binary_serializator::ISerialize
{
	int m_iData;
	double m_dData;
	float m_fData;

	UserData(
		const int i = 0,
		const double d = 0.0,
		const float f = 0.0f
	) :
		m_iData(i),
		m_dData(d),
		m_fData(f) {}

	void serialize(std::vector<std::byte> &buf) override
	{
		buf.resize(buf.size() + 1);
		buf[buf.size() - 1] = static_cast<std::byte>(binary_serializator::DataType::USER_DATA);
		binary_serializator::serialize(buf, m_iData);
		binary_serializator::serialize(buf, m_dData);
		binary_serializator::serialize(buf, m_fData);
	}

	std::size_t deserialize(const std::vector<std::byte> &buf, std::size_t offset) override
	{
		offset = binary_serializator::deserialize(buf, offset + 1, m_iData);
		offset = binary_serializator::deserialize(buf, offset + 1, m_dData);
		offset = binary_serializator::deserialize(buf, offset + 1, m_fData);

		return offset;
	}
};


int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	UserData userData {5, 875.54, 936.4f};

	int iVal = 156;
	serialize(buff, iVal);
	serialize(buff, 234.86f);
	serialize(buff, 568324.86);
	userData.serialize(buff);
	serialize(buff, 2356u);

	auto dataType {DataType::UNKNOWN};
	for (std::size_t offset = 0; offset < buff.size();) {
		dataType = static_cast<DataType>(buff[offset]);
		switch (dataType) {
			case DataType::INT:
			{
				int val {0};
				offset = deserialize(buff, offset + 1, val);
				std::cout << "Deserialized int val: " << val << std::endl;
				break;
			}
			case DataType::UINT:
			{
				unsigned int val {0};
				offset = deserialize(buff, offset + 1, val);
				std::cout << "Deserialized unsigned int val: " << val << std::endl;
				break;
			}
			case DataType::FLOAT:
			{
				float val {0};
				offset = deserialize(buff, offset + 1, val);
				std::cout << "Deserialized float val: " << val << std::endl;
				break;
			}
			case DataType::DOUBLE:
			{
				double val {0};
				offset = deserialize(buff, offset + 1, val);
				std::cout << "Deserialized double val: " << std::fixed << val << std::endl;
				break;
			}
			case DataType::USER_DATA:
			{
				UserData _userData;
				offset = _userData.deserialize(buff, offset + 1);
				std::cout << "Deserialized UserData: " << _userData.m_iData << ", " << _userData.m_dData << ", " << _userData.m_fData << std::endl;
				break;
			}
			default:
				break;
		}
	}

	return 0;
}