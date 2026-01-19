#include "LibraryTable.h"
#include "LibraryTableItem.h"
#include "common/Library.h"

#include <QHeaderView>
#include <QShortcut>
#include <QClipboard>
#include <QApplication>

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
		case VIEW_BY_ARTISTS:		showByArtists(library);			break;
		case VIEW_BY_ALBUMS:		showByAlbums(library);			break;
		case VIEW_BY_TRACKS:		showByTracks(library);			break;
		case VIEW_BY_BEST_TRACKS:	showByBestTracks(library);		break;
		case VIEW_BY_SUMMARY:		showBySummary(library);			break;
		default: return;
	}
	show();
}

void LibraryTable::showLibraries(const std::vector<Library>& libraries)
{
	clear();
	switch (_view_by) {
		case VIEW_BY_HISTORY:		showByHistory(libraries);		break;
		default: return;
	}
	show();
}

void LibraryTable::sortingChanged(int index, Qt::SortOrder order)
{
	// Hardcode! In any view mode COLUMN_YEAR always must have index 1
	_sorting_by_year = (index == 1);
	// Hardcode! In any view mode COLUMN_PLAY_COUNT always must be the last
	_sorting_by_play_count = (index == (columnCount()-1));
	_sorting_order = order;
}

void LibraryTable::copyTitle()
{
	auto selected_items = selectedItems();
	if (!selected_items.isEmpty()) {
		QString selected_text = selected_items.at(0)->text(0);
		QClipboard* clipboard = QApplication::clipboard();
		clipboard->setText(selected_text);
	}
}

void LibraryTable::init()
{
	QFont default_font = font();
	default_font.setPointSize(default_font.pointSize() + 2);
	setFont(default_font);

	// keep current sorting column between view modes
	setSortingEnabled(true);
	connect(header(), &QHeaderView::sortIndicatorChanged, this, &LibraryTable::sortingChanged);

	// copy COLUMN_TITLE text by CTRL+C shortcut
	auto shortcut = new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_C), this);
	shortcut->setContext(Qt::WidgetShortcut);
	connect(shortcut, &QShortcut::activated, this, &LibraryTable::copyTitle);
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
					<< tr("Группа")
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
		items.append(item_artist);

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
			item_album->setText(COLUMN_TITLE, QStringLiteral("[%1] %2")
					.arg(artist_title, album_title));
			item_album->setText(COLUMN_YEAR, album.yearString());
			item_album->setNumb(COLUMN_TRACKS, album.tracksCount());
			item_album->setNumb(COLUMN_PLAY_COUNT, album.playCount());
			item_album->setBackgroundEverywhere(QColor(230, 230, 230));
			items.append(item_album);

			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_album);
				item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());
			}
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
				item_track->setText(COLUMN_TITLE, QStringLiteral("[%1 - %2] %3")
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
		items.append(item_artist);

		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_artist);
				if (artist_title == QStringLiteral("Разное")) {
					item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				} else {
					item_track->setText(COLUMN_TITLE, QStringLiteral("[%1] %2")
							.arg(album_title, track.titleWithTrackNumber()));
				}
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());
			}
		}
	}

	addTopLevelItems(items);
}

void LibraryTable::showBySummary(const Library& library)
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
	sortByColumn(-1, Qt::DescendingOrder);
	setHeaderLabels(QStringList()
					<< tr("Трек")
					<< tr("Год")
					<< tr("Прослушиваний")
					);

	auto [played_artists, played_albums, played_tracks] = library.playedCount();
	auto top_artists = library.topArtists(TOP_SIZE_FOR_SUMMARY);
	auto top_albums = library.topAlbums(TOP_SIZE_FOR_SUMMARY);
	auto top_tracks = library.topTracks(TOP_SIZE_FOR_SUMMARY);
	auto artists_count = library.artistsCount();
	auto albums_count = library.albumsCount();
	auto tracks_count = library.tracksCount();

	QList<QTreeWidgetItem*> items;

	// artists
	auto item_top_artists = new LibraryTableItem(this);
	item_top_artists->setText(COLUMN_TITLE, tr("Топ групп (прослушано %1/%2 - %3%)")
			.arg(played_artists).arg(artists_count).arg(100*played_artists/artists_count));
	item_top_artists->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
	item_top_artists->setBackgroundEverywhere(QColor(230, 230, 230));
	items.append(item_top_artists);

	for (int place = 0; auto [artist, play_count] : top_artists) {
		auto item_artist = new LibraryTableItem(item_top_artists);
		item_artist->setText(COLUMN_TITLE, QStringLiteral("%1. %2")
				.arg(++place, 2, 10, QChar('0')).arg(artist->title()));
		item_artist->setText(COLUMN_YEAR, artist->yearString());
		item_artist->setNumb(COLUMN_PLAY_COUNT, play_count);
	}

	// albums
	auto item_top_albums = new LibraryTableItem(this);
	item_top_albums->setText(COLUMN_TITLE, tr("Топ альбомов (прослушано %1/%2 - %3%)")
			.arg(played_albums).arg(albums_count).arg(100*played_albums/albums_count));
	item_top_albums->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
	item_top_albums->setBackgroundEverywhere(QColor(230, 230, 230));
	items.append(item_top_albums);

	for (int place = 0; auto [album, play_count] : top_albums) {
		auto item_album = new LibraryTableItem(item_top_albums);
		item_album->setText(COLUMN_TITLE, QStringLiteral("%1. [%2] %3")
				.arg(++place, 2, 10, QChar('0')).arg(album->artist(), album->title()));
		item_album->setText(COLUMN_YEAR, album->yearString());
		item_album->setNumb(COLUMN_PLAY_COUNT, play_count);
	}

	// tracks
	auto item_top_tracks = new LibraryTableItem(this);
	item_top_tracks->setText(COLUMN_TITLE, tr("Топ треков (прослушано %1/%2 - %3%)")
			.arg(played_tracks).arg(tracks_count).arg(100*played_tracks/tracks_count));
	item_top_tracks->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
	item_top_tracks->setBackgroundEverywhere(QColor(230, 230, 230));
	items.append(item_top_tracks);

	for (int place = 0; auto [track, play_count] : top_tracks) {
		auto item_track = new LibraryTableItem(item_top_tracks);
		if (track->artist() == QStringLiteral("Разное")) {
			item_track->setText(COLUMN_TITLE, QStringLiteral("%1. [%2] %3")
					.arg(++place, 2, 10, QChar('0')).arg(track->artist(), track->title()));
		} else {
			item_track->setText(COLUMN_TITLE, QStringLiteral("%1. [%2 - %3] %4")
					.arg(++place, 2, 10, QChar('0')).arg(track->artist(), track->album(), track->title()));
		}
		item_track->setText(COLUMN_YEAR, track->yearString());
		item_track->setNumb(COLUMN_PLAY_COUNT, play_count);
	}

	addTopLevelItems(items);
	expandAll();
}

void LibraryTable::showByHistory(const std::vector<Library>& libraries)
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
	sortByColumn(-1, Qt::DescendingOrder);
	setHeaderLabels(QStringList()
					<< tr("Трек")
					<< tr("Год")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	for (unsigned int i = 1; i < libraries.size(); ++i) {
		const auto& prev_library = libraries[i-1];
		const auto& library = libraries[i];

		auto [played_artists, played_albums, played_tracks] = library.playedCount();
		auto top_artists = library.topArtists(TOP_SIZE_FOR_HISTORY);
		auto top_albums = library.topAlbums(TOP_SIZE_FOR_HISTORY);
		auto top_tracks = library.topTracks(TOP_SIZE_FOR_HISTORY);
		auto artists_count = library.artistsCount();
		auto albums_count = library.albumsCount();
		auto tracks_count = library.tracksCount();

		auto item_library = new LibraryTableItem(this);
		item_library->setText(COLUMN_TITLE, tr("%1 → %2")
				.arg(prev_library.titleOnlyDate(), library.titleOnlyDate()));
		item_library->setNumb(COLUMN_PLAY_COUNT, library.playCount());
		item_library->setBackgroundEverywhere(QColor(220, 220, 220));
		items.append(item_library);

		// artists
		auto item_top_artists = new LibraryTableItem(item_library);
		item_top_artists->setText(COLUMN_TITLE, tr("Топ групп (прослушано %1/%2 - %3%)")
				.arg(played_artists).arg(artists_count).arg(100*played_artists/artists_count));
		item_top_artists->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
		item_top_artists->setBackgroundEverywhere(QColor(230, 230, 230));

		for (int place = 0; auto [artist, play_count] : top_artists) {
			auto item_artist = new LibraryTableItem(item_top_artists);
			item_artist->setText(COLUMN_TITLE, QStringLiteral("%1. %2")
					.arg(++place, 2, 10, QChar('0')).arg(artist->title()));
			item_artist->setText(COLUMN_YEAR, artist->yearString());
			item_artist->setNumb(COLUMN_PLAY_COUNT, play_count);
		}

		// albums
		auto item_top_albums = new LibraryTableItem(item_library);
		item_top_albums->setText(COLUMN_TITLE, tr("Топ альбомов (прослушано %1/%2 - %3%)")
				.arg(played_albums).arg(albums_count).arg(100*played_albums/albums_count));
		item_top_albums->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
		item_top_albums->setBackgroundEverywhere(QColor(230, 230, 230));
		items.append(item_top_albums);

		for (int place = 0; auto [album, play_count] : top_albums) {
			auto item_album = new LibraryTableItem(item_top_albums);
			item_album->setText(COLUMN_TITLE, QStringLiteral("%1. [%2] %3")
					.arg(++place, 2, 10, QChar('0')).arg(album->artist(), album->title()));
			item_album->setText(COLUMN_YEAR, album->yearString());
			item_album->setNumb(COLUMN_PLAY_COUNT, play_count);
		}

		// tracks
		auto item_top_tracks = new LibraryTableItem(item_library);
		item_top_tracks->setText(COLUMN_TITLE, tr("Топ треков (прослушано %1/%2 - %3%)")
				.arg(played_tracks).arg(tracks_count).arg(100*played_tracks/tracks_count));
		item_top_tracks->setText(COLUMN_PLAY_COUNT, QStringLiteral(" "));
		item_top_tracks->setBackgroundEverywhere(QColor(230, 230, 230));
		items.append(item_top_tracks);

		for (int place = 0; auto [track, play_count] : top_tracks) {
			auto item_track = new LibraryTableItem(item_top_tracks);
			if (track->artist() == QStringLiteral("Разное")) {
				item_track->setText(COLUMN_TITLE, QStringLiteral("%1. [%2] %3")
						.arg(++place, 2, 10, QChar('0')).arg(track->artist(), track->title()));
			} else {
				item_track->setText(COLUMN_TITLE, QStringLiteral("%1. [%2 - %3] %4")
						.arg(++place, 2, 10, QChar('0')).arg(track->artist(), track->album(), track->title()));
			}
			item_track->setText(COLUMN_YEAR, track->yearString());
			item_track->setNumb(COLUMN_PLAY_COUNT, play_count);
		}
	}

	addTopLevelItems(items);
	expandAll();
}
