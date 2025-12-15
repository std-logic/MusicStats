#pragma once

#include "BasicTypes.h"

class Track
{
public:
	Track() = default;
	Track(const BasicTypes::StringType& title) : _title(title) {}

	inline bool operator==(const Track& track) const noexcept
	{
		return	(_title == track.title()) &&
				(_album == track.album()) &&
				(_artist == track.artist());
	}

	Track& operator-=(const Track& track) noexcept
	{
		if (_play_count >= track._play_count) {
			_play_count -= track._play_count;
		} else {
			_play_count = 0;
		}
		return *this;
	}

	inline void setTitle(const BasicTypes::StringType& title)
	{ _title = title; }
	inline void setTitle(BasicTypes::StringType&& title)
	{ _title = std::move(title); }
	inline BasicTypes::StringType title() const
	{ return _title; }
	BasicTypes::StringType titleWithTrackNumber() const
	{
		return	(_track_number == BasicTypes::undefined_track_number) ?
				_title :
				(_track_number < 10 ? "0" : "") + std::to_string(_track_number) + ". " + _title;
	}

	inline void setAlbum(const BasicTypes::StringType& album)
	{ _album = album; }
	inline void setAlbum(BasicTypes::StringType&& album)
	{ _album = std::move(album); }
	inline BasicTypes::StringType album() const
	{ return _album; }

	inline void setArtist(const BasicTypes::StringType& artist)
	{ _artist = artist; }
	inline void setArtist(BasicTypes::StringType&& artist)
	{ _artist = std::move(artist); }
	inline BasicTypes::StringType artist() const
	{ return _artist; }

	inline void setTrackNumer(uint32_t track_number)
	{ _track_number = track_number; }
	inline uint32_t trackNumber() const
	{ return _track_number; }

	inline void setYear(uint32_t year)
	{ _year = year; }
	inline uint32_t year() const
	{ return _year; }
	BasicTypes::StringType yearString() const
	{
		return	(_year == BasicTypes::undefined_year) ?
				BasicTypes::StringType("") :
				std::to_string(_year);
	}

	inline void setPlayCount(uint32_t play_count)
	{ _play_count = play_count; }
	inline uint32_t playCount() const
	{ return _play_count; }

	inline void setSize(uint32_t size)
	{ _size = size; }
	inline uint64_t size() const
	{ return _size; }

	inline void setTime(uint32_t time)
	{ _time = time; }
	inline uint32_t time() const
	{ return _time; }

private:
	BasicTypes::StringType _title;
	BasicTypes::StringType _album;
	BasicTypes::StringType _artist;
	uint32_t _track_number = BasicTypes::undefined_track_number;
	uint32_t _year = BasicTypes::undefined_year;
	uint32_t _play_count = 0;
	uint32_t _size = 0;
	uint32_t _time = 0;
};
