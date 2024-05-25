#include <iostream>
#include <algorithm>

#include "../include/serializator.h"
#include "../include/core.h"

struct UserData : public binary_serializator::ISerializable
{
	int m_iData;
	double m_dData;
	float m_fData;
	std::string m_strData;

	UserData(
		const int i = 0,
		const double d = 0.0,
		const float f = 0.0f,
		const std::string &str = ""
	) :
		m_iData(i),
		m_dData(d),
		m_fData(f),
		m_strData(str) {}

	std::vector<std::byte> serialize() override
	{
		std::vector<std::byte> buff;

		ISerializable::serializeData(buff, m_iData);
		ISerializable::serializeData(buff, m_dData);
		ISerializable::serializeData(buff, m_fData);
		ISerializable::serializeData(buff, m_strData);

		return buff;
	}

	void deserialize(const std::vector<std::byte> &buff) override
	{
		std::size_t offset = 0;
		
		offset = ISerializable::deserializeData(buff, offset, m_iData);
		offset = ISerializable::deserializeData(buff, offset, m_dData);
		offset = ISerializable::deserializeData(buff, offset, m_fData);
		offset = ISerializable::deserializeData(buff, offset, m_strData);
	}
};

int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	std::vector<int> iVector = {50, 464, 30574, 10'000'000};

	int iVal = 156;
	binary_serializator::core::serialize(buff, false);
	binary_serializator::core::serialize(buff, iVal);
	binary_serializator::core::serialize(buff, 234.86f);
	binary_serializator::core::serialize(buff, 568324.86);
	binary_serializator::core::serialize(buff, 2356u);
	binary_serializator::core::serialize(buff, iVector);
	
	UserData userData {5, 875.54, 936.4f, "string_data"};

	auto userBuff = userData.serialize();

	bool bDeserializeVal = false;
	int iDeserializeVal = 0;
	float fDeserializeVal = 0.0f;
	double dDeserializeVal = 0.0;
	unsigned int uDeserializeVal = 0;
	std::vector<int> iDeserializedVector;
	UserData deserializedUserData;
	
	std::size_t offset = 0;

	offset = binary_serializator::core::deserialize(buff, offset, bDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, iDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, fDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, dDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, uDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, iDeserializedVector);

	deserializedUserData.deserialize(userBuff);

	std::cout << "Deserialized bool val: " << bDeserializeVal << std::endl;
	std::cout << "Deserialized int val: " << iDeserializeVal << std::endl;
	std::cout << "Deserialized float val: " << fDeserializeVal << std::endl;
	std::cout << "Deserialized double val: " << std::fixed << dDeserializeVal << std::endl;
	std::cout << "Deserialized unsigned int val: " << uDeserializeVal << std::endl;
	std::cout << "Deserialized int vector: "; std::for_each(iDeserializedVector.cbegin(), iDeserializedVector.cend(), [&iDeserializedVector](const auto &val) { std::cout << val << ((iDeserializedVector.back() == val) ? "" : ", ") ; }); std::cout << std::endl;

	std::cout << "Deserialized userData: " << std::endl;
	std::cout << "* iVal: " << deserializedUserData.m_iData << std::endl;
	std::cout << "* dVal: " << deserializedUserData.m_dData << std::endl;
	std::cout << "* fVal: " << deserializedUserData.m_fData << std::endl;
	std::cout << "* strVal: " << deserializedUserData.m_strData << std::endl;

	return 0;
}