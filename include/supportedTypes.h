#pragma once

namespace binary_serializator
{

/*
* Types supported for serializtaion.
*/
enum class SupportedType : uint8_t {
	INT,
	UINT,
	FLOAT,
	DOUBLE,

	UNKNOWN
};

}