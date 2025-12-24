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
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLibrary(const Library& library)
{
	_library = library;
	_library_table->showLibrary(library);
	_statistics_chart->showStatistics(library);
	showLibraryTitle(library.title());
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
					QKeySequence(Qt::CTRL | Qt::Key_C),
					this,
					&MainWindow::clearAll);
	}

	// Вид
	{
		auto menu_view = menuBar()->addMenu(tr("Вид"));

		auto group_view_by = new QActionGroup(this);
		group_view_by->setExclusive(true);

		// Вид -> По исполнителям
		auto action_view_by_artists = menu_view->addAction(
					tr("По исполнителям"),
					QKeySequence(Qt::CTRL | Qt::Key_1),
					this,
					&MainWindow::viewByArtists);
		action_view_by_artists->setCheckable(true);
		action_view_by_artists->setChecked(true);
		action_view_by_artists->setActionGroup(group_view_by);

		// Вид -> По альбомам
		auto action_view_by_albums = menu_view->addAction(
					tr("По альбомам"),
					QKeySequence(Qt::CTRL | Qt::Key_2),
					this,
					&MainWindow::viewByAlbums);
		action_view_by_albums->setCheckable(true);
		action_view_by_albums->setActionGroup(group_view_by);

		// Вид -> По трекам
		auto action_view_by_tracks = menu_view->addAction(
					tr("По трекам"),
					QKeySequence(Qt::CTRL | Qt::Key_3),
					this,
					&MainWindow::viewByTracks);
		action_view_by_tracks->setCheckable(true);
		action_view_by_tracks->setActionGroup(group_view_by);
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

void MainWindow::showLibraryTitle(const QString& title)
{
	setWindowTitle(QString("%1 [%2]").arg(tr("Статистика музыки"), title));
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
							 .arg(_read_files_timer.elapsed()), 5000);
}

void MainWindow::openFile()
{
	auto file_names = QFileDialog::getOpenFileNames(
				this,
				tr("Выберите один или два файла для анализа"),
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
