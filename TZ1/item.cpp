#include "item.h"

Item::Item(int type, int _count) : QTableWidgetItem(type){
    count = _count;
    this->setText(QString::number(count));
    switch(type){
    case Item::Apple:{
        this->setIcon(QIcon(":/apple.png"));
        this->setToolTip("Apple");
        break;
    }
    }
}

void Item::setCount(int _count){
    count = _count;
    checkCount();
}

void Item::use(int n){
    int tmp = getCount();
    tmp -= n;
    setCount(tmp);
}

void Item::checkCount(){
    if(count <= 0){
        //delete(this);
    }
}

int Item::getCount(){
    return count;
}
