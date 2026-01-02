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

	auto minYear() const
	{
		auto min_year = Global::undefined_year;
		for (const auto& artist : _artists) {
			min_year = Helper::checkMinYear(min_year, artist.second.minYear());
		}
		return min_year;
	}
	auto maxYear() const
	{
		auto max_year = Global::undefined_year;
		for (const auto& artist : _artists) {
			max_year = Helper::checkMaxYear(max_year, artist.second.maxYear());
		}
		return max_year;
	}
	QString yearString() const
	{ return Helper::yearString(minYear(), maxYear()); }

	uint32_t playCount() const
	{
		uint32_t sum = 0;
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

private:
	QString _title;
	ArtistsContainer _artists;
};
