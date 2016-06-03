#pragma once

namespace Utility {

	template <typename T> inline T make_bits(size_t pos)
	{
		return static_cast<T>(1) << pos;
	}

	template <typename T, typename... Rest> inline T make_bits(size_t pos, Rest... rest)
	{
		return make_bits<T>(rest...) | (static_cast<T>(1) << pos);
	}
};
