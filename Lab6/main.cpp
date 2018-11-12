#include <QtGui/QApplication>
#include "loginwindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Dropbox\\QtProj\\Lab6\\mydatabase.sqlite");

    if (!db.open()) {
          qDebug() << db.lastError().text();
          return -1;
    }

//    MainWindow w(&db);
    LoginWindow lw(&db);
    lw.show();
//    w.show();

    return a.exec();
}
