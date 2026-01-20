#pragma once

#include "Global.h"

#include <QString>

class Track
{
public:
	Track() = default;
	Track(const QString& title) : _title(title) {}

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

	inline void setTitle(const QString& title)
	{ _title = title; }
	inline void setTitle(QString&& title)
	{ _title = std::move(title); }
	inline QString title() const
	{ return _title; }
	QString titleWithTrackNumber() const
	{
		return	(_track_number == Global::undefined_track_number) ?
				_title :
				QString("%1. %2").arg(_track_number, 2, 10, QChar('0')).arg(_title);
	}

	inline void setAlbum(const QString& album)
	{ _album = album; }
	inline void setAlbum(QString&& album)
	{ _album = std::move(album); }
	inline QString album() const
	{ return _album; }

	inline void setArtist(const QString& artist)
	{ _artist = artist; }
	inline void setArtist(QString&& artist)
	{ _artist = std::move(artist); }
	inline QString artist() const
	{ return _artist; }

	inline void setGenre(const QString& genre)
	{ _genre = genre; }
	inline void setGenre(QString&& genre)
	{ _genre = std::move(genre); }
	inline QString genre() const
	{ return _genre; }

	inline void setTrackNumer(uint32_t track_number)
	{ _track_number = track_number; }
	inline uint32_t trackNumber() const
	{ return _track_number; }

	inline void setYear(uint32_t year)
	{ _year = year; }
	inline uint32_t year() const
	{ return _year; }
	QString yearString() const
	{
		return	(_year == Global::undefined_year) ?
				QString() :
				QString::number(_year);
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
	QString _title;
	QString _album;
	QString _artist;
	QString _genre;
	uint32_t _track_number = Global::undefined_track_number;
	uint32_t _year = Global::undefined_year;
	uint32_t _play_count = 0;
	uint32_t _size = 0;
	uint32_t _time = 0;
};
