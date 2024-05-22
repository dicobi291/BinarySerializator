#pragma once

namespace binary_serializator
{

/*
* Types supported for serializtaion.
*/
enum class SupportedType : uint8_t {
	BOOL,
	INT,
	UINT,
	FLOAT,
	DOUBLE,

	UNKNOWN
};

}