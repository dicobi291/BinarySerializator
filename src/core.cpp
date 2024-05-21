#include "../include/core.h"

namespace binary_serializator
{

namespace core
{

/*
* Serialize data and put it into the std::vector buffer of byte.
*/
//template<
//		typename T, //serialized type
//		SerializedType SERIALIZED_TYPE, //DataType enum value detected from T
//		typename
//>
//void serialize(std::vector<std::byte> &buf, T &&data)
//{
//	//Compile time error while trying to serialize unsupported type.
//	static_assert(SERIALIZED_TYPE != SerializedType::UNKNOWN, "Error: serialize unknown type. Check serialize(...) function call and DataType enum.");
//
//	auto dataSize = sizeof(data);
//	buf.resize(buf.size() + dataSize + 1);
//	buf[buf.size() - dataSize - 1] = static_cast<std::byte>(SERIALIZED_TYPE);
//	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
//}

} //core

} //binary_serializator
