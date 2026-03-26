#pragma once

#include "common/Library.h"

#include <QObject>

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
	void librariesReady(const std::vector<Library>& libraries);

private:
	std::list<Track> readTracks(const QString& file_name);
	std::list<Track> calcTracksDiff(std::list<Track> tracks_1, std::list<Track> tracks_2);
	QString createLibraryTitleFromFileName(const QString& file_name);
	QString createLibraryTitleFromFileNames(const QString& file_name_1, const QString& file_name_2);
	Library createLibraryFromTracks(const QString& title, const std::list<Track>& tracks);

private:
	std::unordered_map<QString, std::list<Track>> _tracks_cache;
};
