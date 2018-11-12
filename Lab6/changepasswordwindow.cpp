#include "changepasswordwindow.h"

ChangePasswordWindow::ChangePasswordWindow(QSqlDatabase *db, QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("No Login");
    this->resize(200, 100);
    sdb = db;

    QLabel *lPrev = new QLabel("Previous Password:", this);
    lPrev->setGeometry(10, 10, 90, 20);

    ePrevPas = new QLineEdit(this);
    ePrevPas->setGeometry(100, 10, 90, 20);

    QLabel *lNew = new QLabel("New Password:", this);
    lNew->setGeometry(10, 40, 90, 20);

    eNewPas = new QLineEdit(this);
    eNewPas->setGeometry(100, 40, 90, 20);

    lInfo = new QLabel("", this);
    lInfo->setGeometry(10, 70, 180, 20);

    connect(ePrevPas, SIGNAL(returnPressed()), this, SLOT(checkAndChange()));
    connect(eNewPas, SIGNAL(returnPressed()), this, SLOT(checkAndChange()));
}

void ChangePasswordWindow::setData(QString log){
    login = log;
    this->setWindowTitle(login);
}

void ChangePasswordWindow::checkAndChange(){
    QSqlTableModel *model = new QSqlTableModel(this, *sdb);
    model->setTable("Users");

    QSqlQuery q(*sdb);
    /*
    q.prepare("UPDATE Users SET password = (newpas) WHERE login = (log), password = (oldpas)"
              "VALUES (:newpas, :log, :oldpas)");
    q.bindValue(":newpas", eNewPas->text());
    q.bindValue(":log", login);
    q.bindValue(":oldpas", ePrevPas->text());
    */
    q.prepare(QString("UPDATE Users SET password = '%1' WHERE login = '%2'").arg(eNewPas->text(), login));

    if(q.exec()){
        this->close();
    }
    else{
        lInfo->setText("Error");
    }
}
