#pragma once
#include "core.h"

namespace binary_serializator
{

/*
* Class which provides interface for serialize/deserialize
* functionality via protected functions. All user types
* that need to be [de]serialized must be inherited from it.
*/
class ISerializable
{
//Forbidden dynamic polymorphism.
protected:
	~ISerializable() = default;

	template<typename T>
	void serializeData(std::vector<details::Byte> &buff, T&& data)
	{
		binary_serializator::core::serialize(buff, std::forward<T>(data));
	}

	template<typename T>
	std::size_t deserializeData(const std::vector<details::Byte> &buf, const std::size_t &offset, T &result)
	{
		return binary_serializator::core::deserialize(buf, offset, result);
	}

public:
	virtual std::vector<details::Byte> serialize() = 0;
	virtual void deserialize(const std::vector<details::Byte> &buf) = 0;
};

} //binary_serializator