#pragma once

#include "BasicTypes.h"
#include "Helper.h"
#include "Album.h"

#include <map>
// #include <algorithm>
// #include <numeric>

class Artist
{
public:
	Artist() = default;
	Artist(const BasicTypes::StringType& title) : _title(title) {}

	using AlbumsContainer = std::map<BasicTypes::StringType, Album>;

	AlbumsContainer::iterator begin() noexcept
	{ return _albums.begin(); }
	AlbumsContainer::const_iterator begin() const noexcept
	{ return _albums.begin(); }

	AlbumsContainer::iterator end() noexcept
	{ return _albums.end(); }
	AlbumsContainer::const_iterator end() const noexcept
	{ return _albums.end(); }

	Album& operator[](const BasicTypes::StringType& title)
	{ return _albums[title]; }

	inline void setTitle(const BasicTypes::StringType& title)
	{ _title = title; }
	inline BasicTypes::StringType title() const
	{ return _title; }
	inline bool isTitleEmpty() const
	{ return _title.empty(); }

	auto minYear() const
	{
		auto min_year = BasicTypes::undefined_year;
		for (const auto& album : _albums) {
			min_year = Helper::checkMinYear(min_year, album.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = BasicTypes::undefined_year;
		for (const auto& album : _albums) {
			max_year = Helper::checkMaxYear(max_year, album.second.maxYear());
		}
		return max_year;
	}
	BasicTypes::StringType yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	unsigned int playCount() const
	{
		unsigned int sum = 0;
		for (const auto& album : _albums) {
			sum += album.second.playCount();
		}
		return sum;
	}

	uint64_t size() const
	{
		auto sum = uint64_t(0);
		for (const auto& album : _albums) {
			sum += album.second.size();
		}
		return sum;
	}

	auto albumsCount() const
	{ return _albums.size(); }

	auto tracksCount() const
	{
		size_t sum = 0;
		for (const auto& album : _albums) {
			sum += album.second.tracksCount();
		}
		return sum;
	}

private:
	BasicTypes::StringType _title;
	AlbumsContainer _albums;
};
