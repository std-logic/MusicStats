#pragma once

#include "BasicTypes.h"
#include "Helper.h"
#include "Artist.h"

#include <map>

class Library
{
public:
	Library() = default;
	Library(const BasicTypes::StringType& title) : _title(title) {}

	using ArtistsContainer = std::map<BasicTypes::StringType, Artist>;

	ArtistsContainer::iterator begin() noexcept
	{ return _artists.begin(); }
	ArtistsContainer::const_iterator begin() const noexcept
	{ return _artists.begin(); }

	ArtistsContainer::iterator end() noexcept
	{ return _artists.end(); }
	ArtistsContainer::const_iterator end() const noexcept
	{ return _artists.end(); }

	Artist& operator[](const BasicTypes::StringType& title)
	{ return _artists[title]; }

	inline void setTitle(const BasicTypes::StringType& title)
	{ _title = title; }
	inline BasicTypes::StringType title() const
	{ return _title; }

	auto minYear() const
	{
		auto min_year = BasicTypes::undefined_year;
		for (const auto& artist : _artists) {
			min_year = Helper::checkMinYear(min_year, artist.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = BasicTypes::undefined_year;
		for (const auto& artist : _artists) {
			max_year = Helper::checkMaxYear(max_year, artist.second.maxYear());
		}
		return max_year;
	}
	BasicTypes::StringType yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	unsigned int playCount() const
	{
		unsigned int sum = 0;
		for (const auto& artist : _artists) {
			sum += artist.second.playCount();
		}
		return sum;
	}

	uint64_t size() const
	{
		auto sum = uint64_t(0);
		for (const auto& artist : _artists) {
			sum += artist.second.size();
		}
		return sum;
	}

	auto artistsCount() const
	{ return _artists.size(); }

	auto albumsCount() const
	{
		size_t sum = 0;
		for (const auto& map_item : _artists) { sum += map_item.second.albumsCount(); }
		return sum;
	}

	auto tracksCount() const
	{
		size_t sum = 0;
		for (const auto& map_item : _artists) { sum += map_item.second.tracksCount(); }
		return sum;
	}

private:
	BasicTypes::StringType _title;
	ArtistsContainer _artists;
};
