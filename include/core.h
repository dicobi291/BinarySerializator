#pragma once

#include <vector>

#include "supportedTypes.h"
#include "helpers.h"

namespace binary_serializator
{

namespace core
{

constexpr char TRUE = 'T';
constexpr char FALSE = 'F';

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
			||
			std::is_same_v<
				std::string,
				std::decay_t<T> //remove from T const, volatile qualifires and all references
			>
		>
	>
void serialize(std::vector<std::byte> &buf, T &&data)
{
	//Compile time error while trying to serialize unsupported type.
	static_assert(SUPPORTED_TYPE != SupportedType::UNKNOWN, "Error: serialize unknown type. Check serialize(...) function call and DataType enum.");

	auto dataSize = sizeof(data);
	if constexpr (SUPPORTED_TYPE == SupportedType::BOOL) {
		buf.resize(buf.size() + dataSize);
		std::memcpy(buf.data() + (buf.size() - dataSize), (data == true ? reinterpret_cast<const std::byte *>(&TRUE) : reinterpret_cast<const std::byte *>(&FALSE)), dataSize);
	} else if constexpr (SUPPORTED_TYPE == SupportedType::STRING) {
		dataSize = data.size();
		buf.resize(buf.size() + sizeof(dataSize) + dataSize);
		std::memcpy(buf.data() + (buf.size() - sizeof(dataSize) - dataSize), reinterpret_cast<const std::byte *>(&dataSize), sizeof(dataSize));
		std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte *>(data.data()), dataSize);
	} {
		buf.resize(buf.size() + dataSize);
		std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte *>(&data), dataSize);
	}
}

template<
		typename T,
		SupportedType SUPPORTED_TYPE = helpers::SupportedTypeFromType_v<std::decay_t<T> >, //DataType enum value detected from T
		typename = std::enable_if_t< //only for arithmetic types bool, char, int, float, double etc.
			std::is_arithmetic_v<
				std::decay_t<T> //remove from T const, volatile qualifires and all references
			>
			||
			std::is_same_v<
				std::string,
				std::decay_t<T> //remove from T const, volatile qualifires and all references
			>
		>
	>
void serialize(std::vector<std::byte> &buf, const std::vector<T> &data)
{
	//Compile time error while trying to serialize unsupported type.
	static_assert(SUPPORTED_TYPE != SupportedType::UNKNOWN, "Error: serialize vector of unknown type. Check serialize(..., std::vector<...>) function call and DataType enum.");

	auto dataSize = data.size();
	if constexpr (SUPPORTED_TYPE == SupportedType::BOOL) {
		dataSize *= 8;
	}
	buf.resize(buf.size() + sizeof(dataSize));
	std::memcpy(buf.data() + (buf.size() - sizeof(dataSize)), reinterpret_cast<const std::byte *>(&dataSize), sizeof(dataSize));
	for (const auto &el : data) {
		serialize(buf, el);
	}
}

template<
	typename T,
	typename = std::enable_if_t<
		std::is_arithmetic_v<
			std::decay_t<T>
		>
		||
		std::is_same_v<
			std::string,
			std::decay_t<T> //remove from T const, volatile qualifires and all references
		>
	>
> //only for arithmetic types bool, char, int, float, double etc.
std::size_t deserialize(const std::vector<std::byte> &buf, const std::size_t &offset, T &result)
{
	if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
		result = (reinterpret_cast<const char &>(buf[offset]) == TRUE);
	} else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
		decltype(result.size()) dataSize = 0;
		std::memcpy(reinterpret_cast<std::byte *>(&dataSize), buf.data() + offset, sizeof(dataSize));
		result.resize(dataSize);
		std::memcpy(reinterpret_cast<std::byte *>(result.data()), buf.data() + offset + sizeof(dataSize), dataSize);
		return offset + sizeof(dataSize) + result.size();
	} {
		std::memcpy(reinterpret_cast<std::byte *>(&result), buf.data() + offset, sizeof(result));
	}
	return offset + sizeof(result);
}

template<
	typename T,
	typename = std::enable_if_t<
		std::is_arithmetic_v<
			std::decay_t<T>
		>
		||
		std::is_same_v<
			std::string,
			std::decay_t<T> //remove from T const, volatile qualifires and all references
		>
	>
> //only for arithmetic types bool, char, int, float, double etc.
std::size_t deserialize(const std::vector<std::byte> &buf, const std::size_t &offset, std::vector<T> &result)
{
	decltype(result.size()) dataSize = 0;
	std::memcpy(reinterpret_cast<std::byte *>(&dataSize), buf.data() + offset, sizeof(dataSize));
	if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
		dataSize /= 8;
	}
	result.resize(dataSize);
	std::size_t _offset = offset + sizeof(dataSize);
	for (int i = 0; i < dataSize; i++) {
		_offset = deserialize(buf, _offset, result[i]);
	}

	return _offset;
}

} //core

} //binary_serializator