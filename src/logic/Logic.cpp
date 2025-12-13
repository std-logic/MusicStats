#include "Logic.h"

#include <QDebug>
#include <QElapsedTimer>

Logic::Logic(QObject* parent)
	: QObject{parent}
{
}

Logic::~Logic()
{
}

void Logic::processTracks(const std::vector<Track>& tracks)
{
	Library library;
	// library.title = "My library";

	QElapsedTimer el_timer;
	el_timer.start();

	for (const auto& track : tracks) {
		auto artist_title = track.artist();
		// if (artist_title == "Разное") {
		// 	artist_title = track.album();
		// }

		auto& artist = library/*.artists*/[artist_title];
		if (artist.isTitleEmpty()) {
			artist.setTitle(artist_title);
		}

		auto& album = artist/*.albums*/[track.album()];
		if (album.isTitleEmpty()) {
			album.setTitle(track.album());
		}
		album.addTrack(track);
	}

	// qDebug() << "found" << library/*.artists*/.artistsCount() << "artists for" << el_timer.elapsed() << "ms";
	// qDebug() << "albums in library" << library.albumsCount();
	// qDebug() << "tracks in library" << library.tracksCount();

	emit libraryReady(library);
}
