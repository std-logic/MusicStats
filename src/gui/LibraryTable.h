#pragma once

#include <QTreeWidget>

class Library;

class LibraryTable : public QTreeWidget
{
	Q_OBJECT
public:
	enum ViewByTypes
	{
		VIEW_BY_ARTISTS,
		VIEW_BY_ALBUMS,
		VIEW_BY_TRACKS,
		VIEW_BY_BEST_TRACKS,
		VIEW_BY_SUMMARY,
		NUM_OF_VIEW_BY_TYPES
	};

public:
	explicit LibraryTable(QWidget* parent = nullptr);

	bool setViewByType(ViewByTypes view_by);
	void clearLibrary();
	void showLibrary(const Library& library);

signals:

private slots:
	void sortingChanged(int index, Qt::SortOrder order);
	void copyTitle();

private:
	void init();

	inline QString createOverallString(size_t size) const
	{ return QStringLiteral("#%1: %2").arg(tr("Всего")).arg(size); }

	void showByArtists(const Library& library);
	void showByAlbums(const Library& library);
	void showByTracks(const Library& library);
	void showByBestTracks(const Library& library);
	void showBySummary(const Library& library);

private:
	static const int TOP_SIZE = 10;

	ViewByTypes _view_by = VIEW_BY_ARTISTS;

	bool _sorting_by_year = false;
	bool _sorting_by_play_count = false;
	Qt::SortOrder _sorting_order = Qt::AscendingOrder;
};
