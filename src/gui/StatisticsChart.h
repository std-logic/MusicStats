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
	explicit StatisticsChart(QWidget* parent = nullptr);

	void clearStatistics();
	void showStatistics(const Library& library);

signals:

private:
	void init();

	QBarSet* getBarSet();
	QBarCategoryAxis* getHAxis();
	QValueAxis* getVAxis();
};
