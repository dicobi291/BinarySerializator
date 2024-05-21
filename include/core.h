#pragma once

#include <vector>

#include "supportedTypes.h"
#include "helpers.h"

namespace binary_serializator
{

namespace core
{

/*
* Serialize data and put it into the std::vector buffer of byte.
*/
template<
		typename T, //serialized type
		SupportedType SUPPORTED_TYPE = helpers::SupportedTypeFromType_v<std::decay_t<T> >, //DataType enum value detected from T
		typename = std::enable_if_t< //only for arithmetic types bool, char, int, float, double etc.
			std::is_arithmetic_v<
				std::decay_t<T> //remove from T const, volatile qualifires and all references
			>
		>
	>
void serialize(std::vector<std::byte> &buf, T &&data)
{
	//Compile time error while trying to serialize unsupported type.
	static_assert(SUPPORTED_TYPE != SupportedType::UNKNOWN, "Error: serialize unknown type. Check serialize(...) function call and DataType enum.");

	auto dataSize = sizeof(data);
	buf.resize(buf.size() + dataSize);
	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
}

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<std::decay_t<T> > > > //only for arithmetic types bool, char, int, float, double etc.
std::size_t deserialize(const std::vector<std::byte> &buf, const std::size_t &offset, T &result)
{
	std::memcpy(reinterpret_cast<std::byte *>(&result), buf.data() + offset, sizeof(result));
	return offset + sizeof(result);
}

} //core

} //binary_serializator