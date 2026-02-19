#pragma once

#include "Global.h"
#include "Helper.h"

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

	inline void setComposer(const QString& composer)
	{ _composer = composer; }
	inline void setComposer(QString&& composer)
	{ _composer = std::move(composer); }
	inline QString composer() const
	{ return _composer; }

	inline void setFormat(const QString& format)
	{ _format = format; }
	inline void setFormat(QString&& format)
	{ _format = std::move(format); }
	inline QString format() const
	{ return _format; }

	inline void setPublisher(const QString& publisher)
	{ _publisher = publisher; }
	inline void setPublisher(QString&& publisher)
	{ _publisher = std::move(publisher); }
	inline QString publisher() const
	{ return _publisher; }

	inline void setComments(const QString& comments)
	{ _comments = comments; }
	inline void setComments(QString&& comments)
	{ _comments = std::move(comments); }
	inline QString comments() const
	{ return _comments; }

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

	inline void setBitrate(uint32_t bitrate)
	{ _bitrate = bitrate; }
	inline uint32_t bitrate() const
	{ return _bitrate; }

	QString fullInfo() const
	{
		QString text;
		text += QString("Трек: %1").arg(_title);
		if (_year != Global::undefined_year) {
			text += QString("\nГод: %1").arg(yearString());
		}
		text += QString("\nПрослушиваний: %1").arg(_play_count);
		text += QString("\nЖанр: %1").arg(_genre);
		text += QString("\nДлина: %1").arg(Helper::timeString(_time));
		text += QString("\nРазмер: %1").arg(Helper::sizeString(_size));
		text += QString("\nБитрейт: %1 кбит/с (%2)").arg(_bitrate).arg(_format);
		if (!_composer.isEmpty()) {
			text += QString("\nКомпозитор: %1").arg(_composer);
		}
		if (!_publisher.isEmpty()) {
			text += QString("\nИздатель: %1").arg(_publisher);
		}
		if (!_comments.isEmpty()) {
			text += QString("\n\n%1").arg(_comments);
		}
		return text;
	}

private:
	QString _title;
	QString _album;
	QString _artist;
	QString _genre;
	QString _composer;
	QString _format;
	QString _publisher;
	QString _comments;
	uint32_t _track_number = Global::undefined_track_number;
	uint32_t _year = Global::undefined_year;
	uint32_t _play_count = 0;
	uint32_t _size = 0;
	uint32_t _time = 0;
	uint32_t _bitrate = 0;
};
