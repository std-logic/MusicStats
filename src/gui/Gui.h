#pragma once

#include <QObject>

class MainWindow;

class Gui : public QObject
{
	Q_OBJECT
public:
	explicit Gui(QObject* parent = nullptr);
	~Gui();

	void	show();

signals:

private:
	std::unique_ptr<MainWindow> _main_window;

};
