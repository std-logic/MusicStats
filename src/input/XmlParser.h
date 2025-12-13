#pragma once

#include "common/Track.h"

#include <QString>

class XmlParser
{
public:
	XmlParser();

	std::vector<Track> readFile(const QString& file_name);
};
