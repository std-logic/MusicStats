#pragma once

#include "Global.h"
#include "Helper.h"
#include "Album.h"

#include <QString>

#include <map>

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

	inline QString genre() const
	{
		if (_albums.empty()) { return QString(); }
		QString genre_first = _albums.begin()->second.genre();
		QString genre_last = _albums.rbegin()->second.genre();
		return (genre_first == genre_last) ? genre_first : QString("[Разные]");
	}

	auto minYear() const
	{
		auto min_year = Global::undefined_year;
		for (const auto& item : _albums) {
			min_year = Helper::checkMinYear(min_year, item.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = Global::undefined_year;
		for (const auto& item : _albums) {
			max_year = Helper::checkMaxYear(max_year, item.second.maxYear());
		}
		return max_year;
	}
	QString yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	uint32_t playCount() const
	{
		uint32_t sum = 0;
		for (const auto& item : _albums) { sum += item.second.playCount(); }
		return sum;
	}

	uint64_t size() const
	{
		auto sum = uint64_t(0);
		for (const auto& item : _albums) { sum += item.second.size(); }
		return sum;
	}

	uint64_t time() const
	{
		auto sum = uint64_t(0);
		for (const auto& item : _albums) { sum += item.second.time(); }
		return sum;
	}

	auto albumsCount() const
	{ return _albums.size(); }

	auto tracksCount() const
	{
		size_t sum = 0;
		for (const auto& item : _albums) { sum += item.second.tracksCount(); }
		return sum;
	}

	QString fullInfo() const
	{
		QString text;
		text += QString("Группа: %1").arg(_title);
		text += QString("\nГод: %1").arg(yearString());
		text += QString("\nАльбомов: %1").arg(albumsCount());
		text += QString("\nТреков: %1").arg(tracksCount());
		text += QString("\nПрослушиваний: %1").arg(playCount());
		text += QString("\nЖанр: %1").arg(genre());
		text += QString("\nДлина: %1").arg(Helper::timeString(time()));
		text += QString("\nРазмер: %1").arg(Helper::sizeString(size()));
		return text;
	}

private:
	QString _title;
	AlbumsContainer _albums;
};
