#pragma once

#include <memory>

class Gui;
class Input;

class MusicStats
{
public:
	MusicStats();
	~MusicStats();

	void show();

private:
	void initConnections();

private:
	std::unique_ptr<Gui> _gui;
	std::unique_ptr<Input> _input;
};
