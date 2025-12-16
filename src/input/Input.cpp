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
	auto tracks = (file_names.size() == 1) ?
					readTracks(file_names[0]) :
					readTracksDiff(file_names[0], file_names[1]);
	auto title = createLibraryTitleFromFileNames(file_names);
	auto library = createLibraryFromTracks(title, tracks);
	emit libraryReady(library);
}

std::list<Track> Input::readTracks(const QString& file_name)
{
	return _xml_parser->readFile(file_name);
}

std::list<Track> Input::readTracksDiff(const QString& file_name_1, const QString& file_name_2)
{
	auto tracks_1 = _xml_parser->readFile(file_name_1);
	auto tracks_2 = _xml_parser->readFile(file_name_2);
	for (auto& track : tracks_2) {
		auto equal_track_it = std::find(tracks_1.begin(), tracks_1.end(), track);
		if (equal_track_it != tracks_1.end()) {
			track -= *equal_track_it;
			tracks_1.erase(equal_track_it);
		}
	}
	return tracks_2;
}

QString Input::createLibraryTitleFromFileNames(const QStringList& file_names)
{
	if (file_names.size() == 1) {
		return file_names[0].split("/").last();
	} else {
		return QString("%1 → %2")
				.arg(file_names[0].split("/").last(),
					 file_names[1].split("/").last());
	}
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
