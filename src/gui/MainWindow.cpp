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
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLibrary(const Library& library)
{
	_artists_table->showLibrary(library);
}

void MainWindow::initCommonParams()
{
	setWindowTitle(tr("Статистика музыки"));
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

	emit readFiles(file_names);
}

void MainWindow::clearAll()
{
	_artists_table->clearLibrary();
}
