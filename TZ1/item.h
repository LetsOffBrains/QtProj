#ifndef ITEM_H
#define ITEM_H

#include <QTableWidgetItem>

class Item : public QTableWidgetItem
{
    Q_OBJECT
public:
    Item(int type = Item::Nothing, int _count = 1);
    void setCount(int _count);
    int getCount();
    void use(int n = 1);
private:
    void checkCount();
    enum ItemType{Nothing = 1000, Apple = 1001};
    int count;
signals:
    
public slots:

};

#endif // ITEM_H
