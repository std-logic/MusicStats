#include "MusicStats.h"
#include "gui/Gui.h"
#include "input/Input.h"
#include "logic/Logic.h"

MusicStats::MusicStats()
	: _gui{std::make_unique<Gui>()}
	, _input{std::make_unique<Input>()}
	, _logic{std::make_unique<Logic>()}
{
}

MusicStats::~MusicStats()
{
}

void MusicStats::show()
{
	_gui->show();
}
