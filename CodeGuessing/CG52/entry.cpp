#include <iostream>
#include <bitset>

template<std::size_t n>
class Pow2
{
public:
    constexpr static std::size_t value = 2 * Pow2<n-1>::value;
};

template<>
class Pow2<0>
{
public:
    constexpr static std::size_t value = 1;
};

template<std::size_t n>
constexpr std::bitset<2*n> generate(const std::bitset<n>& bs)
{
    std::bitset<2*n> result{};
    auto inv = ~bs;

    for(std::size_t i = 0; i < n; ++i)
    {
        result[i] = inv[i];
    }
    for(std::size_t i = 0; i < n; ++i)
    {
        result[n + i] = bs[i];
    }
    return result;
}

template<std::size_t n>
class ThueMorse
{
public:
    static constexpr std::bitset<Pow2<n>::value> value = generate(ThueMorse<n-1>::value);
};

template<>
class ThueMorse<0>
{
public:
    static constexpr std::bitset<1> value{0};
};

int main()
{
    std::cout << ThueMorse<6>::value.to_string() << std::endl;

    return 0;
}
