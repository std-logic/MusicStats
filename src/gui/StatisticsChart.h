#pragma once

#include <QChartView>

class Library;

class QBarSet;
class QBarCategoryAxis;
class QValueAxis;

class StatisticsChart : public QChartView
{
	Q_OBJECT
public:
	enum StatisticsTypes
	{
		STATISTICS_OFF,
		STATISTICS_PLAY_COUNTS,
		STATISTICS_YEARS,
		STATISTICS_HISTORY_PLAY_COUNTS,
		STATISTICS_HISTORY_ARTISTS,
		STATISTICS_HISTORY_ALBUMS,
		STATISTICS_HISTORY_TRACKS,
		NUM_OF_STATISTICS_TYPES
	};

public:
	explicit StatisticsChart(QWidget* parent = nullptr);

	bool setStatisticsType(StatisticsTypes statistics_type);
	void clearStatistics();
	void showStatistics(const Library& library);
	void showStatistics(const std::vector<Library>& libraries);

signals:

private:
	void init();

	QBarSet* getBarSet();
	QBarCategoryAxis* getAxisX();
	QValueAxis* getAxisY();

	void showPlayCounts(const Library& library);
	void showYears(const Library& library);
	void showHistoryPlayCounts(const std::vector<Library>& libraries);
	void showHistoryArtists(const std::vector<Library>& libraries);
	void showHistoryAlbums(const std::vector<Library>& libraries);
	void showHistoryTracks(const std::vector<Library>& libraries);

private:
	StatisticsTypes _statistics_type = STATISTICS_OFF;
};
