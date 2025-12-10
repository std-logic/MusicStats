#pragma once

#include <QObject>

class Logic : public QObject
{
	Q_OBJECT
public:
	explicit Logic(QObject* parent = nullptr);
	~Logic();

signals:

};
