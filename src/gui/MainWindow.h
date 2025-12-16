#pragma once

#include "common/Library.h"

#include <QMainWindow>
#include <QElapsedTimer>

class LibraryTable;

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

	void showLibraryTitle(const QString& title);
	void clearLibraryTitle();

	void showReadingStart();
	void showReadingFinish();

private slots:
	void openFile();
	void clearAll();
	void viewByArtists(bool checked);
	void viewByAlbums(bool checked);
	void viewByTracks(bool checked);

private:
	std::unique_ptr<LibraryTable> _library_table;

	Library _library;
	bool _library_ready = false;

	QElapsedTimer _read_files_timer;
};
