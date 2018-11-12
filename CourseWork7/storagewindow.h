#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

#include "arrivaldialog.h"
#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QAction>
#include <QModelIndex>
#include <QSqlQuery>

namespace Ui {
class StorageWindow;
}

class StorageWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StorageWindow(QWidget *parent = 0);
    ~StorageWindow();
    void setDataBase(QSqlDatabase *database);
    QSqlDatabase *db;
    
private slots:
    void on_pbArrival_clicked();
    void showProductsTable();
    void showStorageTable();
    void showRecipesTable();

    void on_pbDelete_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pbAdd_clicked();

    void on_pbUpdCost_clicked();

    void on_leFilter_textChanged(const QString &arg1);

    void on_deFrom_dateChanged(const QDate &date);

    void on_deTo_dateChanged(const QDate &date);

    void date_filter();

private:
    Ui::StorageWindow *ui;
    QSqlTableModel *model;
    ArrivalDialog *arrd;
    QModelIndex lastClicked;
    int table;
};

#endif // STORAGEWINDOW_H
