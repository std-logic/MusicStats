#pragma once

#include "Global.h"
#include "Helper.h"
#include "Track.h"

#include <QString>

#include <vector>

class Album
{
public:
	Album() = default;
	Album(const QString& title) : _title(title) {}

	using TracksContainer = std::vector<Track>;

	TracksContainer::iterator begin() noexcept
	{ return _tracks.begin(); }
	TracksContainer::const_iterator begin() const noexcept
	{ return _tracks.begin(); }

	TracksContainer::iterator end() noexcept
	{ return _tracks.end(); }
	TracksContainer::const_iterator end() const noexcept
	{ return _tracks.end(); }

	inline Track& operator[](size_t index)
	{ return _tracks[index]; }
	inline const Track& operator[](size_t index) const
	{ return _tracks[index]; }

	inline void setTitle(const QString& title)
	{ _title = title; }
	inline QString title() const
	{ return _title; }
	inline bool isTitleEmpty() const noexcept
	{ return _title.isEmpty(); }

	inline QString artist() const
	{ return !_tracks.empty() ? _tracks.front().artist() : QString(); }

	inline QString genre() const
	{ return !_tracks.empty() ? _tracks.front().genre() : QString(); }

	inline QString publisher() const
	{ return !_tracks.empty() ? _tracks.front().publisher() : QString(); }

	auto minYear() const
	{
		auto min_year = Global::undefined_year;
		for (const auto& track : _tracks) {
			min_year = Helper::checkMinYear(min_year, track.year());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = Global::undefined_year;
		for (const auto& track : _tracks) {
			max_year = Helper::checkMaxYear(max_year, track.year());
		}
		return max_year;
	}
	QString yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	uint32_t playCount() const
	{
		uint32_t sum = 0;
		for (const auto& track : _tracks) { sum += track.playCount(); }
		return sum;
	}

	uint64_t size() const
	{
		auto sum = uint64_t(0);
		for (const auto& track : _tracks) { sum += track.size(); }
		return sum;
	}

	uint64_t time() const
	{
		auto sum = uint64_t(0);
		for (const auto& track : _tracks) { sum += track.time(); }
		return sum;
	}

	auto tracksCount() const
	{ return _tracks.size(); }

	void addTrack(const Track& track)
	{ _tracks.push_back(track); }

	QString fullInfo() const
	{
		QString text;
		text += QString("Альбом: %1").arg(_title);
		QString year_string = yearString();
		if (!year_string.isEmpty()) {
			text += QString("\nГод: %1").arg(year_string);
		}
		text += QString("\nТреков: %1").arg(tracksCount());
		text += QString("\nПрослушиваний: %1").arg(playCount());
		text += QString("\nЖанр: %1").arg(genre());
		text += QString("\nДлина: %1").arg(Helper::timeString(time()));
		text += QString("\nРазмер: %1").arg(Helper::sizeString(size()));
		QString publisher_string = publisher();
		if (!publisher_string.isEmpty()) {
			text += QString("\nИздатель: %1").arg(publisher_string);
		}
		return text;
	}

private:
	QString _title;
	TracksContainer _tracks;
};
