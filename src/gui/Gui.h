#pragma once

#include "common/Library.h"

#include <QObject>

class MainWindow;

class Gui : public QObject
{
	Q_OBJECT
public:
	explicit Gui(QObject* parent = nullptr);
	~Gui();

public slots:
	void show();
	void showLibrary(const Library& library);

signals:
	void readFiles(const QStringList& file_names);

private:
	void initConnections();

private:
	std::unique_ptr<MainWindow> _main_window;
};
