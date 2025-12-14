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

	// qDebug() << "found" << library/*.artists*/.artistsCount() << "artists for" << el_timer.elapsed() << "ms";
	// qDebug() << "albums in library" << library.albumsCount();
	// qDebug() << "tracks in library" << library.tracksCount();

	emit libraryReady(library);
}
