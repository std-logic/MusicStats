#include "XmlParser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QElapsedTimer>

XmlParser::XmlParser()
{
}

std::vector<Track> XmlParser::readFile(const QString& file_name)
{
	auto tracks = std::vector<Track>();
	// tracks.reserve(10000);
	auto curr_track = Track();

	QFile file(file_name);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		return tracks;
	}

	bool found_tracks_list_key = false;
	bool found_tracks_list_dict = false;
	bool found_track_key = false;
	bool found_track_dict = false;
	bool conv_ok;

	QElapsedTimer el_timer;
	el_timer.start();

	// int i = 0;
	QXmlStreamReader xml(&file);
	while (!xml.atEnd()) {
		if (!found_tracks_list_key) {
			xml.readNext();
			if (xml.isCharacters() && (xml.text() == u"Tracks")) {
				found_tracks_list_key = true;
			}
		} else if (!found_tracks_list_dict) {
			if (xml.readNextStartElement() && (xml.name() == u"dict")) {
				found_tracks_list_dict = true;
			}
		} else if (!found_track_key) {
			if (xml.readNextStartElement() && (xml.name() == u"key")) {
				found_track_key = true;
			}
		} else if (!found_track_dict) {
			if (xml.readNextStartElement() && (xml.name() == u"dict")) {
				found_track_dict = true;
				curr_track = Track();
			}
		} else {
			xml.readNext();
			if (xml.isEndElement() && (xml.name() == u"dict")) {
				tracks.push_back(curr_track);
				found_track_dict = false;
				found_track_key = false;
			} else if (xml.isStartElement() && (xml.name() == u"key")) {
				QString element_name = xml.readElementText();
				if (element_name == "Name") {
					if (xml.readNextStartElement()) {
						curr_track.setTitle(xml.readElementText().toStdString());
					}
				} else if (element_name == "Artist") {
					if (xml.readNextStartElement()) {
						curr_track.setArtist(xml.readElementText().toStdString());
					}
				} else if (element_name == "Album") {
					if (xml.readNextStartElement()) {
						curr_track.setAlbum(xml.readElementText().toStdString());
					}
				} else if (element_name == "Size") {
					if (xml.readNextStartElement()) {
						curr_track.setSize(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) {
							qDebug() << "Wrong Size!";
						}
					}
				} else if (element_name == "Total Time") {
					if (xml.readNextStartElement()) {
						curr_track.setTime(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) {
							qDebug() << "Wrong Total Time!";
						}
					}
				} else if (element_name == "Track Number") {
					if (xml.readNextStartElement()) {
						curr_track.setTrackNumer(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) {
							qDebug() << "Wrong Track Number!";
						}
					}
				} else if (element_name == "Year") {
					if (xml.readNextStartElement()) {
						curr_track.setYear(xml.readElementText().toUInt(&conv_ok));
						if (!conv_ok) {
							qDebug() << "Wrong Year!";
						}
					}
				} else if (element_name == "Play Count") {
					if (xml.readNextStartElement()) {
						curr_track.setPlayCount(xml.readElementText().toUInt(&conv_ok));
						if (!conv_ok) {
							qDebug() << "Wrong Play Count!";
						}
					}
				}
			}
		}

		// if (++i > 500) {
		// 	break;
		// }
	}

	qDebug() << "found" << tracks.size() << "tracks for" << el_timer.elapsed() << "ms";
	// qDebug() << "first track" << tracks[0].artist() << tracks[0].album() << tracks[0].title();
	// size_t sum_size = 0;
	// size_t sum_time = 0;
	// for (const auto& track : tracks) {
	// 	sum_size += track.size();
	// 	sum_time += track.time();
	// }
	// qDebug() << "sum size" << static_cast<double>(sum_size) / 1024. / 1024. / 1024. << "GB";
	// qDebug() << "sum time" << static_cast<double>(sum_time) / 1000. / 60. / 60. / 24 << "d";

	file.close();
	return tracks;
}
