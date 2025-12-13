#include "MusicStats.h"
#include "gui/Gui.h"
#include "input/Input.h"
#include "logic/Logic.h"

MusicStats::MusicStats()
	: _gui{std::make_unique<Gui>()}
	, _input{std::make_unique<Input>()}
	, _logic{std::make_unique<Logic>()}
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
	QObject::connect(_input.get(), &Input::tracksReady, _logic.get(), &Logic::processTracks);
	QObject::connect(_logic.get(), &Logic::libraryReady, _gui.get(), &Gui::showLibrary);
}
