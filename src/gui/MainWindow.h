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
	enum ShowModes
	{
		SHOW_LIBRARY,
		SHOW_LIBRARIES,
		NUM_OF_SHOW_MODES
	};

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void showLibrary(const Library& library);
	void showLibraries(const std::vector<Library>& libraries);

signals:
	void readFiles(const QStringList& file_names);

private:
	void initCommonParams();
	void initMenuBar();
	void initStatusBar();
	void initCentralWidgets();

	void showLibraryTitle(const Library& library);
	void showLibrariesTitle(const std::vector<Library>& libraries);
	void clearLibraryTitle();

	void showReadingStart();
	void showReadingFinish();

	void updateMenuView(ShowModes show_mode);
	void updateMenuFile(const QStringList& file_names = QStringList());
	void updateMenuFileNext(bool enabled, const QString& suffix = QString());
	void updateMenuFilePrev(bool enabled, const QString& suffix = QString());

private slots:
	void openFile();
	void openNextFile();
	void openPrevFile();
	void clearAll();

	void viewByArtists(bool checked);
	void viewByAlbums(bool checked);
	void viewByTracks(bool checked);
	void viewByBestTracks(bool checked);
	void viewByGenres(bool checked);
	void viewBySummary(bool checked);
	void viewByHistory(bool checked);

	void statisticsChartOff(bool checked);
	void statisticsChartPlayCounts(bool checked);
	void statisticsChartYears(bool checked);
	void statisticsChartHistoryPlayCounts(bool checked);
	void statisticsChartHistoryArtists(bool checked);
	void statisticsChartHistoryAlbums(bool checked);
	void statisticsChartHistoryTracks(bool checked);

private:
	std::unique_ptr<LibraryTable> _library_table;
	std::unique_ptr<StatisticsChart> _statistics_chart;

	std::optional<Library> _library;
	std::optional<std::vector<Library>> _libraries;

	QAction* _menu_file_action_next = nullptr;
	QAction* _menu_file_action_prev = nullptr;
	QString _menu_file_path;
	QStringList _menu_file_entry_list;
	qsizetype _menu_file_curr_pos = -1;

	ShowModes _show_mode = NUM_OF_SHOW_MODES;
	std::vector<QAction*> _menu_view_actions[NUM_OF_SHOW_MODES];
	QAction* _default_menu_view_by[NUM_OF_SHOW_MODES];
	QAction* _default_menu_statistics_chart[NUM_OF_SHOW_MODES];

	QElapsedTimer _read_files_timer;
};
