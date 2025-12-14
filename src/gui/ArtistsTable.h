#pragma once

#include "common/Library.h"

#include <QTreeWidget>

class ArtistsTable : public QTreeWidget
{
	Q_OBJECT
public:
	explicit ArtistsTable(QWidget* parent = nullptr);

	void clearLibrary();
	void showLibrary(const Library& library);

signals:

private:
	void init();

private:
	enum ColumnsNames
	{
		COLUMN_ARTISTS,
		COLUMN_YEAR,
		COLUMN_ALBUMS,
		COLUMN_TRACKS,
		COLUMN_PLAY_COUNT,
		COLUMN_PLAY_COEFF,
		COLUMN_SIZE,
		NUM_OF_COLUMNS
	};
};
