#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "item.h"
#include "inventory.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    inv = new Inventory(3, 3, this);
    inv->setGeometry(25, 75, 452, 452);
    inv->horizontalHeader()->setDefaultSectionSize(150);
    inv->horizontalHeader()->setVisible(false);
    inv->verticalHeader()->setDefaultSectionSize(150);
    inv->verticalHeader()->setVisible(false);
    inv->setIconSize(QSize(100, 100));
    inv->setEditTriggers(QAbstractItemView::NoEditTriggers);

    Item *it = new Item(1001, 3);
    inv->setItem(1, 1, it);
    it->use();
}

MainWidget::~MainWidget()
{
    delete ui;
}
