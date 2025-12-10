#pragma once

#include <QObject>

class Input : public QObject
{
	Q_OBJECT
public:
	explicit Input(QObject* parent = nullptr);
	~Input();

signals:

};
