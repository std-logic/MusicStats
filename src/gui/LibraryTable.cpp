#include "LibraryTable.h"
#include "LibraryTableItem.h"
#include "common/Library.h"

#include <QHeaderView>

LibraryTable::LibraryTable(QWidget* parent)
	: QTreeWidget{parent}
{
	init();
	hide();
}

bool LibraryTable::setViewByType(ViewByTypes view_by)
{
	if (_view_by == view_by) {
		return false;
	}
	_view_by = view_by;
	return true;
}

void LibraryTable::clearLibrary()
{
	_sorting_by_year = false;
	_sorting_by_play_count = false;

	clear();
	hide();
}

void LibraryTable::showLibrary(const Library& library)
{
	clear();
	switch (_view_by) {
		case VIEW_BY_ARTISTS:		showByArtists(library);		break;
		case VIEW_BY_ALBUMS:		showByAlbums(library);		break;
		case VIEW_BY_TRACKS:		showByTracks(library);		break;
		case VIEW_BY_BEST_TRACKS:	showByBestTracks(library);	break;
		default: return;
	}
	show();
}

void LibraryTable::sortIndicatorChanged(int index, Qt::SortOrder order)
{
	// Hardcode! In any view mode COLUMN_YEAR always must have index 1
	_sorting_by_year = (index == 1);
	// Hardcode! In any view mode COLUMN_PLAY_COUNT always must be the last
	_sorting_by_play_count = (index == (columnCount()-1));
	_sorting_order = order;
}

void LibraryTable::init()
{
	QFont default_font = font();
	default_font.setPointSize(default_font.pointSize() + 2);
	setFont(default_font);

	setSortingEnabled(true);

	connect(header(), &QHeaderView::sortIndicatorChanged,
			this, &LibraryTable::sortIndicatorChanged);
}

void LibraryTable::showByArtists(const Library& library)
{
	enum ColumnsNames
	{
		COLUMN_TITLE,
		COLUMN_YEAR,
		COLUMN_ALBUMS,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	setColumnWidth(COLUMN_TITLE, 500);
	if (_sorting_by_year)				{ sortByColumn(COLUMN_YEAR, _sorting_order); }
	else if (_sorting_by_play_count)	{ sortByColumn(COLUMN_PLAY_COUNT, _sorting_order); }
	else								{ sortByColumn(COLUMN_TITLE, Qt::AscendingOrder); }
	setHeaderLabels(QStringList()
					<< tr("Исполнитель")
					<< tr("Год")
					<< tr("Альбомов")
					<< tr("Треков")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, createOverallString(library.artistsCount()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_ALBUMS, library.albumsCount());
	item_all->setNumb(COLUMN_TRACKS, library.tracksCount());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	item_all->setBackgroundEverywhere(QColor(210, 210, 210));
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		auto item_artist = new LibraryTableItem(this);
		item_artist->setText(COLUMN_TITLE, artist_title);
		item_artist->setText(COLUMN_YEAR, artist.yearString());
		item_artist->setNumb(COLUMN_ALBUMS, artist.albumsCount());
		item_artist->setNumb(COLUMN_TRACKS, artist.tracksCount());
		item_artist->setNumb(COLUMN_PLAY_COUNT, artist.playCount());
		item_artist->setBackgroundEverywhere(QColor(220, 220, 220));

		for (const auto& [album_title, album] : artist) {
			auto item_album = new LibraryTableItem(item_artist);
			item_album->setText(COLUMN_TITLE, album_title);
			item_album->setText(COLUMN_YEAR, album.yearString());
			item_album->setNumb(COLUMN_TRACKS, album.tracksCount());
			item_album->setNumb(COLUMN_PLAY_COUNT, album.playCount());
			item_album->setBackgroundEverywhere(QColor(230, 230, 230));

			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_album);
				item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());
			}
		}

		items.append(item_artist);
	}

	addTopLevelItems(items);
}

void LibraryTable::showByAlbums(const Library& library)
{
	enum ColumnsNames
	{
		COLUMN_TITLE,
		COLUMN_YEAR,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	setColumnWidth(COLUMN_TITLE, 600);
	if (_sorting_by_year)				{ sortByColumn(COLUMN_YEAR, _sorting_order); }
	else if (_sorting_by_play_count)	{ sortByColumn(COLUMN_PLAY_COUNT, _sorting_order); }
	else								{ sortByColumn(COLUMN_TITLE, Qt::AscendingOrder); }
	setHeaderLabels(QStringList()
					<< tr("Альбом")
					<< tr("Год")
					<< tr("Треков")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, createOverallString(library.albumsCount()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_TRACKS, library.tracksCount());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	item_all->setBackgroundEverywhere(QColor(210, 210, 210));
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			auto item_album = new LibraryTableItem(this);
			item_album->setText(COLUMN_TITLE, QStringLiteral("%1 - %2")
					.arg(artist_title, album_title));
			item_album->setText(COLUMN_YEAR, album.yearString());
			item_album->setNumb(COLUMN_TRACKS, album.tracksCount());
			item_album->setNumb(COLUMN_PLAY_COUNT, album.playCount());
			item_album->setBackgroundEverywhere(QColor(230, 230, 230));

			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_album);
				item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());
			}

			items.append(item_album);
		}
	}

	addTopLevelItems(items);
}

void LibraryTable::showByTracks(const Library& library)
{
	enum ColumnsNames
	{
		COLUMN_TITLE,
		COLUMN_YEAR,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	setColumnWidth(COLUMN_TITLE, 700);
	if (_sorting_by_year)				{ sortByColumn(COLUMN_YEAR, _sorting_order); }
	else if (_sorting_by_play_count)	{ sortByColumn(COLUMN_PLAY_COUNT, _sorting_order); }
	else								{ sortByColumn(COLUMN_TITLE, Qt::AscendingOrder); }
	setHeaderLabels(QStringList()
					<< tr("Трек")
					<< tr("Год")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, createOverallString(library.tracksCount()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	item_all->setBackgroundEverywhere(QColor(210, 210, 210));
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(this);
				item_track->setText(COLUMN_TITLE, QStringLiteral("%1 - %2 - %3")
						.arg(artist_title, album_title, track.titleWithTrackNumber()));
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());

				items.append(item_track);
			}
		}
	}

	addTopLevelItems(items);
}

void LibraryTable::showByBestTracks(const Library& library)
{
	enum ColumnsNames
	{
		COLUMN_TITLE,
		COLUMN_YEAR,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	setColumnWidth(COLUMN_TITLE, 600);
	if (_sorting_by_year)				{ sortByColumn(COLUMN_YEAR, _sorting_order); }
	else if (_sorting_by_play_count)	{ sortByColumn(COLUMN_PLAY_COUNT, _sorting_order); }
	else								{ sortByColumn(COLUMN_PLAY_COUNT, Qt::DescendingOrder); }
	setHeaderLabels(QStringList()
					<< tr("Трек")
					<< tr("Год")
					<< tr("Треков")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, createOverallString(library.artistsCount()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_TRACKS, library.tracksCount());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	item_all->setBackgroundEverywhere(QColor(210, 210, 210));
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		auto item_artist = new LibraryTableItem(this);
		item_artist->setText(COLUMN_TITLE, artist_title);
		item_artist->setText(COLUMN_YEAR, artist.yearString());
		item_artist->setNumb(COLUMN_TRACKS, artist.tracksCount());
		item_artist->setNumb(COLUMN_PLAY_COUNT, artist.playCount());
		item_artist->setBackgroundEverywhere(QColor(220, 220, 220));

		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_artist);
				item_track->setText(COLUMN_TITLE, QStringLiteral("%1 - %2")
						.arg(album_title, track.titleWithTrackNumber()));
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());
			}
		}

		items.append(item_artist);
	}

	addTopLevelItems(items);
}
