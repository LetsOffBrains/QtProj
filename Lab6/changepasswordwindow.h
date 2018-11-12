#ifndef CHANGEPASSWORDWINDOW_H
#define CHANGEPASSWORDWINDOW_H

#include <QWidget>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QLineEdit>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

class ChangePasswordWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChangePasswordWindow(QSqlDatabase *db, QWidget *parent = 0);
    void setData(QString log);

private:
    QSqlDatabase *sdb;
    QString login, password;
    QLineEdit *ePrevPas, *eNewPas;
    QLabel *lInfo;

signals:
    
public slots:
    void checkAndChange();
    
};

#endif // CHANGEPASSWORDWINDOW_H
