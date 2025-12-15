#pragma once

#include "BasicTypes.h"

namespace Helper
{
	inline auto checkMinYear(uint32_t curr_min_year, uint32_t year)
	{
		if (year != BasicTypes::undefined_year) {
			if ((curr_min_year > year) || (curr_min_year == BasicTypes::undefined_year)) {
				curr_min_year = year;
			}
		}
		return curr_min_year;
	}
	inline auto checkMaxYear(uint32_t curr_max_year, uint32_t year)
	{
		if (year != BasicTypes::undefined_year) {
			if ((curr_max_year < year) || (curr_max_year == BasicTypes::undefined_year)) {
				curr_max_year = year;
			}
		}
		return curr_max_year;
	}
	inline BasicTypes::StringType yearString(uint32_t min_year, uint32_t max_year)
	{
		return	(min_year == BasicTypes::undefined_year) ?
				BasicTypes::StringType("") : (min_year == max_year) ?
				std::to_string(min_year) :
				std::to_string(min_year) + "-" + std::to_string(max_year);
	}

	inline uint64_t sizeInMB(uint64_t size_in_bytes)
	{
		return (size_in_bytes / uint64_t(1048576));
	}
}
