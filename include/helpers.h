#pragma once

#include "supportedTypes.h"

namespace binary_serializator
{

/*
* Helpers templates to detect SupportedType enum value from serialized type.
*/
namespace helpers {
	/*
	* Compile time if-else.
	* if T == int then DT == SupportedType::INT
	* if T == float then DT == SupportedType::FLOAT
	* etc...
	* if T is not one of supported type then DT == SupportedType::UNKNOWN
	*/
	template<typename T>
	struct SupportedTypeFromType
	{
		static constexpr SupportedType DT = SupportedType::UNKNOWN;
	};

	template<typename T>
	struct SupportedTypeFromType<std::vector<T> >
	{
		static constexpr SupportedType DT = SupportedType::VECTOR;
	};

	/*
	* I don't like macros, but this just only because I'm lazy
	* to print full template spec again when add new type support.
	* Template specialization for realizing compile time if-else.
	*/
#define Spec(Type, EnumType) \
	template<> \
	struct SupportedTypeFromType<Type> {\
		static constexpr SupportedType DT = SupportedType::EnumType; \
	};

	Spec(bool, BOOL)
	Spec(int, INT)
	Spec(unsigned int, UINT)
	Spec(float, FLOAT)
	Spec(double, DOUBLE)
	Spec(std::string, STRING)
#undef Spec

	/*
	* Just for simplify using like in stl templates.
	*/
	template<typename T>
	inline constexpr SupportedType SupportedTypeFromType_v = SupportedTypeFromType<T>::DT;
}

}
