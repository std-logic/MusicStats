#include "Gui.h"
#include "MainWindow.h"

Gui::Gui(QObject* parent)
	: QObject{parent}
	, _main_window{std::make_unique<MainWindow>()}
{
}

Gui::~Gui()
{
}

void Gui::show()
{
	_main_window->show();
}
