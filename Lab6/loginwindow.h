#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QLabel>
#include "mainwindow.h"
#include "forgotwindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QSqlDatabase *sdb = 0, QWidget *parent = 0);
    
private:
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *forgot;
    QLineEdit *logEdit;
    QLineEdit *pasEdit;
    QLabel *info;
    MainWindow *child;
    QSqlDatabase *db;

    MainWindow *mw;
    ForgotWindow *fw;
signals:
    
public slots:
    void checkPass();
    void restorePassword();
};

#endif // LOGINWINDOW_H
