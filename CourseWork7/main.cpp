#include <QtGui/QApplication>
#include "storagewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("C:\\Dropbox\\QtProj\\CourseWork7\\mydatabase.sqlite");
    db.setDatabaseName("../CourseWork7/mydatabase.sqlite");

    if (!db.open()) {
          qDebug() << db.lastError().text();
          return -1;
    }

    StorageWindow w;
    w.setDataBase(&db);
    w.show();
    
    return a.exec();
}
