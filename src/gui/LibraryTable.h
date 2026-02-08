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
		VIEW_BY_GENRES,
		VIEW_BY_SUMMARY,
		VIEW_BY_HISTORY,
		NUM_OF_VIEW_BY_TYPES
	};

public:
	explicit LibraryTable(QWidget* parent = nullptr);

	bool setViewByType(ViewByTypes view_by);
	void clearLibrary();
	void showLibrary(const Library& library);
	void showLibraries(const std::vector<Library>& libraries);

signals:

private slots:
	void sortingChanged(int index, Qt::SortOrder order);
	void copyTitle();

private:
	void init();

	inline QString createOverallString(size_t size) const
	{ return QStringLiteral(" %1: %2").arg(tr("Всего")).arg(size); }

	void showByArtists(const Library& library);
	void showByAlbums(const Library& library);
	void showByTracks(const Library& library);
	void showByBestTracks(const Library& library);
	void showByGenres(const Library& library);
	void showBySummary(const Library& library);
	void showByHistory(const std::vector<Library>& libraries);

private:
	static const int TOP_SIZE_FOR_SUMMARY = 10;
	static const int TOP_SIZE_FOR_HISTORY = 3;

	ViewByTypes _view_by = VIEW_BY_ARTISTS;

	bool _sorting_by_year = false;
	bool _sorting_by_play_count = false;
	Qt::SortOrder _sorting_order = Qt::AscendingOrder;
};
