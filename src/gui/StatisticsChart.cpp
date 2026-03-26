#include "StatisticsChart.h"
#include "common/Library.h"

#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <unordered_map>

StatisticsChart::StatisticsChart(QWidget* parent)
	: QChartView{parent}
{
	init();
	hide();
}

bool StatisticsChart::setStatisticsType(StatisticsTypes statistics_type)
{
	if (_statistics_type == statistics_type) {
		return false;
	}
	_statistics_type = statistics_type;
	return true;
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
	switch (_statistics_type) {
		case STATISTICS_PLAY_COUNTS:	showPlayCounts(library);	break;
		case STATISTICS_YEARS:			showYears(library);			break;
		default: return;
	}
	show();
}

void StatisticsChart::showStatistics(const std::vector<Library>& libraries)
{
	clearStatistics();
	switch (_statistics_type) {
		case STATISTICS_HISTORY_PLAY_COUNTS:	showHistoryPlayCounts(libraries);	break;
		case STATISTICS_HISTORY_ARTISTS:		showHistoryArtists(libraries);		break;
		case STATISTICS_HISTORY_ALBUMS:			showHistoryAlbums(libraries);		break;
		case STATISTICS_HISTORY_TRACKS:			showHistoryTracks(libraries);		break;
		default: return;
	}
	show();
}

void StatisticsChart::init()
{
	setViewportMargins(0, 0, 0, 0);
	setRubberBand(QChartView::NoRubberBand);
	setRenderHint(QPainter::Antialiasing);

	auto bar_chart = chart();

	bar_chart->setTheme(QChart::ChartThemeLight);
	bar_chart->setMargins({3, 0, 10, 0});
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
	auto bar_set_1 = bar_sets.first();
	return bar_set_1;
}

QBarCategoryAxis* StatisticsChart::getAxisX()
{
	auto bar_chart = chart();
	auto bar_axis_x = bar_chart->axes(Qt::Horizontal);
	auto bar_axis_x_1 = reinterpret_cast<QBarCategoryAxis*>(bar_axis_x.first());
	return bar_axis_x_1;
}

QValueAxis* StatisticsChart::getAxisY()
{
	auto bar_chart = chart();
	auto bar_axis_y = bar_chart->axes(Qt::Vertical);
	auto bar_axis_y_1 = reinterpret_cast<QValueAxis*>(bar_axis_y.first());
	return bar_axis_y_1;
}

uint32_t StatisticsChart::calcStepY(uint32_t max_y)
{
	return	(max_y > 60000)	? 20000 :	// 4+ steps
			(max_y > 30000)	? 10000 :	// 4-7 steps
			(max_y > 12000)	? 5000 :	// 3-7 steps
			(max_y > 6000)	? 2000 :	// 4-7 steps
			(max_y > 3000)	? 1000 :	// 4-7 steps
			(max_y > 1200)	? 500 :		// 3-7 steps
			(max_y > 600)	? 200 :		// 4-7 steps
			(max_y > 300)	? 100 :		// 4-7 steps
			(max_y > 120)	? 50 :		// 3-7 steps
			(max_y > 60)	? 20 :		// 4-7 steps
			(max_y > 30)	? 10 :		// 4-7 steps
			(max_y > 12)	? 5 :		// 3-7 steps
			(max_y > 5)		? 2 :		// 3-7 steps
							  1;		// 1-6 steps
}

void StatisticsChart::updateBars(uint32_t max_y, const QStringList& categories)
{
	uint32_t step_y = calcStepY(max_y);

	auto bar_axis_x = getAxisX();
	bar_axis_x->setCategories(categories);

	auto bar_axis_y = getAxisY();
	bar_axis_y->setRange(0, ((max_y / step_y) + 1) * step_y);
	bar_axis_y->setTickCount((max_y / step_y) + 2);
}

void StatisticsChart::showPlayCounts(const Library& library)
{
	chart()->setTitle(tr("Распределение по прослушиваниям"));

	auto map_val_y = std::unordered_map<uint32_t, uint32_t>();
	uint32_t min_x = 9999, max_x = 0;
	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto play_cnt = track.playCount();
				auto val_x = play_cnt / 10;
				map_val_y[val_x]++;
				if (min_x > val_x) { min_x = val_x; }
				if (max_x < val_x) { max_x = val_x; }
			}
		}
	}

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	for (uint32_t val_x = min_x; val_x <= max_x; ++val_x) {
		uint32_t val_y = map_val_y[val_x];
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(QString("%1-%2").arg(val_x*10).arg(val_x*10+9));
	}
	updateBars(max_y, categories);
}

void StatisticsChart::showYears(const Library& library)
{
	chart()->setTitle(tr("Распределение по годам"));

	auto map_val_y = std::unordered_map<uint32_t, uint32_t>();
	uint32_t min_x = 9999, max_x = 0;
	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto year = track.year();
				if (year != Global::undefined_year) {
					auto val_x = year / 10;
					map_val_y[val_x]++;
					if (min_x > val_x) { min_x = val_x; }
					if (max_x < val_x) { max_x = val_x; }
				} else {
					map_val_y[Global::undefined_year]++;
				}
			}
		}
	}

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	bar_set->append(map_val_y[Global::undefined_year]);
	categories.append(tr("Неизвестные"));
	for (uint32_t val_x = min_x; val_x <= max_x; ++val_x) {
		uint32_t val_y = map_val_y[val_x];
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(QString("%1-%2").arg(val_x*10).arg(val_x*10+9));
	}
	updateBars(max_y, categories);
}

void StatisticsChart::showHistoryPlayCounts(const std::vector<Library>& libraries)
{
	chart()->setTitle(tr("Прирост прослушиваний"));

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	for (unsigned int i = 1; i < libraries.size(); ++i) {
		uint32_t val_y = libraries[i].playCount();
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(libraries[i].titleOnlyDate());
	}
	updateBars(max_y, categories);
}

void StatisticsChart::showHistoryArtists(const std::vector<Library>& libraries)
{
	chart()->setTitle(tr("Прирост групп"));

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	for (unsigned int i = 1; i < libraries.size(); ++i) {
		uint32_t val_y = libraries[i].artistsCount() - libraries[i-1].artistsCount();
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(libraries[i].titleOnlyDate());
	}
	updateBars(max_y, categories);
}

void StatisticsChart::showHistoryAlbums(const std::vector<Library>& libraries)
{
	chart()->setTitle(tr("Прирост альбомов"));

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	for (unsigned int i = 1; i < libraries.size(); ++i) {
		uint32_t val_y = libraries[i].albumsCount() - libraries[i-1].albumsCount();
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(libraries[i].titleOnlyDate());
	}
	updateBars(max_y, categories);
}

void StatisticsChart::showHistoryTracks(const std::vector<Library>& libraries)
{
	chart()->setTitle(tr("Прирост треков"));

	auto bar_set = getBarSet();
	QStringList categories;
	uint32_t max_y = 0;
	for (unsigned int i = 1; i < libraries.size(); ++i) {
		uint32_t val_y = libraries[i].tracksCount() - libraries[i-1].tracksCount();
		if (max_y < val_y) { max_y = val_y; }
		bar_set->append(val_y);
		categories.append(libraries[i].titleOnlyDate());
	}
	updateBars(max_y, categories);
}
