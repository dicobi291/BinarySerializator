#include <vector>
#include <cstddef>

namespace binary_serializator
{

enum class DataType : uint8_t {
	INT,
	FLOAT,

	UNKNOWN
};

template<DataType DATA_TYPE, typename T, typename = std::enable_if_t<std::is_fundamental_v<std::decay_t<T> > > >
void serialize(std::vector<std::byte> &buf, T&& data)
{
	static_assert(DATA_TYPE != DataType::UNKNOWN, "Error: serialzie unknown type.");

	auto dataSize = sizeof(data);
	buf.resize(buf.size() + dataSize + 1);
	buf[buf.size() - dataSize - 1] = static_cast<std::byte>(DATA_TYPE);
	std::memcpy(buf.data() + (buf.size() - dataSize), reinterpret_cast<const std::byte*>(&data), dataSize);
}

template<typename T, typename = std::enable_if_t<std::is_fundamental_v<T> > >
void deserialize(const std::byte *bufStart, T &data)
{
	std::memcpy(reinterpret_cast<std::byte *>(&data), bufStart, sizeof(data));
}

} //binary_serializator