#ifndef __MATRIX__
#define __MATRIX__

#include <array>


template <typename T = std::uint32_t,
		  std::uint32_t m = 0,
		  std::uint32_t n = 0>
class matrix
{
public:
	const auto& operator[] 
	(std::uint32_t i) const -> std::array<T, n>&
	{ return _arr[i]; }

private:
	std::array <std::array<T, n>, m> _arr;
};

#endif