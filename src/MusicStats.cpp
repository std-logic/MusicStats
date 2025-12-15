#include "MusicStats.h"
#include "gui/Gui.h"
#include "input/Input.h"

MusicStats::MusicStats()
	: _gui{std::make_unique<Gui>()}
	, _input{std::make_unique<Input>()}
{
	initConnections();
}

MusicStats::~MusicStats()
{
}

void MusicStats::show()
{
	_gui->show();
}

void MusicStats::initConnections()
{
	QObject::connect(_gui.get(), &Gui::readFiles, _input.get(), &Input::readFiles);
	QObject::connect(_input.get(), &Input::libraryReady, _gui.get(), &Gui::showLibrary);
}
