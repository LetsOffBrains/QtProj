#ifndef INVENTORY_H
#define INVENTORY_H

#include <QTableWidget>

class Inventory : public QTableWidget
{
    Q_OBJECT
public:
    Inventory(int rows, int columns, QWidget *parent = 0);

private slots:
    void useItemAt(int row, int column);
};

#endif // INVENTORY_H
