#include "Input.h"
#include "XmlParser.h"

#include <QDebug>
// #include <QElapsedTimer>

#include <algorithm>

Input::Input(QObject* parent)
	: QObject{parent}
	, _xml_parser{std::make_unique<XmlParser>()}
{
}

Input::~Input()
{
}

void Input::readFiles(const QStringList& file_names)
{
	if (file_names.size() == 1) {
		auto title = createLibraryTitleFromFileName(file_names[0]);
		auto tracks = readTracks(file_names[0]);
		auto library = createLibraryFromTracks(title, tracks);
		emit libraryReady(library);
	} else if (file_names.size() == 2) {
		auto title = createLibraryTitleFromFileNames(file_names[0], file_names[1]);
		auto tracks_1 = readTracks(file_names[0]);
		auto tracks_2 = readTracks(file_names[1]);
		auto tracks_diff = calcTracksDiff(tracks_1, tracks_2);
		auto library = createLibraryFromTracks(title, tracks_diff);
		emit libraryReady(library);
	} else {
		std::vector<Library> libraries;
		std::list<Track> prev_tracks, tracks;
		bool first_file = true;
		for (const auto& file_name : file_names) {
			if (first_file) {
				first_file = false;
				auto title = createLibraryTitleFromFileName(file_name);
				tracks = readTracks(file_name);
				libraries.emplace_back(createLibraryFromTracks(title, tracks));
			} else {
				auto title = createLibraryTitleFromFileName(file_name);
				tracks = readTracks(file_name);
				auto tracks_diff = calcTracksDiff(prev_tracks, tracks);
				libraries.emplace_back(createLibraryFromTracks(title, tracks_diff));
			}
			prev_tracks = tracks;
		}
		emit librariesReady(libraries);
	}
}

std::list<Track> Input::readTracks(const QString& file_name)
{
	return _xml_parser->readFile(file_name);
}

std::list<Track> Input::calcTracksDiff(std::list<Track> tracks_1, std::list<Track> tracks_2)
{
	for (auto& track : tracks_2) {
		auto equal_track_it = std::find(tracks_1.begin(), tracks_1.end(), track);
		if (equal_track_it != tracks_1.end()) {
			track -= *equal_track_it;
			tracks_1.erase(equal_track_it);
		}
	}
	return tracks_2;
}

QString Input::createLibraryTitleFromFileName(const QString& file_name)
{
	return file_name.split("/").last();
}

QString Input::createLibraryTitleFromFileNames(const QString& file_name_1, const QString& file_name_2)
{
	return QString("%1 → %2").arg(file_name_1.split("/").last(), file_name_2.split("/").last());
}

Library Input::createLibraryFromTracks(const QString& title, const std::list<Track>& tracks)
{
	Library library(title);

	for (const auto& track : tracks) {
		auto& artist = library[track.artist()];
		if (artist.isTitleEmpty()) {
			artist.setTitle(track.artist());
		}

		auto& album = artist[track.album()];
		if (album.isTitleEmpty()) {
			album.setTitle(track.album());
		}

		album.addTrack(track);
	}

	return library;
}
