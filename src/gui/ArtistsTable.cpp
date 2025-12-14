#include "ArtistsTable.h"
#include "common/Helper.h"

ArtistsTable::ArtistsTable(QWidget* parent)
	: QTreeWidget{parent}
{
	init();
	hide();
}

void ArtistsTable::clearLibrary()
{
	clear();
	hide();
}

void ArtistsTable::showLibrary(const Library& library)
{
	QList<QTreeWidgetItem*> items;

	auto library_tracks_count = library.tracksCount();
	auto library_play_count = library.playCount();
	auto library_play_coeff = library_play_count / library_tracks_count;
	auto item_all = new QTreeWidgetItem(this);
	item_all->setText(COLUMN_ARTISTS, tr("#Всего: %1").arg(library.artistsCount()));
	item_all->setText(COLUMN_YEAR, QString::fromStdString(library.yearString()));
	item_all->setData(COLUMN_ALBUMS, Qt::DisplayRole, library.albumsCount());
	item_all->setData(COLUMN_TRACKS, Qt::DisplayRole, library_tracks_count);
	item_all->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, library_play_count);
	item_all->setData(COLUMN_PLAY_COEFF, Qt::DisplayRole, library_play_coeff);
	item_all->setData(COLUMN_SIZE, Qt::DisplayRole, Helper::sizeInMB(library.size()));
	for (auto i = 0; i < NUM_OF_COLUMNS; ++i) {
		QFont default_font = item_all->font(i);
		default_font.setBold(true);
		item_all->setFont(i, default_font);
	}
	items.append(item_all);

	for (const auto& artist_container : library) {
		const auto& artist = artist_container.second;
		auto artist_tracks_count = artist.tracksCount();
		auto artist_play_count = artist.playCount();
		auto artist_play_coeff = artist_play_count / artist_tracks_count;
		auto item_artist = new QTreeWidgetItem(this);
		item_artist->setText(COLUMN_ARTISTS, QString::fromStdString(artist.title()));
		item_artist->setText(COLUMN_YEAR, QString::fromStdString(artist.yearString()));
		item_artist->setData(COLUMN_ALBUMS, Qt::DisplayRole, artist.albumsCount());
		item_artist->setData(COLUMN_TRACKS, Qt::DisplayRole, artist_tracks_count);
		item_artist->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, artist_play_count);
		item_artist->setData(COLUMN_PLAY_COEFF, Qt::DisplayRole, artist_play_coeff);
		item_artist->setData(COLUMN_SIZE, Qt::DisplayRole, Helper::sizeInMB(artist.size()));

		for (const auto& album_container : artist) {
			const auto& album = album_container.second;
			auto album_tracks_count = album.tracksCount();
			auto album_play_count = album.playCount();
			auto album_play_coeff = album_play_count / album_tracks_count;
			auto item_album = new QTreeWidgetItem(item_artist);
			item_album->setText(COLUMN_ARTISTS, QString::fromStdString(album.title()));
			item_album->setText(COLUMN_YEAR, QString::fromStdString(album.yearString()));
			item_album->setData(COLUMN_TRACKS, Qt::DisplayRole, album_tracks_count);
			item_album->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, album_play_count);
			item_album->setData(COLUMN_PLAY_COEFF, Qt::DisplayRole, album_play_coeff);
			item_album->setData(COLUMN_SIZE, Qt::DisplayRole, Helper::sizeInMB(album.size()));

			for (const auto& track : album) {
				auto item_track = new QTreeWidgetItem(item_album);
				item_track->setText(COLUMN_ARTISTS, QString::fromStdString(track.titleWithTrackNumber()));
				item_track->setText(COLUMN_YEAR, QString::fromStdString(track.yearString()));
				item_track->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, track.playCount());
				item_track->setData(COLUMN_SIZE, Qt::DisplayRole, Helper::sizeInMB(track.size()));
			}
		}

		items.append(item_artist);
	}

	addTopLevelItems(items);

	show();
}

void ArtistsTable::init()
{
	setColumnCount(NUM_OF_COLUMNS);
	setHeaderLabels(QStringList()
					<< tr("Исполнитель")
					<< tr("Год")
					<< tr("Альбомов")
					<< tr("Треков")
					<< tr("Кол-во пр.")
					<< tr("Коэфф. пр.")
					<< tr("Размер, MB")
					);

	// auto head_artists = new QTreeWidgetItem(this);
	// head_artists->setText(COLUMN_ARTISTS, tr("1"));
	// head_artists->setText(COLUMN_ALBUMS, tr("2"));
	// head_artists->setText(COLUMN_TRACKS, tr("3"));
	// setHeaderItem(head_artists);

	QFont default_font = font();
	default_font.setPointSize(default_font.pointSize() + 2);
	setFont(default_font);

	setSortingEnabled(true);
	sortByColumn(COLUMN_ARTISTS, Qt::AscendingOrder);
}
