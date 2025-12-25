#pragma once

#include "common/Library.h"

#include <QMainWindow>
#include <QElapsedTimer>

#include <optional>

class LibraryTable;
class StatisticsChart;

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

	void statisticsChartOff(bool checked);
	void statisticsChartPlayCounts(bool checked);
	void statisticsChartYears(bool checked);

private:
	std::unique_ptr<LibraryTable> _library_table;
	std::unique_ptr<StatisticsChart> _statistics_chart;

	std::optional<Library> _library;

	QElapsedTimer _read_files_timer;
};
