#include <cstdint>
#include <iostream>

template <std::size_t SIZE>
class bignum
{
public:
	constexpr static std::size_t size = SIZE;
};


int main()
{
	bignum<128> a;
	std::cout << a.size;
}
