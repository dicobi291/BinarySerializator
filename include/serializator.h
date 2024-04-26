#include <vector>
#include <cstddef>

namespace binary_serializator
{

enum class DataType : uint8_t {
	INT,
	FLOAT,

	UNKNOWN
};

template<typename T, typename = std::enable_if_t<std::is_fundamental_v<T> > >
void serialize(std::vector<std::byte> &buf, const DataType dataType, const T& data)
{
	if (dataType == DataType::UNKNOWN) {
		throw std::runtime_error("Error: serialzie unknown type.");
	}

	auto dataSize = sizeof(data);
	buf.resize(buf.size() + dataSize + 1);
	std::memcpy(buf.data() + (buf.size() - dataSize - 1), reinterpret_cast<const std::byte*>(&dataType), sizeof(dataType));
	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
}

template<typename T, typename = std::enable_if_t<std::is_fundamental_v<T> > >
void deserialize(const std::byte *bufStart, T &data)
{
	std::memcpy(reinterpret_cast<std::byte *>(&data), bufStart, sizeof(data));
}

} //binary_serializator