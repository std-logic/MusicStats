#include "MainWindow.h"
#include "LibraryTable.h"
#include "StatisticsChart.h"

#include <QMenuBar>
#include <QActionGroup>
#include <QStatusBar>
#include <QFileDialog>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow{parent}
	, _library_table{std::make_unique<LibraryTable>(this)}
	, _statistics_chart{std::make_unique<StatisticsChart>(this)}
{
	initCommonParams();
	initMenuBar();
	initStatusBar();
	initCentralWidgets();

	clearLibraryTitle();
	updateMenuBar(SHOW_LIBRARY);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLibrary(const Library& library)
{
	_libraries.reset();
	updateMenuBar(SHOW_LIBRARY);
	_library = library;
	_library_table->showLibrary(library);
	_statistics_chart->showStatistics(library);
	showLibraryTitle(library);
	showReadingFinish();
}

void MainWindow::showLibraries(const std::vector<Library>& libraries)
{
	_library.reset();
	updateMenuBar(SHOW_LIBRARIES);
	_libraries = libraries;
	_library_table->showLibraries(libraries);
	_statistics_chart->showStatistics(libraries);
	showLibrariesTitle(libraries);
	showReadingFinish();
}

void MainWindow::initCommonParams()
{
	setMinimumSize(QSize(960, 540));
}

void MainWindow::initMenuBar()
{
	setMenuBar(new QMenuBar);

	// Файл
	{
		auto menu_file = menuBar()->addMenu(tr("Файл"));

		// Файл -> Открыть...
		menu_file->addAction(
					QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
					tr("Открыть..."),
					QKeySequence(Qt::CTRL | Qt::Key_O),
					this,
					&MainWindow::openFile);

		// Файл -> Очистить
		menu_file->addAction(
					QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh),
					tr("Очистить"),
					QKeySequence(Qt::CTRL | Qt::Key_R),
					this,
					&MainWindow::clearAll);
	}

	// Вид
	{
		auto menu_view = menuBar()->addMenu(tr("Вид"));

		auto group_view_by = new QActionGroup(this);
		group_view_by->setExclusive(true);

		// Вид -> По группам
		auto action_view_by_artists = menu_view->addAction(
					tr("По группам"),
					QKeySequence(Qt::CTRL | Qt::Key_1),
					this,
					&MainWindow::viewByArtists);
		action_view_by_artists->setCheckable(true);
		action_view_by_artists->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_artists);
		_default_menu_view_by[SHOW_LIBRARY] = action_view_by_artists;

		// Вид -> По альбомам
		auto action_view_by_albums = menu_view->addAction(
					tr("По альбомам"),
					QKeySequence(Qt::CTRL | Qt::Key_2),
					this,
					&MainWindow::viewByAlbums);
		action_view_by_albums->setCheckable(true);
		action_view_by_albums->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_albums);

		// Вид -> По трекам
		auto action_view_by_tracks = menu_view->addAction(
					tr("По трекам"),
					QKeySequence(Qt::CTRL | Qt::Key_3),
					this,
					&MainWindow::viewByTracks);
		action_view_by_tracks->setCheckable(true);
		action_view_by_tracks->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_tracks);

		// Вид -> По лучшим трекам
		auto action_view_by_best_tracks = menu_view->addAction(
					tr("По лучшим трекам"),
					QKeySequence(Qt::CTRL | Qt::Key_4),
					this,
					&MainWindow::viewByBestTracks);
		action_view_by_best_tracks->setCheckable(true);
		action_view_by_best_tracks->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_best_tracks);

		// Вид -> По жанрам
		auto action_view_by_genres = menu_view->addAction(
					tr("По жанрам"),
					QKeySequence(Qt::CTRL | Qt::Key_5),
					this,
					&MainWindow::viewByGenres);
		action_view_by_genres->setCheckable(true);
		action_view_by_genres->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_genres);

		// Вид -> Сводка
		auto action_view_by_summary = menu_view->addAction(
					tr("Сводка"),
					QKeySequence(Qt::CTRL | Qt::Key_6),
					this,
					&MainWindow::viewBySummary);
		action_view_by_summary->setCheckable(true);
		action_view_by_summary->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARY].push_back(action_view_by_summary);

		// Вид -> История
		auto action_view_by_history = menu_view->addAction(
					tr("История"),
					QKeySequence(Qt::CTRL | Qt::Key_7),
					this,
					&MainWindow::viewByHistory);
		action_view_by_history->setCheckable(true);
		action_view_by_history->setActionGroup(group_view_by);
		_menu_actions[SHOW_LIBRARIES].push_back(action_view_by_history);
		_default_menu_view_by[SHOW_LIBRARIES] = action_view_by_history;

		menu_view->addSeparator();

		auto group_statistics_chart = new QActionGroup(this);
		group_statistics_chart->setExclusive(true);

		// Вид -> Без графиков
		auto action_statistics_chart_off = menu_view->addAction(
					tr("Без графиков"),
					QKeySequence(Qt::ALT | Qt::Key_1),
					this,
					&MainWindow::statisticsChartOff);
		action_statistics_chart_off->setCheckable(true);
		action_statistics_chart_off->setActionGroup(group_statistics_chart);
		_default_menu_statistics_chart[SHOW_LIBRARY] = action_statistics_chart_off;
		_default_menu_statistics_chart[SHOW_LIBRARIES] = action_statistics_chart_off;

		// Вид -> По прослушиваниям
		auto action_statistics_chart_play_counts = menu_view->addAction(
					tr("По прослушиваниям"),
					QKeySequence(Qt::ALT | Qt::Key_2),
					this,
					&MainWindow::statisticsChartPlayCounts);
		action_statistics_chart_play_counts->setCheckable(true);
		action_statistics_chart_play_counts->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARY].push_back(action_statistics_chart_play_counts);

		// Вид -> По годам
		auto action_statistics_chart_years = menu_view->addAction(
					tr("По годам"),
					QKeySequence(Qt::ALT | Qt::Key_3),
					this,
					&MainWindow::statisticsChartYears);
		action_statistics_chart_years->setCheckable(true);
		action_statistics_chart_years->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARY].push_back(action_statistics_chart_years);

		// Вид -> Прирост прослушиваний
		auto action_statistics_chart_history_play_counts = menu_view->addAction(
					tr("Прирост прослушиваний"),
					QKeySequence(Qt::ALT | Qt::Key_4),
					this,
					&MainWindow::statisticsChartHistoryPlayCounts);
		action_statistics_chart_history_play_counts->setCheckable(true);
		action_statistics_chart_history_play_counts->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARIES].push_back(action_statistics_chart_history_play_counts);

		// Вид -> Прирост групп
		auto action_statistics_chart_history_artists = menu_view->addAction(
					tr("Прирост групп"),
					QKeySequence(Qt::ALT | Qt::Key_5),
					this,
					&MainWindow::statisticsChartHistoryArtists);
		action_statistics_chart_history_artists->setCheckable(true);
		action_statistics_chart_history_artists->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARIES].push_back(action_statistics_chart_history_artists);

		// Вид -> Прирост альбомов
		auto action_statistics_chart_history_albums = menu_view->addAction(
					tr("Прирост альбомов"),
					QKeySequence(Qt::ALT | Qt::Key_6),
					this,
					&MainWindow::statisticsChartHistoryAlbums);
		action_statistics_chart_history_albums->setCheckable(true);
		action_statistics_chart_history_albums->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARIES].push_back(action_statistics_chart_history_albums);

		// Вид -> Прирост треков
		auto action_statistics_chart_history_tracks = menu_view->addAction(
					tr("Прирост треков"),
					QKeySequence(Qt::ALT | Qt::Key_7),
					this,
					&MainWindow::statisticsChartHistoryTracks);
		action_statistics_chart_history_tracks->setCheckable(true);
		action_statistics_chart_history_tracks->setActionGroup(group_statistics_chart);
		_menu_actions[SHOW_LIBRARIES].push_back(action_statistics_chart_history_tracks);
	}
}

void MainWindow::initStatusBar()
{
	setStatusBar(new QStatusBar);
}

void MainWindow::initCentralWidgets()
{
	auto central_widget = new QWidget(this);
	auto central_layout = new QVBoxLayout(central_widget);

	central_layout->setContentsMargins(0, 0, 0, 0);
	central_layout->setSpacing(0);
	central_layout->addWidget(_library_table.get(), 60);
	central_layout->addWidget(_statistics_chart.get(), 40);

	setCentralWidget(central_widget);
}

void MainWindow::showLibraryTitle(const Library& library)
{
	setWindowTitle(QString("%1 – %2 – %3: %4 – %5: %6 – %7: %8")
				   .arg(tr("Статистика музыки"),
						library.titleOnlyDate(),
						tr("Групп"))
				   .arg(library.artistsCount())
				   .arg(tr("Альбомов"))
				   .arg(library.albumsCount())
				   .arg(tr("Треков"))
				   .arg(library.tracksCount()));
}

void MainWindow::showLibrariesTitle(const std::vector<Library>& libraries)
{
	setWindowTitle(QString("%1 – %2 → %3 – %4: %5")
				   .arg(tr("Статистика музыки"),
						libraries.front().titleOnlyDate(),
						libraries.back().titleOnlyDate(),
						tr("Файлов"))
				   .arg(libraries.size()));
}

void MainWindow::clearLibraryTitle()
{
	setWindowTitle(tr("Статистика музыки"));
}

void MainWindow::showReadingStart()
{
	_read_files_timer.start();
}

void MainWindow::showReadingFinish()
{
	statusBar()->showMessage(tr("Библиотека загружена за %1 мс")
							 .arg(_read_files_timer.elapsed()), 10000);
}

void MainWindow::updateMenuBar(ShowModes show_mode)
{
	if (_show_mode == show_mode) {
		return;
	}
	_show_mode = show_mode;

	for (auto action : _menu_actions[SHOW_LIBRARY]) {
		action->setEnabled(show_mode == SHOW_LIBRARY);
	}
	for (auto action : _menu_actions[SHOW_LIBRARIES]) {
		action->setEnabled(show_mode == SHOW_LIBRARIES);
	}

	_default_menu_view_by[show_mode]->trigger();
	_default_menu_statistics_chart[show_mode]->trigger();
}

void MainWindow::openFile()
{
	auto file_names = QFileDialog::getOpenFileNames(
				this,
				tr("Выберите один или несколько файлов для анализа"),
				QString(),
				QString("XML files (*.xml)"));

	if (file_names.isEmpty()) {
		return;
	}

	file_names.sort();
	showReadingStart();
	emit readFiles(file_names);
}

void MainWindow::clearAll()
{
	clearLibraryTitle();
	_library_table->clearLibrary();
	_statistics_chart->clearStatistics();
	_library.reset();
	_libraries.reset();
	updateMenuBar(SHOW_LIBRARY);
}

void MainWindow::viewByArtists(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_ARTISTS)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewByAlbums(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_ALBUMS)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewByTracks(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_TRACKS)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewByBestTracks(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_BEST_TRACKS)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewByGenres(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_GENRES)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewBySummary(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_SUMMARY)) {
		if (_library.has_value()) {
			_library_table->showLibrary(_library.value());
		}
	}
}

void MainWindow::viewByHistory(bool /*checked*/)
{
	if (_library_table->setViewByType(LibraryTable::VIEW_BY_HISTORY)) {
		if (_libraries.has_value()) {
			_library_table->showLibraries(_libraries.value());
		}
	}
}

void MainWindow::statisticsChartOff(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_OFF)) {
		_statistics_chart->clearStatistics();
	}
}

void MainWindow::statisticsChartPlayCounts(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_PLAY_COUNTS)) {
		if (_library.has_value()) {
			_statistics_chart->showStatistics(_library.value());
		}
	}
}

void MainWindow::statisticsChartYears(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_YEARS)) {
		if (_library.has_value()) {
			_statistics_chart->showStatistics(_library.value());
		}
	}
}

void MainWindow::statisticsChartHistoryPlayCounts(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_HISTORY_PLAY_COUNTS)) {
		if (_libraries.has_value()) {
			_statistics_chart->showStatistics(_libraries.value());
		}
	}
}

void MainWindow::statisticsChartHistoryArtists(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_HISTORY_ARTISTS)) {
		if (_libraries.has_value()) {
			_statistics_chart->showStatistics(_libraries.value());
		}
	}
}

void MainWindow::statisticsChartHistoryAlbums(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_HISTORY_ALBUMS)) {
		if (_libraries.has_value()) {
			_statistics_chart->showStatistics(_libraries.value());
		}
	}
}

void MainWindow::statisticsChartHistoryTracks(bool /*checked*/)
{
	if (_statistics_chart->setStatisticsType(StatisticsChart::STATISTICS_HISTORY_TRACKS)) {
		if (_libraries.has_value()) {
			_statistics_chart->showStatistics(_libraries.value());
		}
	}
}
