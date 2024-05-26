#include <concepts>

template<typename T>
concept NumberType = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, bool>;

template<typename T>
concept BooleanType = std::same_as<T, bool>;

template<typename T>
concept StringType = std::same_as<T, std::string>;

template<typename T>
concept AllowedType = NumberType<T> || BooleanType<T> || StringType<T>;

template<typename T> requires NumberType<std::decay_t<T> >
constexpr void serialize(T &&data)
{
    std::cout << "serialize " << data << std::endl;
}

template<typename T> requires BooleanType<std::decay_t<T> >
constexpr void serialize(T &&data)
{
    std::cout << "serialize bool" << data << std::endl;
}

template<typename T> requires StringType<std::decay_t<T> >
constexpr void serialize(T &&data)
{
    std::cout << "serialize string" << data << std::endl;
}

template<typename T> requires AllowedType<std::decay_t<T> >
constexpr void serialize(const std::vector<T> &data)
{
    std::cout << "serialize vector" << std::endl;
}

template<typename T, std::size_t N> requires AllowedType<std::decay_t<T> >
constexpr void serialize(const T(&arr)[N])
{
    std::cout << "serialize fixed array" << std::endl;
}
