#pragma once
#include <concepts>
#include <vector>

#include "details.h"

namespace binary_serializator
{
    namespace core
    {
        using namespace details;

        //Constatns for boolean type serialization
        static constexpr unsigned char TRUE = 'T';
        static constexpr unsigned char FALSE = 'F';

        //Supported types concepts
        template<typename T>
        concept NumberType = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, bool>;

        template<typename T>
        concept BooleanType = std::same_as<T, bool>;

        template<typename T>
        concept StringType = std::same_as<T, std::string>;

        template<typename T>
        concept AllowedType = NumberType<T> || BooleanType<T> || StringType<T>;

        //serialization
        template<typename T> requires NumberType<std::decay_t<T> >
        constexpr inline void serialize(std::vector<Byte> &buff, T &&data)
        {
            std::cout << "serialize " << data << std::endl;
            auto dataSize = sizeof(data);
            buff.resize(buff.size() + dataSize);
            std::memcpy(buff.data() + (buff.size() - dataSize), reinterpret_cast<const Byte *>(&data), dataSize);
        }

        template<typename T> requires BooleanType<std::decay_t<T> >
        constexpr inline void serialize(std::vector<Byte> &buff, T &&data)
        {
            std::cout << "serialize bool " << data << std::endl;
            auto dataSize = sizeof(data);
            buff.resize(buff.size() + dataSize);
            std::memcpy(buff.data() + (buff.size() - dataSize), (data == true ? reinterpret_cast<const Byte *>(&TRUE) : reinterpret_cast<const Byte *>(&FALSE)), dataSize);
        }

        template<typename T> requires StringType<std::decay_t<T> >
        constexpr inline void serialize(std::vector<Byte> &buff, T &&data)
        {
            std::cout << "serialize string " << data << std::endl;
            auto dataSize = data.size();
            buff.resize(buff.size() + sizeof(dataSize) + dataSize);
            std::memcpy(buff.data() + (buff.size() - sizeof(dataSize) - dataSize), reinterpret_cast<const Byte *>(&dataSize), sizeof(dataSize));
            std::memcpy(buff.data() + (buff.size() - dataSize), reinterpret_cast<const Byte *>(data.data()), dataSize);
        }

        template<typename T> requires AllowedType<std::decay_t<T> >
        constexpr inline void serialize(std::vector<Byte> &buff, const std::vector<T> &data)
        {
            std::cout << "serialize vector " << data.size() << std::endl;
	        auto dataSize = data.size();
	        if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
	            dataSize *= 8;
	        }
	        buff.resize(buff.size() + sizeof(dataSize));
	        std::memcpy(buff.data() + (buff.size() - sizeof(dataSize)), reinterpret_cast<const Byte *>(&dataSize), sizeof(dataSize));
	        for (const auto &el : data) {
	            serialize(buff, el);
	        }
        }

        //deserialization
        template<typename T> requires NumberType<std::decay_t<T> >
        constexpr inline std::size_t deserialize(const std::vector<Byte> &buff, const std::size_t &offset, T &result)
        {
            std::cout << "deserialize arithmetic type" << std::endl;
		    std::memcpy(reinterpret_cast<Byte *>(&result), buff.data() + offset, sizeof(result));
	        return offset + sizeof(result);
        }

        template<typename T> requires BooleanType<std::decay_t<T> >
        constexpr inline std::size_t deserialize(const std::vector<Byte> &buff, const std::size_t &offset, T &result)
        {
            std::cout << "deserialize bool " << std::endl;
		    result = (reinterpret_cast<const char &>(buff[offset]) == TRUE);
	        return offset + sizeof(result);
        }

        template<typename T> requires StringType<std::decay_t<T> >
        constexpr inline std::size_t deserialize(const std::vector<Byte> &buff, const std::size_t &offset, T &result)
        {
            std::cout << "deserialize string " << std::endl;
		    decltype(result.size()) dataSize = 0;
		    std::memcpy(reinterpret_cast<Byte *>(&dataSize), buff.data() + offset, sizeof(dataSize));
		    result.resize(dataSize);
		    std::memcpy(reinterpret_cast<Byte *>(result.data()), buff.data() + offset + sizeof(dataSize), dataSize);
		    return offset + sizeof(dataSize) + result.size();
        }

        template<typename T> requires AllowedType<std::decay_t<T> >
        constexpr inline std::size_t deserialize(const std::vector<Byte> &buff, const std::size_t &offset, std::vector<T> &result)
        {
            std::cout << "deserialize vector " << std::endl;
	        decltype(result.size()) dataSize = 0;
	        std::memcpy(reinterpret_cast<Byte *>(&dataSize), buff.data() + offset, sizeof(dataSize));
	        if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
	            dataSize /= 8;
	        }
	        result.resize(dataSize);
	        std::size_t _offset = offset + sizeof(dataSize);
	        for (int i = 0; i < dataSize; i++) {
	            _offset = deserialize(buff, _offset, result[i]);
	        }

	        return _offset;
        }

    } //core

} //binary_serializator
