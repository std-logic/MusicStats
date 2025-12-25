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
		NUM_OF_STATISTICS_TYPES
	};

public:
	explicit StatisticsChart(QWidget* parent = nullptr);

	bool setStatisticsType(StatisticsTypes statistics_type);
	void clearStatistics();
	void showStatistics(const Library& library);

signals:

private:
	void init();

	QBarSet* getBarSet();
	QBarCategoryAxis* getAxisX();
	QValueAxis* getAxisY();

	void showPlayCounts(const Library& library);
	void showYears(const Library& library);

private:
	StatisticsTypes _statistics_type = STATISTICS_OFF;
};
