#pragma once

#include "common/Library.h"

#include <QObject>

class XmlParser;

class Input : public QObject
{
	Q_OBJECT
public:
	explicit Input(QObject* parent = nullptr);
	~Input();

public slots:
	void readFiles(const QStringList& file_names);

signals:
	void libraryReady(const Library& library);

private:
	std::list<Track> readTracks(const QString& file_name);
	std::list<Track> readTracksDiff(const QString& file_name_1, const QString& file_name_2);
	QString createLibraryTitleFromFileNames(const QStringList& file_names);
	Library createLibraryFromTracks(const QString& title, const std::list<Track>& tracks);

private:
	std::unique_ptr<XmlParser> _xml_parser;
};
