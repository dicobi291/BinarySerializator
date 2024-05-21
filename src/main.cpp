#include <iostream>

#include "../include/serializator.h"
#include "../include/core.h"

struct UserData : public binary_serializator::ISerializable
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

	std::vector<std::byte> serialize() override
	{
		std::vector<std::byte> buff;

		ISerializable::serializeData(buff, m_iData);
		ISerializable::serializeData(buff, m_dData);
		ISerializable::serializeData(buff, m_fData);

		return buff;
	}

	void deserialize(const std::vector<std::byte> &buff) override
	{
		std::size_t offset = 0;
		
		offset = ISerializable::deserializeData(buff, offset, m_iData);
		offset = ISerializable::deserializeData(buff, offset, m_dData);
		offset = ISerializable::deserializeData(buff, offset, m_fData);
	}
};

int main()
{
	using namespace binary_serializator;

	std::cout << "Hello binary serializator\n";

	std::vector<std::byte> buff;

	int iVal = 156;
	binary_serializator::core::serialize(buff, iVal);
	binary_serializator::core::serialize(buff, 234.86f);
	binary_serializator::core::serialize(buff, 568324.86);
	binary_serializator::core::serialize(buff, 2356u);
	
	UserData userData {5, 875.54, 936.4f};

	auto userBuff = userData.serialize();

	int iDeserializeVal = 0;
	float fDeserializeVal = 0.0f;
	double dDeserializeVal = 0.0;
	unsigned int uDeserializeVal = 0;
	UserData deserializedUserData;
	
	std::size_t offset = 0;

	offset = binary_serializator::core::deserialize(buff, offset, iDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, fDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, dDeserializeVal);
	offset = binary_serializator::core::deserialize(buff, offset, uDeserializeVal);

	deserializedUserData.deserialize(userBuff);

	std::cout << "Deserialized int val: " << iDeserializeVal << std::endl;
	std::cout << "Deserialized float val: " << fDeserializeVal << std::endl;
	std::cout << "Deserialized double val: " << std::fixed << dDeserializeVal << std::endl;
	std::cout << "Deserialized unsigned int val: " << uDeserializeVal << std::endl;

	std::cout << "Deserialized userData: " << std::endl;
	std::cout << "* iVal: " << deserializedUserData.m_iData << std::endl;
	std::cout << "* dVal: " << deserializedUserData.m_dData << std::endl;
	std::cout << "* fVal: " << deserializedUserData.m_fData << std::endl;

	return 0;
}