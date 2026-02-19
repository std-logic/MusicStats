#pragma once

#include "Global.h"

#include <QString>

namespace Helper
{
	inline auto checkMinYear(uint32_t curr_min_year, uint32_t year)
	{
		if (year != Global::undefined_year) {
			if ((curr_min_year > year) || (curr_min_year == Global::undefined_year)) {
				curr_min_year = year;
			}
		}
		return curr_min_year;
	}
	inline auto checkMaxYear(uint32_t curr_max_year, uint32_t year)
	{
		if (year != Global::undefined_year) {
			if ((curr_max_year < year) || (curr_max_year == Global::undefined_year)) {
				curr_max_year = year;
			}
		}
		return curr_max_year;
	}
	inline QString yearString(uint32_t min_year, uint32_t max_year)
	{
		return	(min_year == Global::undefined_year) ? QString() :
				(min_year == max_year) ? QString::number(min_year) :
				QString("%1-%2").arg(min_year).arg(max_year);
	}

	inline uint64_t sizeInMB(uint64_t size_in_bytes)
	{
		return (size_in_bytes / uint64_t(1048576));
	}
	inline QString sizeString(uint64_t size)
	{
		return	(size <= 1024 * 1024 * 1024) ?
				QString("%1 МБ").arg(static_cast<double>(size) / (1024. * 1024.), 0, 'f', 0) :
				QString("%1 ГБ").arg(static_cast<double>(size) / (1024. * 1024. * 1024.), 0, 'f', 2);
	}

	inline QString timeString(uint32_t time)
	{
		return	(time < 60 * 60000) ?
				QString("%1:%2")
					.arg(time / 60000)
					.arg((time % 60000) / 1000, 2, 10, QChar('0')) :
				QString("%1:%2:%3")
					.arg(time / (60 * 60000))
					.arg((time % (60 * 60000)) / 60000, 2, 10, QChar('0'))
					.arg((time % 60000) / 1000, 2, 10, QChar('0'));
	}
}
