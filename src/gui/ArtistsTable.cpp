#include "ArtistsTable.h"

// class TreeWidgetItem : public QTreeWidgetItem {
// public:
// 	TreeWidgetItem(QTreeWidget* parent):QTreeWidgetItem(parent){}
// private:
// 	bool operator<(const QTreeWidgetItem &other)const {
// 		int column = treeWidget()->sortColumn();
// 		return text(column).toLower() < other.text(column).toLower();
// 	}
// };

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
	auto library_play_coeff = static_cast<double>(library_play_count) / static_cast<double>(library_tracks_count);
	auto item_all = new QTreeWidgetItem(this);
	item_all->setText(COLUMN_ARTISTS, tr("#Всего: %1").arg(library.artistsCount()));
	item_all->setText(COLUMN_YEAR, QString::fromStdString(library.yearString()));
	item_all->setText(COLUMN_ALBUMS, QString::number(library.albumsCount()));
	item_all->setText(COLUMN_TRACKS, QString::number(library_tracks_count));
	item_all->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, library_play_count);
	item_all->setText(COLUMN_PLAY_COEFF, QString::number(library_play_coeff, 'f', 1));
	for (auto i = 0; i < NUM_OF_COLUMNS; ++i) {
		QFont default_font = item_all->font(i);
		default_font.setBold(true);
		item_all->setFont(i, default_font);
	}
	items.append(item_all);

	for (const auto& artist : library) {
		auto artist_tracks_count = artist.second.tracksCount();
		auto artist_play_count = artist.second.playCount();
		auto artist_play_coeff = static_cast<double>(artist_play_count) / static_cast<double>(artist_tracks_count);
		auto item_artist = new QTreeWidgetItem(this);
		item_artist->setText(COLUMN_ARTISTS, QString::fromStdString(artist.second.title()));
		item_artist->setText(COLUMN_YEAR, QString::fromStdString(artist.second.yearString()));
		item_artist->setText(COLUMN_ALBUMS, QString::number(artist.second.albumsCount()));
		item_artist->setText(COLUMN_TRACKS, QString::number(artist_tracks_count));
		item_artist->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, artist_play_count);
		item_artist->setText(COLUMN_PLAY_COEFF, QString::number(artist_play_coeff, 'f', 1));

		for (const auto& album : artist.second) {
			auto album_tracks_count = album.second.tracksCount();
			auto album_play_count = album.second.playCount();
			auto album_play_coeff = static_cast<double>(album_play_count) / static_cast<double>(album_tracks_count);
			auto item_album = new QTreeWidgetItem(item_artist);
			item_album->setText(COLUMN_ARTISTS, QString::fromStdString(album.second.title()));
			item_album->setText(COLUMN_YEAR, QString::fromStdString(album.second.yearString()));
			item_album->setText(COLUMN_TRACKS, QString::number(album_tracks_count));
			item_album->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, album_play_count);
			item_album->setText(COLUMN_PLAY_COEFF, QString::number(album_play_coeff, 'f', 1));

			for (const auto& track : album.second) {
				auto item_track = new QTreeWidgetItem(item_album);
				item_track->setText(COLUMN_ARTISTS, QString::fromStdString(track.titleWithTrackNumber()));
				// item_track->setIcon(COLUMN_ARTISTS, QIcon::fromTheme(QIcon::ThemeIcon::AudioCard));
				item_track->setText(COLUMN_YEAR, QString::fromStdString(track.yearString()));
				item_track->setData(COLUMN_PLAY_COUNT, Qt::DisplayRole, track.playCount());
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
