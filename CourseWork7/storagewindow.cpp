#include "storagewindow.h"
#include "ui_storagewindow.h"

StorageWindow::StorageWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StorageWindow)
{
    ui->setupUi(this);

    table = 0;
    this->setWindowTitle("Storage Window");

    QAction *actProd = ui->menu->actions().first();
    QAction *actStor = ui->menu->actions().at(1);
    QAction *actRec = ui->menu->actions().at(2);
//    qDebug() << actProd->text();
    connect(actProd, SIGNAL(triggered()), this, SLOT(showProductsTable()));
    connect(actStor, SIGNAL(triggered()), this, SLOT(showStorageTable()));
    connect(actRec, SIGNAL(triggered()), this, SLOT(showRecipesTable()));
}

StorageWindow::~StorageWindow()
{
    delete ui;
}

void StorageWindow::setDataBase(QSqlDatabase *database)
{
    db = database;
    model = new QSqlTableModel(this, *db);
    model->setTable("Products");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    qDebug() << "Rows: " << model->rowCount() << endl << "Columns: " << model->columnCount();

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setShowGrid(true);

    ui->pbAdd->setEnabled(true);
    ui->pbDelete->setEnabled(true);
    ui->pbArrival->setEnabled(false);
    ui->pbUpdCost->setEnabled(false);
}

void StorageWindow::on_pbArrival_clicked()
{
    arrd = new ArrivalDialog(this);
    arrd->db = db;
    arrd->fill_table();
    arrd->show();
    model->select();
}

void StorageWindow::showProductsTable()
{
    model->setTable("Products");
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setShowGrid(true);

    ui->pbAdd->setEnabled(true);
    ui->pbDelete->setEnabled(true);
    ui->pbArrival->setEnabled(false);
    ui->pbUpdCost->setEnabled(false);
    table = 0;
}

void StorageWindow::showStorageTable()
{
    model->setTable("Storage");
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setShowGrid(true);

    ui->pbAdd->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->pbArrival->setEnabled(true);
    ui->pbUpdCost->setEnabled(false);
    table = 1;
}

void StorageWindow::showRecipesTable()
{
    model->setTable("Recipes");
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setShowGrid(true);

    ui->pbAdd->setEnabled(false);
    ui->pbDelete->setEnabled(false);
    ui->pbArrival->setEnabled(false);
    ui->pbUpdCost->setEnabled(true);
    table = 2;
}

void StorageWindow::on_pbDelete_clicked()
{
    qDebug() << lastClicked.row();
    model->removeRow(lastClicked.row());
    QPixmap pix;
    ui->lPicture->setPixmap(pix);
}

void StorageWindow::on_tableView_clicked(const QModelIndex &index)
{
    lastClicked = index;
    qDebug() << lastClicked.data().toString();
    if(lastClicked.column() == 0){
        QPixmap pixmap("../CourseWork7/img/"+lastClicked.data().toString());
        ui->lPicture->setPixmap(pixmap.scaled(QSize(ui->lPicture->width(),ui->lPicture->height()),Qt::KeepAspectRatio));
    }
}

void StorageWindow::on_pbAdd_clicked()
{
    model->insertRow(model->rowCount());
}

void StorageWindow::on_pbUpdCost_clicked()
{
    QSqlQuery recipes("SELECT * FROM Recipes", *db);

    while(recipes.next()){
        int colRecName = recipes.record().indexOf("Name");
        QString recipeName = recipes.record().value(colRecName).toString();
//        qDebug() << name;
        QSqlQuery q(QString("SELECT * FROM %1").arg(recipeName), *db);
        if(q.isActive()){
//            qDebug() << "Fount table " << recipeName;
            int colProdName = q.record().indexOf("Name");
            int colProdNumber = q.record().indexOf("Number");
            double cost = 0.0;
            QString prodName;
            while(q.next()){
                prodName = q.value(colProdName).toString();
                int prodNumber = q.value(colProdNumber).toInt();
//                qDebug() << prodName << " = " << prodNumber;

                QSqlQuery qProd(QString("SELECT * FROM Products WHERE Name = '%1'").arg(prodName));
                qProd.first();

                int index_in_pack = qProd.record().indexOf("In_pack");
                int index_in_one = qProd.record().indexOf("In_one");
                int index_value = qProd.record().indexOf("Value");

                double in_pack = qProd.value(index_in_pack).toDouble();
                double in_one = qProd.value(index_in_one).toDouble();
                double value = qProd.value(index_value).toDouble();

                cost += (double)prodNumber / in_one / in_pack * value;
                qDebug() << cost;
            }
            QSqlQuery qUpd(*db);
            if(!qUpd.exec(QString("UPDATE Recipes SET Cost = '%2' WHERE Name = '%1'").arg(recipeName).arg(cost))){
                qDebug() << qUpd.lastError().text();
            }
        }
    }
}

void StorageWindow::on_leFilter_textChanged(const QString &)
{
    model->setFilter(QString("Name Like '%1%'").arg(ui->leFilter->text()));
}

void StorageWindow::on_deFrom_dateChanged(const QDate &date)
{
    date_filter();
}

void StorageWindow::on_deTo_dateChanged(const QDate &date)
{
    date_filter();
}

void StorageWindow::date_filter()
{
    if(table == 1){
        model->setFilter(QString("Receipt_date >= '%1' and Receipt_date <= '%2'").arg(ui->deFrom->date().toString("yyyy-MM-dd")).arg(ui->deTo->date().toString("yyyy-MM-dd")));
    }
}
