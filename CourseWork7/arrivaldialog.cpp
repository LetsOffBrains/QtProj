#include "arrivaldialog.h"
#include "ui_arrivaldialog.h"
#include <QDate>

ArrivalDialog::ArrivalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArrivalDialog)
{
    ui->setupUi(this);
}

ArrivalDialog::~ArrivalDialog()
{
    delete ui;
}

void ArrivalDialog::on_buttonBox_accepted()
{
    QSqlQuery q(*db);

    int rowCount = ui->tableWidget->rowCount(), count;
    QString name;
    for(int i = 0; i < rowCount; ++i){
        name = ui->tableWidget->item(i, 0)->text();
        count = ui->tableWidget->item(i, 1)->text().toInt();

        if(count != 0){
            q.exec(QString("SELECT * FROM Storage WHERE Name = '%1'").arg(name));

            int colNumber = q.record().indexOf("Number");
            q.first();
            int old = q.value(colNumber).toInt();

            q.exec(QString("SELECT * FROM Products WHERE Name = '%1'").arg(name));
            int colExp = q.record().indexOf("Expiration");
            q.first();
            int exp = q.value(colExp).toInt();
            QDate curdate, date;
            curdate = QDate::currentDate();
            date = curdate.addDays(exp);

            qDebug() << name << " = " << old << " + " << count << " = " << old + count;
            qDebug() << "Received " << curdate.toString("yyyy-MM-dd") << "last for" << exp << " expires " << date.toString("yyyy-MM-dd");

            if(!q.exec(QString("UPDATE Storage SET Number = '%2' WHERE Name = '%1'").arg(name).arg(count + old))){
                qDebug() << q.lastError().text();
            }
            if(!q.exec(QString("UPDATE Storage SET Receipt_date = '%2' WHERE Name = '%1'").arg(name).arg(curdate.toString("yyyy-MM-dd")))){
                    qDebug() << q.lastError().text();
            }
            if(!q.exec(QString("UPDATE Storage SET Rotting_date = '%2' WHERE Name = '%1'").arg(name).arg(date.toString("yyyy-MM-dd")))){
                    qDebug() << q.lastError().text();
            }
        }
    }
}

void ArrivalDialog::fill_table()
{
    QSqlQuery q("SELECT Name FROM Products", *db);
    QSqlRecord rec = q.record();

    bool sort = false;
    int nameCol = rec.indexOf("Name"), i = -1;
    ui->tableWidget->clear();

    if(ui->tableWidget->isSortingEnabled()){
        ui->tableWidget->setSortingEnabled(false);
        sort = true;
    }

    while(q.next()){
//        qDebug() << q.value(nameCol).toString();
        ui->tableWidget->insertRow(++i);

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem());
        ui->tableWidget->item(i, 0)->setText(q.value(nameCol).toString());

        ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
        ui->tableWidget->item(i, 1)->setText("0");
    }

    if(sort){
        ui->tableWidget->setSortingEnabled(true);
    }
    ui->tableWidget->resizeColumnsToContents();
}
