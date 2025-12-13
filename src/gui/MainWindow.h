#pragma once

#include "common/Library.h"

#include <QMainWindow>

class ArtistsTable;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void showLibrary(const Library& library);

signals:
	void readFiles(const QStringList& file_names);

private:
	void initCommonParams();
	void initMenuBar();
	void initStatusBar();
	void initCentralWidgets();

private slots:
	void openFile();
	void clearAll();

private:
	std::unique_ptr<ArtistsTable> _artists_table;
};
