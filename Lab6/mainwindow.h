#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QMenuBar>
#include "changepasswordwindow.h"
#include "QLineEdit"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QSqlDatabase *sdb, QWidget *parent = 0);
    ~MainWindow();
    QString user;
    void setPermissions();

private:
    QSqlDatabase *db;
    QSqlTableModel *model;
    QTableView *view;
    QMenuBar *menu;
    QMenu *m;
    QAction *actAddUser;
    QLineEdit *eFilter;

    ChangePasswordWindow *cpw;

signals:
    void callLoginAgain();

private slots:
    void returnToLoginAgain();
    void setFilter();

};

#endif // MAINWINDOW_H
