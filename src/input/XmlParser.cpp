#include "XmlParser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

std::list<Track> XmlParser::readFile(const QString& file_name)
{
	auto tracks = std::list<Track>();
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
						curr_track.setTitle(xml.readElementText());
					}
				} else if (element_name == "Artist") {
					if (xml.readNextStartElement()) {
						curr_track.setArtist(xml.readElementText());
					}
				} else if (element_name == "Album") {
					if (xml.readNextStartElement()) {
						curr_track.setAlbum(xml.readElementText());
					}
				} else if (element_name == "Genre") {
					if (xml.readNextStartElement()) {
						curr_track.setGenre(xml.readElementText());
					}
				} else if (element_name == "Comments") {
					if (xml.readNextStartElement()) {
						curr_track.setComments(xml.readElementText());
					}
				} else if (element_name == "Size") {
					if (xml.readNextStartElement()) {
						curr_track.setSize(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Size!"; }
					}
				} else if (element_name == "Total Time") {
					if (xml.readNextStartElement()) {
						curr_track.setTime(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Total Time!"; }
					}
				} else if (element_name == "Track Number") {
					if (xml.readNextStartElement()) {
						curr_track.setTrackNumer(xml.readElementText().toInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Track Number!"; }
					}
				} else if (element_name == "Year") {
					if (xml.readNextStartElement()) {
						curr_track.setYear(xml.readElementText().toUInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Year!"; }
					}
				} else if (element_name == "Bit Rate") {
					if (xml.readNextStartElement()) {
						curr_track.setBitrate(xml.readElementText().toUInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Bit Rate!"; }
					}
				} else if (element_name == "Play Count") {
					if (xml.readNextStartElement()) {
						curr_track.setPlayCount(xml.readElementText().toUInt(&conv_ok));
						if (!conv_ok) { qDebug() << "Wrong Play Count!"; }
					}
				} else if (element_name == "Composer") {
					if (xml.readNextStartElement()) {
						curr_track.setComposer(xml.readElementText());
					}
				} else if (element_name == "Publisher") {
					if (xml.readNextStartElement()) {
						curr_track.setPublisher(xml.readElementText());
					}
				} else if (element_name == "Location") {
					if (xml.readNextStartElement()) {
						QString location = xml.readElementText();
						auto format_start = location.lastIndexOf('.') + 1;
						curr_track.setFormat(location.sliced(format_start));
					}
				}
			}
		}
	}

	file.close();
	return tracks;
}
