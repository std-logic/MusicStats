#include "MainWindow.h"
#include "ArtistsTable.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow{parent}
	, _artists_table{std::make_unique<ArtistsTable>(this)}
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
	_artists_table->showLibrary(library);
	showLibraryTitle(QString::fromStdString(library.title()));
	showReadingFinish();
}

void MainWindow::initCommonParams()
{
	setMinimumSize(QSize(960, 540));
}

void MainWindow::initMenuBar()
{
	setMenuBar(new QMenuBar);

	auto menu_file = menuBar()->addMenu(tr("Файл"));
	menu_file->addAction(
				QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
				tr("Открыть..."),
				QKeySequence(Qt::CTRL | Qt::Key_O),
				this,
				&MainWindow::openFile);
	menu_file->addAction(
				QIcon::fromTheme(QIcon::ThemeIcon::ViewRefresh),
				tr("Очистить"),
				QKeySequence(Qt::CTRL | Qt::Key_C),
				this,
				&MainWindow::clearAll);

	// auto menu_view = menuBar()->addMenu(tr("Вид"));
	// auto action_add_various = menu_view->addAction(
	// 			tr("Вкл. 'Разное' в список групп"),
	// 			QKeySequence(),
	// 			this,
	// 			&MainWindow::changedAddVarious);
	// action_add_various->setCheckable(true);
}

void MainWindow::initStatusBar()
{
	setStatusBar(new QStatusBar);
}

void MainWindow::initCentralWidgets()
{
	setCentralWidget(_artists_table.get());
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
	_artists_table->clearLibrary();
}
