#include "LibraryTableItem.h"

LibraryTableItem::LibraryTableItem(int type)
	: QTreeWidgetItem{type}
{
}

LibraryTableItem::LibraryTableItem(QTreeWidget* treeview, int type)
	: QTreeWidgetItem{treeview, type}
{
}

LibraryTableItem::LibraryTableItem(QTreeWidgetItem* parent, int type)
	: QTreeWidgetItem{parent, type}
{
}

void LibraryTableItem::setBold(bool enable)
{
	for (auto i = 0; i < columnCount(); ++i) {
		auto default_font = font(i);
		default_font.setBold(enable);
		setFont(i, default_font);
	}
}

void LibraryTableItem::setBackgroundEverywhere(const QBrush& brush)
{
	for (auto i = 0; i < columnCount(); ++i) {
		setBackground(i, brush);
	}
}
