#pragma once

#include "BasicTypes.h"
#include "Helper.h"
#include "Track.h"

#include <vector>
// #include <algorithm>

class Album
{
public:
	Album() = default;
	Album(const BasicTypes::StringType& title) : _title(title) {}

	using TracksContainer = std::vector<Track>;

	TracksContainer::iterator begin() noexcept
	{ return _tracks.begin(); }
	TracksContainer::const_iterator begin() const noexcept
	{ return _tracks.begin(); }

	TracksContainer::iterator end() noexcept
	{ return _tracks.end(); }
	TracksContainer::const_iterator end() const noexcept
	{ return _tracks.end(); }

	Track& operator[](size_t index)
	{ return _tracks[index]; }
	const Track& operator[](size_t index) const
	{ return _tracks[index]; }

	inline void setTitle(const BasicTypes::StringType& title)
	{ _title = title; }
	inline BasicTypes::StringType title() const
	{ return _title; }
	inline bool isTitleEmpty() const
	{ return _title.empty(); }

	auto minYear() const
	{
		auto min_year = BasicTypes::undefined_year;
		for (const auto& track : _tracks) {
			min_year = Helper::checkMinYear(min_year, track.year());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = BasicTypes::undefined_year;
		for (const auto& track : _tracks) {
			max_year = Helper::checkMaxYear(max_year, track.year());
		}
		return max_year;
	}
	BasicTypes::StringType yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	unsigned int playCount() const
	{
		unsigned int sum = 0;
		for (const auto& track : _tracks) {
			sum += track.playCount();
		}
		return sum;
	}

	auto tracksCount() const
	{ return _tracks.size(); }

	void addTrack(const Track& track)
	{ _tracks.push_back(track); }

private:
	BasicTypes::StringType _title;
	TracksContainer _tracks;
};
