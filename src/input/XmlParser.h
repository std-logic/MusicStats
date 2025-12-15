#pragma once

#include "common/Track.h"

#include <QString>

#include <list>

class XmlParser
{
public:
	XmlParser();

	std::list<Track> readFile(const QString& file_name);
};
