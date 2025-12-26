#pragma once

#include <QTreeWidgetItem>

class LibraryTableItem : public QTreeWidgetItem
{
public:
	explicit LibraryTableItem(int type = QTreeWidgetItem::Type);
	explicit LibraryTableItem(QTreeWidget* treeview, int type = QTreeWidgetItem::Type);
	explicit LibraryTableItem(QTreeWidgetItem* parent, int type = QTreeWidgetItem::Type);

	inline void setNumb(int column, const QVariant& value)
	{ setData(column, Qt::DisplayRole, value); }

	void setBold(bool enable);
	void setBackgroundEverywhere(const QBrush& brush);
};

