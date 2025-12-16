#pragma once

#include "Global.h"
#include "Helper.h"
#include "Album.h"

#include <QString>

#include <map>
// #include <algorithm>
// #include <numeric>

class Artist
{
public:
	Artist() = default;
	Artist(const QString& title) : _title(title) {}

	using AlbumsContainer = std::map<QString, Album>;

	AlbumsContainer::iterator begin() noexcept
	{ return _albums.begin(); }
	AlbumsContainer::const_iterator begin() const noexcept
	{ return _albums.begin(); }

	AlbumsContainer::iterator end() noexcept
	{ return _albums.end(); }
	AlbumsContainer::const_iterator end() const noexcept
	{ return _albums.end(); }

	Album& operator[](const QString& title)
	{ return _albums[title]; }

	inline void setTitle(const QString& title)
	{ _title = title; }
	inline QString title() const
	{ return _title; }
	inline bool isTitleEmpty() const noexcept
	{ return _title.isEmpty(); }

	auto minYear() const
	{
		auto min_year = Global::undefined_year;
		for (const auto& album : _albums) {
			min_year = Helper::checkMinYear(min_year, album.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = Global::undefined_year;
		for (const auto& album : _albums) {
			max_year = Helper::checkMaxYear(max_year, album.second.maxYear());
		}
		return max_year;
	}
	QString yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	uint32_t playCount() const
	{
		uint32_t sum = 0;
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
	QString _title;
	AlbumsContainer _albums;
};
