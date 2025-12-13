#pragma once

#include "BasicTypes.h"

class Track
{
public:
	Track() = default;
	Track(const BasicTypes::StringType& title) : _title(title) {}

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

	inline void setTrackNumer(unsigned int track_number)
	{ _track_number = track_number; }
	inline unsigned int trackNumber() const
	{ return _track_number; }

	inline void setYear(unsigned int year)
	{ _year = year; }
	inline unsigned int year() const
	{ return _year; }
	BasicTypes::StringType yearString() const
	{
		return	(_year == BasicTypes::undefined_year) ?
				BasicTypes::StringType("") :
				std::to_string(_year);
	}

	inline void setPlayCount(unsigned int play_count)
	{ _play_count = play_count; }
	inline unsigned int playCount() const
	{ return _play_count; }

	inline void setSize(unsigned int size)
	{ _size = size; }
	inline unsigned int size() const
	{ return _size; }

	inline void setTime(unsigned int time)
	{ _time = time; }
	inline unsigned int time() const
	{ return _time; }

private:
	BasicTypes::StringType _title;
	BasicTypes::StringType _album;
	BasicTypes::StringType _artist;
	unsigned int _track_number = BasicTypes::undefined_track_number;
	unsigned int _year = BasicTypes::undefined_year;
	unsigned int _play_count = 0;
	unsigned int _size = 0;
	unsigned int _time = 0;
};
