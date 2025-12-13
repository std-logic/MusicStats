#include "Gui.h"
#include "MainWindow.h"

Gui::Gui(QObject* parent)
	: QObject{parent}
	, _main_window{std::make_unique<MainWindow>()}
{
	initConnections();
}

Gui::~Gui()
{
}

void Gui::show()
{
	_main_window->show();
}

void Gui::showLibrary(const Library& library)
{
	_main_window->showLibrary(library);
}

void Gui::initConnections()
{
	connect(_main_window.get(), &MainWindow::readFiles, this, &Gui::readFiles);
}
