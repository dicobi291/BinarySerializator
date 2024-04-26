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
	std::memcpy(buf.data() + (buf.size() - dataSize - 1), reinterpret_cast<std::byte*>(dataType), sizeof(dataType));
	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<std::byte*>(data), dataSize);
}

} //binary_serializator