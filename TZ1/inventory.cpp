#include "inventory.h"
#include <QTableWidget>
#include "item.h"

Inventory::Inventory(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent){
    setItemPrototype(new Item);
    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(useItemAt(int, int)));
}

void Inventory::useItemAt(int row, int column){
    //QTableWidgetItem *twi = this->itemAt(row, column);
    //Item* it = dynamic_cast<Item*>( twi );
    //it->use();
}
