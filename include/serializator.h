#pragma once

#include <vector>
#include <cstddef>

namespace binary_serializator
{

/*
* Types supported for serializtaion.
*/
enum class DataType : uint8_t {
	INT,
	UINT,
	FLOAT,
	DOUBLE,

	USER_DATA,
	USER_DATA_,

	UNKNOWN
};

/*
* Helpers templates to detect DataType enum value from serialized type.
*/
namespace helpers {
	/*
	* Compile time if-else.
	* if T == int then DT == DataType::INT
	* if T == float then DT == DataType::FLOAT
	* etc...
	* if T is not one of supported type then DT == DataType::UNKNOWN
	*/
	template<typename T>
	struct DataTypeFromType
	{
		static constexpr DataType DT = DataType::UNKNOWN;
	};

	template<>
	struct DataTypeFromType<int>
	{
		static constexpr DataType DT = DataType::INT;
	};

	template<>
	struct DataTypeFromType<unsigned int>
	{
		static constexpr DataType DT = DataType::UINT;
	};

	template<>
	struct DataTypeFromType<float>
	{
		static constexpr DataType DT = DataType::FLOAT;
	};

	template<>
	struct DataTypeFromType<double>
	{
		static constexpr DataType DT = DataType::DOUBLE;
	};

	/*
	* Just for simplify using like in stl templates.
	*/
	template<typename T>
	inline constexpr DataType DataTypeFromType_v = DataTypeFromType<T>::DT;
}

class ISerializable
{
protected:
	~ISerializable() = default;

public:
	virtual void serialize(std::vector<std::byte> &buf) = 0;
	virtual void deserialize(const std::vector<std::byte> &buf) = 0;
};

class Serializator
{
public:
	/*
	* Serialize data and put it into the std::vector buffer of byte.
	*/
	template<
		typename T, //serialized type
		DataType DATA_TYPE = helpers::DataTypeFromType_v<std::decay_t<T> >, //DataType enum value detected from T
		typename = std::enable_if_t< //only for arithmetic types bool, char, int, float, double etc.
			std::is_arithmetic_v<
			std::decay_t<T> //removed from T const, volatile qualifires and all references
			>
		>
	>
	static void serialize(T &&data)
	{
		//Compile time error while trying to serialize unsupported type.
		static_assert(DATA_TYPE != DataType::UNKNOWN, "Error: serialize unknown type. Check serialize(...) function call and DataType enum.");

		auto dataSize = sizeof(data);
		m_serializedData.resize(m_serializedData.size() + dataSize + 1);
		m_serializedData[m_serializedData.size() - dataSize - 1] = static_cast<std::byte>(DATA_TYPE);
		std::memcpy(m_serializedData.data() + (m_serializedData.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
	}

	template<
		typename T, //serialized type
		typename = std::enable_if_t< //only for arithmetic types bool, char, int, float, double etc.
			std::is_base_of_v<ISerializable, std::decay_t<T>>
		>
	>
	static void serialize(T &&data)
	{
		data.serialize(m_serializedData);
	}

	/*
	* Deserialize data from bufStart place and put it into the result variable.
	*/
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> > > //only for arithmetic types bool, char, int, float, double etc.
	static std::size_t deserialize(const std::vector<std::byte> buf, std::size_t offset, T &result)
	{
		std::memcpy(reinterpret_cast<std::byte *>(&result), buf.data() + offset, sizeof(result));
		return offset + sizeof(result);
	}

private:
	static std::vector<std::byte> m_serializedData;
};

class UserData_ : public ISerializable
{
	int m_iData;
	double m_dData;

public:
	void serialize(std::vector<std::byte> &buf) override
	{
		buf.resize(buf.size() + 1);
		buf[buf.size() - 1] = static_cast<std::byte>(DataType::USER_DATA_);
		Serializator::serialize(m_iData);
		Serializator::serialize(m_dData);
	}

	void deserialize(const std::vector<std::byte> &buf) override
	{
		std::size_t offset = 0;
		offset = Serializator::deserialize(buf, offset, m_iData);
		offset = Serializator::deserialize(buf, offset, m_dData);
	}
};

/*
* Serialize data and put it into the std::vector buffer of byte.
*/
template<
		typename T, //serialized type
		DataType DATA_TYPE = helpers::DataTypeFromType_v<std::decay_t<T> >, //DataType enum value detected from T
		typename = std::enable_if_t< //only for arithmetic types bool, char, int, float, double etc.
			std::is_arithmetic_v<
				std::decay_t<T> //remove from T const, volatile qualifires and all references
			>
		>
	>
void serialize(std::vector<std::byte> &buf, T&& data)
{
	//Compile time error while trying to serialize unsupported type.
	static_assert(DATA_TYPE != DataType::UNKNOWN, "Error: serialize unknown type. Check serialize(...) function call and DataType enum.");

	auto dataSize = sizeof(data);
	buf.resize(buf.size() + dataSize + 1);
	buf[buf.size() - dataSize - 1] = static_cast<std::byte>(DATA_TYPE);
	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
}

/*
* Deserialize data from bufStart place and put it into the result variable.
*/
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> > > //only for arithmetic types bool, char, int, float, double etc.
std::size_t deserialize(const std::vector<std::byte> buf, std::size_t offset, T &result)
{
	std::memcpy(reinterpret_cast<std::byte *>(&result), buf.data() + offset, sizeof(result));
	return offset + sizeof(result);
}

/*
* Base struct for user defined structures
* which we want serialize/deserialize.
*/
struct ISerialize
{
//Forbidden dynamic polymorphism.
protected:
	~ISerialize() = default;

public:
	virtual void serialize(std::vector<std::byte> &buf) = 0;
	virtual std::size_t deserialize(const std::vector<std::byte> &buf, std::size_t offset) = 0;
};

} //binary_serializator