#include "LibraryTable.h"
#include "common/Helper.h"

#include "LibraryTableItem.h"

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
	clear();
	hide();
}

void LibraryTable::showLibrary(const Library& library)
{
	clear();
	switch (_view_by) {
		case VIEW_BY_ARTISTS:	showByArtists(library);		break;
		case VIEW_BY_ALBUMS:	showByAlbums(library);		break;
		case VIEW_BY_TRACKS:	showByTracks(library);		break;
		default: break;
	}
	show();
}

void LibraryTable::init()
{
	QFont default_font = font();
	default_font.setPointSize(default_font.pointSize() + 2);
	setFont(default_font);

	setSortingEnabled(true);
}

void LibraryTable::showByArtists(const Library& library)
{
	enum ColumnsNames
	{
		COLUMN_TITLE,
		COLUMN_SIZE,
		COLUMN_YEAR,
		COLUMN_ALBUMS,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	sortByColumn(COLUMN_TITLE, Qt::AscendingOrder);
	setHeaderLabels(QStringList()
					<< tr("Исполнитель")
					<< tr("Размер, MB")
					<< tr("Год")
					<< tr("Альбомов")
					<< tr("Треков")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, QStringLiteral("#%1: %2")
					  .arg(tr("Всего")).arg(library.artistsCount()));
	item_all->setNumb(COLUMN_SIZE, Helper::sizeInMB(library.size()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_ALBUMS, library.albumsCount());
	item_all->setNumb(COLUMN_TRACKS, library.tracksCount());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		auto item_artist = new LibraryTableItem(this);
		item_artist->setText(COLUMN_TITLE, artist_title);
		item_artist->setNumb(COLUMN_SIZE, Helper::sizeInMB(artist.size()));
		item_artist->setText(COLUMN_YEAR, artist.yearString());
		item_artist->setNumb(COLUMN_ALBUMS, artist.albumsCount());
		item_artist->setNumb(COLUMN_TRACKS, artist.tracksCount());
		item_artist->setNumb(COLUMN_PLAY_COUNT, artist.playCount());

		for (const auto& [album_title, album] : artist) {
			auto item_album = new LibraryTableItem(item_artist);
			item_album->setText(COLUMN_TITLE, album_title);
			item_album->setNumb(COLUMN_SIZE, Helper::sizeInMB(album.size()));
			item_album->setText(COLUMN_YEAR, album.yearString());
			item_album->setNumb(COLUMN_TRACKS, album.tracksCount());
			item_album->setNumb(COLUMN_PLAY_COUNT, album.playCount());

			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_album);
				item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				item_track->setNumb(COLUMN_SIZE, Helper::sizeInMB(track.size()));
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
		COLUMN_SIZE,
		COLUMN_YEAR,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	sortByColumn(COLUMN_TITLE, Qt::AscendingOrder);
	setHeaderLabels(QStringList()
					<< tr("Альбом")
					<< tr("Размер, MB")
					<< tr("Год")
					<< tr("Треков")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, QStringLiteral("#%1: %2")
					  .arg(tr("Всего")).arg(library.albumsCount()));
	item_all->setNumb(COLUMN_SIZE, Helper::sizeInMB(library.size()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_TRACKS, library.tracksCount());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			auto item_album = new LibraryTableItem(this);
			item_album->setText(COLUMN_TITLE, QStringLiteral("%1 - %2")
								.arg(artist_title, album_title));
			item_album->setNumb(COLUMN_SIZE, Helper::sizeInMB(album.size()));
			item_album->setText(COLUMN_YEAR, album.yearString());
			item_album->setNumb(COLUMN_TRACKS, album.tracksCount());
			item_album->setNumb(COLUMN_PLAY_COUNT, album.playCount());

			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(item_album);
				item_track->setText(COLUMN_TITLE, track.titleWithTrackNumber());
				item_track->setNumb(COLUMN_SIZE, Helper::sizeInMB(track.size()));
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
		COLUMN_SIZE,
		COLUMN_YEAR,
		COLUMN_PLAY_COUNT,
		NUM_OF_COLUMNS
	};
	setColumnCount(NUM_OF_COLUMNS);
	sortByColumn(COLUMN_TITLE, Qt::AscendingOrder);
	setHeaderLabels(QStringList()
					<< tr("Трек")
					<< tr("Размер, MB")
					<< tr("Год")
					<< tr("Прослушиваний")
					);

	QList<QTreeWidgetItem*> items;

	auto item_all = new LibraryTableItem(this);
	item_all->setIcon(COLUMN_TITLE, QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout));
	item_all->setText(COLUMN_TITLE, QStringLiteral("#%1: %2")
					  .arg(tr("Всего")).arg(library.tracksCount()));
	item_all->setNumb(COLUMN_SIZE, Helper::sizeInMB(library.size()));
	item_all->setText(COLUMN_YEAR, library.yearString());
	item_all->setNumb(COLUMN_PLAY_COUNT, library.playCount());
	item_all->setBold(true);
	items.append(item_all);

	for (const auto& [artist_title, artist] : library) {
		for (const auto& [album_title, album] : artist) {
			for (const auto& track : album) {
				auto item_track = new LibraryTableItem(this);
				item_track->setText(COLUMN_TITLE, QStringLiteral("%1 - %2 - %3")
									.arg(artist_title, album_title, track.titleWithTrackNumber()));
				item_track->setNumb(COLUMN_SIZE, Helper::sizeInMB(track.size()));
				item_track->setText(COLUMN_YEAR, track.yearString());
				item_track->setNumb(COLUMN_PLAY_COUNT, track.playCount());

				items.append(item_track);
			}
		}
	}

	addTopLevelItems(items);
}
