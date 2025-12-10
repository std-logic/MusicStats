#pragma once

#include <memory>

class Gui;
class Input;
class Logic;

class MusicStats
{
public:
	MusicStats();
	~MusicStats();

	void	show();

private:
	std::unique_ptr<Gui> _gui;
	std::unique_ptr<Input> _input;
	std::unique_ptr<Logic> _logic;
};
