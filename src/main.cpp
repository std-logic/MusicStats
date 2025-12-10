#include "MusicStats.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "MusicStats_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			app.installTranslator(&translator);
			break;
		}
	}

	MusicStats music_stats;
	music_stats.show();

	return app.exec();
}
