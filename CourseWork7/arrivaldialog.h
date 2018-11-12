#ifndef ARRIVALDIALOG_H
#define ARRIVALDIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

namespace Ui {
class ArrivalDialog;
}

class ArrivalDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ArrivalDialog(QWidget *parent = 0);
    ~ArrivalDialog();
    QSqlDatabase *db;
    void fill_table();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ArrivalDialog *ui;
};

#endif // ARRIVALDIALOG_H
