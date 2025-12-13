#pragma once

#include "common/Track.h"

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
	void tracksReady(const std::vector<Track>& tracks);

private:

private:
	std::unique_ptr<XmlParser> _xml_parser;
};
