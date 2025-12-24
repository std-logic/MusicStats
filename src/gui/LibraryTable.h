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
		NUM_OF_VIEW_BY_TYPES
	};

public:
	explicit LibraryTable(QWidget* parent = nullptr);

	bool setViewByType(ViewByTypes view_by);
	void clearLibrary();
	void showLibrary(const Library& library);

signals:

private:
	void init();

	inline QString createOverallString(size_t size) const
	{ return QStringLiteral("#%1: %2").arg(tr("Всего")).arg(size); }

	void showByArtists(const Library& library);
	void showByAlbums(const Library& library);
	void showByTracks(const Library& library);

private:
	ViewByTypes _view_by = VIEW_BY_ARTISTS;
};
