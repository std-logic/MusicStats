#include "StatisticsChart.h"
#include "common/Library.h"

#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <map>

StatisticsChart::StatisticsChart(QWidget* parent)
	: QChartView{parent}
{
	init();
	hide();
}

void StatisticsChart::clearStatistics()
{
	auto bar_sets_1 = getBarSet();
	bar_sets_1->remove(0, bar_sets_1->count());

	hide();
}

void StatisticsChart::showStatistics(const Library& library)
{
	clearStatistics();

	auto map_val_y = std::map<uint32_t, uint32_t>();
	uint32_t max_x = 0;
	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto play_cnt = track.playCount();
				auto val_x = play_cnt / 10;
				map_val_y[val_x]++;
				if (max_x < val_x) { max_x = val_x; }
			}
		}
	}

	auto bar_sets_1 = getBarSet();
	auto bar_h_axis_1 = getHAxis();
	auto bar_v_axis_1 = getVAxis();
	QStringList bar_categories;
	uint32_t max_y = 0;
	for (uint32_t val_x = 0; val_x <= max_x; ++val_x) {
		uint32_t val_y = map_val_y[val_x];
		bar_sets_1->append(val_y);
		bar_categories << QString("%1-%2").arg(val_x*10).arg((val_x+1)*10);
		if (max_y < val_y) { max_y = val_y; }
	}
	bar_h_axis_1->setCategories(bar_categories);
	bar_v_axis_1->setRange(0, ((max_y / 1000) + 1) * 1000);
	bar_v_axis_1->setTickCount((max_y / 1000) + 2);

	show();
}

void StatisticsChart::init()
{
	setViewportMargins(0, 0, 0, 0);
	setRubberBand(QChartView::NoRubberBand);
	setRenderHint(QPainter::Antialiasing);

	auto bar_chart = chart();

	bar_chart->setTheme(QChart::ChartThemeLight);
	bar_chart->setMargins({0, 0, 5, 0});
	bar_chart->setTitle(tr("Распределение по прослушиваниям"));
	bar_chart->legend()->setVisible(false);
	auto title_font = bar_chart->titleFont();
	title_font.setPointSize(title_font.pointSize() + 2);
	bar_chart->setTitleFont(title_font);

	auto bar_set = new QBarSet("");
	bar_set->setLabelColor(QColor(Qt::black));
	auto label_font = bar_set->labelFont();
	label_font.setPointSize(label_font.pointSize() + 1);
	bar_set->setLabelFont(label_font);

	auto bar_series = new QBarSeries();
	bar_series->setLabelsVisible(true);
	bar_series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
	bar_series->append(bar_set);
	bar_chart->addSeries(bar_series);

	auto bar_axis_x = new QBarCategoryAxis;
	bar_chart->addAxis(bar_axis_x, Qt::AlignBottom);
	bar_series->attachAxis(bar_axis_x);

	auto bar_axis_y = new QValueAxis;
	bar_axis_y->setLabelFormat("%d");
	bar_chart->addAxis(bar_axis_y, Qt::AlignLeft);
	bar_series->attachAxis(bar_axis_y);
}

QBarSet* StatisticsChart::getBarSet()
{
	auto bar_chart = chart();
	auto bar_series = bar_chart->series();
	auto bar_series_1 = bar_series.first();
	auto bar_sets = reinterpret_cast<QAbstractBarSeries*>(bar_series_1)->barSets();
	auto bar_sets_1 = bar_sets.first();
	return bar_sets_1;
}

QBarCategoryAxis* StatisticsChart::getHAxis()
{
	auto bar_chart = chart();
	auto bar_h_axis = bar_chart->axes(Qt::Horizontal);
	auto bar_h_axis_1 = reinterpret_cast<QBarCategoryAxis*>(bar_h_axis.first());
	return bar_h_axis_1;
}

QValueAxis* StatisticsChart::getVAxis()
{
	auto bar_chart = chart();
	auto bar_v_axis = bar_chart->axes(Qt::Vertical);
	auto bar_v_axis_1 = reinterpret_cast<QValueAxis*>(bar_v_axis.first());
	return bar_v_axis_1;
}
