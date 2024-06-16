#pragma once

namespace binary_serializator
{
	namespace details
	{
#if _MSC_VER && !__INTEL_COMPILER
	#if _MSVC_LANG >= 201703L
		using Byte = std::byte;
	#else
		using Byte = uint8_t;
	#endif
#else
	#if __cplusplus >= 201703L
		using Byte = std::byte;
	#else
		using Byte = uint8_t;
	#endif
#endif
	} //details

} //binary_serializator