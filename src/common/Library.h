#pragma once

#include "Global.h"
#include "Helper.h"
#include "Artist.h"

#include <QString>

#include <map>

class Library
{
public:
	Library() = default;
	Library(const QString& title) : _title(title) {}

	using ArtistsContainer = std::map<QString, Artist>;

	ArtistsContainer::iterator begin() noexcept
	{ return _artists.begin(); }
	ArtistsContainer::const_iterator begin() const noexcept
	{ return _artists.begin(); }

	ArtistsContainer::iterator end() noexcept
	{ return _artists.end(); }
	ArtistsContainer::const_iterator end() const noexcept
	{ return _artists.end(); }

	Artist& operator[](const QString& title)
	{ return _artists[title]; }

	inline void setTitle(const QString& title)
	{ _title = title; }
	inline QString title() const
	{ return _title; }
	inline QString titleOnlyDate() const
	{
		QString title_cropped = _title;
		title_cropped.remove(QStringLiteral("winamp_")).remove(QStringLiteral(".xml"));
		return title_cropped;
	}

	auto minYear() const
	{
		auto min_year = Global::undefined_year;
		for (const auto& item : _artists) {
			min_year = Helper::checkMinYear(min_year, item.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = Global::undefined_year;
		for (const auto& item : _artists) {
			max_year = Helper::checkMaxYear(max_year, item.second.maxYear());
		}
		return max_year;
	}
	QString yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	uint32_t playCount() const
	{
		uint32_t sum = 0;
		for (const auto& item : _artists) { sum += item.second.playCount(); }
		return sum;
	}

	uint64_t size() const
	{
		auto sum = uint64_t(0);
		for (const auto& item : _artists) { sum += item.second.size(); }
		return sum;
	}

	uint64_t time() const
	{
		auto sum = uint64_t(0);
		for (const auto& item : _artists) { sum += item.second.time(); }
		return sum;
	}

	auto artistsCount() const
	{ return _artists.size(); }

	auto albumsCount() const
	{
		size_t sum = 0;
		for (const auto& item : _artists) { sum += item.second.albumsCount(); }
		return sum;
	}

	auto tracksCount() const
	{
		size_t sum = 0;
		for (const auto& item : _artists) { sum += item.second.tracksCount(); }
		return sum;
	}

	using TopArtistsData = std::pair<const Artist*, uint32_t>;
	std::vector<TopArtistsData> topArtists(int top_size) const
	{
		std::vector<TopArtistsData> artists_list;
		for (const auto& [artist_title, artist] : _artists) {
			artists_list.emplace_back(TopArtistsData{&artist, artist.playCount()});
		}
		std::sort(artists_list.begin(), artists_list.end(),
				  [](const TopArtistsData& a, const TopArtistsData& b)
				  {return (a.second > b.second);});
		return std::vector<TopArtistsData>{artists_list.begin(), artists_list.begin()+top_size};
	}

	using TopAlbumsData = std::pair<const Album*, uint32_t>;
	std::vector<TopAlbumsData> topAlbums(int top_size) const
	{
		std::vector<TopAlbumsData> albums_list;
		for (const auto& [artist_title, artist] : _artists) {
			for (const auto& [album_title, album] : artist) {
				albums_list.emplace_back(TopAlbumsData{&album, album.playCount()});
			}
		}
		std::sort(albums_list.begin(), albums_list.end(),
				  [](const TopAlbumsData& a, const TopAlbumsData& b)
				  {return (a.second > b.second);});
		return std::vector<TopAlbumsData>{albums_list.begin(), albums_list.begin()+top_size};
	}

	using TopTracksData = std::pair<const Track*, uint32_t>;
	std::vector<TopTracksData> topTracks(int top_size) const
	{
		std::vector<TopTracksData> tracks_list;
		for (const auto& [artist_title, artist] : _artists) {
			for (const auto& [album_title, album] : artist) {
				for (const auto& track : album) {
					tracks_list.emplace_back(TopTracksData{&track, track.playCount()});
				}
			}
		}
		std::sort(tracks_list.begin(), tracks_list.end(),
				  [](const TopTracksData& a, const TopTracksData& b)
				  {return (a.second > b.second);});
		return std::vector<TopTracksData>{tracks_list.begin(), tracks_list.begin()+top_size};
	}

	std::tuple<int, int, int> playedCount() const
	{
		int played_artists = 0, played_albums = 0, played_tracks = 0;
		for (const auto& [artist_title, artist] : _artists) {
			bool played_artist_found = false;
			for (const auto& [album_title, album] : artist) {
				bool played_album_found = false;
				for (const auto& track : album) {
					if (track.playCount()) {
						if (!played_artist_found) {
							played_artist_found = true;
							++played_artists;
						}
						if (!played_album_found) {
							played_album_found = true;
							++played_albums;
						}
						++played_tracks;
					}
				}
			}
		}
		return std::make_tuple(played_artists, played_albums, played_tracks);
	}

	using GenresTracksData = std::vector<const Track*>;
	using GenresAlbumsData = std::map<QString, GenresTracksData>;
	using GenresArtistsData = std::map<QString, GenresAlbumsData>;
	using GenresData = std::map<QString, std::pair<GenresArtistsData, int>>;
	GenresData genres() const
	{
		GenresData genres;
		for (const auto& [artist_title, artist] : _artists) {
			for (const auto& [album_title, album] : artist) {
				for (const auto& track : album) {
					auto genre_title = track.genre();
					auto& item = genres[genre_title];
					item.first[track.artist()][track.album()].push_back(&track);
					item.second++;
					if (genre_title.contains(QStringLiteral("Rock"))) {
						auto& item_rock = genres[QStringLiteral("* Rock")];
						item_rock.first[track.artist()][track.album()].push_back(&track);
						item_rock.second++;
					} else if (genre_title.contains(QStringLiteral("Metal"))) {
						auto& item_metal = genres[QStringLiteral("* Metal")];
						item_metal.first[track.artist()][track.album()].push_back(&track);
						item_metal.second++;
					}
				}
			}
		}
		return genres;
	}

	QString fullInfo() const
	{
		QString text;
		text += QString("Файл: %1").arg(_title);
		text += QString("\nГод: %1").arg(yearString());
		text += QString("\nГрупп: %1").arg(artistsCount());
		text += QString("\nАльбомов: %1").arg(albumsCount());
		text += QString("\nТреков: %1").arg(tracksCount());
		text += QString("\nПрослушиваний: %1").arg(playCount());
		text += QString("\nДлина: %1").arg(Helper::timeString(time()));
		text += QString("\nРазмер: %1").arg(Helper::sizeString(size()));
		return text;
	}

private:
	QString _title;
	ArtistsContainer _artists;
};
