#pragma once

#include "common/Library.h"

#include <QObject>

class Logic : public QObject
{
	Q_OBJECT
public:
	explicit Logic(QObject* parent = nullptr);
	~Logic();

public slots:
	void processTracks(const std::vector<Track>& tracks);

signals:
	void libraryReady(const Library& library);
};
