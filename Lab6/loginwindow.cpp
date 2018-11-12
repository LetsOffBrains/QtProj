#include "loginwindow.h"
#include "forgotwindow.h"
//#include "mainwindow.h"

LoginWindow::LoginWindow(QSqlDatabase *sdb, QWidget *parent) :
    QWidget(parent)
{
    this->db = sdb;
    mw = new MainWindow(sdb);
    fw = new ForgotWindow();
    this->resize(200, 170);
    this->setWindowTitle("Product Data Base");

    info = new QLabel("Enter your data", this);
    info->setGeometry(10, 10, 180, 20);
    info->setAlignment(Qt::AlignHCenter);

    logEdit = new QLineEdit("Login", this);
    logEdit->setGeometry(10, 40, 180, 20);

    pasEdit = new QLineEdit("Password", this);
    pasEdit->setGeometry(10, 70, 180, 20);

    ok = new QPushButton("OK", this);
    ok->setGeometry(10, 100, 80, 20);

    cancel = new QPushButton("Cancel", this);
    cancel->setGeometry(110, 100, 80, 20);

    forgot = new QPushButton("Forgot Password", this);
    forgot->setGeometry(10, 130, 180, 20);

    connect(ok, SIGNAL(clicked()), this, SLOT(checkPass()));
    connect(pasEdit, SIGNAL(returnPressed()), this, SLOT(checkPass()));
    connect(logEdit, SIGNAL(returnPressed()), this, SLOT(checkPass()));

    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(forgot, SIGNAL(clicked()), this, SLOT(restorePassword()));

    connect(fw, SIGNAL(callLoginAgain()), this, SLOT(show()));
    connect(mw, SIGNAL(callLoginAgain()), this, SLOT(show()));
}

void LoginWindow::checkPass(){
    QString login, password;

    QSqlQuery q("SELECT * FROM Users", *db);

    if(!q.isActive()){
        qDebug() << "Query is not Active";
    }

    int index_login = q.record().indexOf("login");
    int index_password = q.record().indexOf("password");
    bool found = false;
    while(q.next()){
//        qDebug() << q.value(index_login).toString() << "\t" << q.value(index_password).toString();
        login = q.value(index_login).toString();
        password = q.value(index_password).toString();
        if(login == logEdit->text() && password == pasEdit->text()){
            found = true;
            mw->user = login;
            mw->setPermissions();
            mw->show();
            this->close();
            break;
        }
    }
    if(!found){
        info->setText("Invalid Login/Password");
    }
}

void LoginWindow::restorePassword(){
    QString login, password, keyword;

    QSqlQuery q("SELECT * FROM Users", *db);

    if(!q.isActive()){
        qDebug() << "Query is not Active";
    }

    int index_login = q.record().indexOf("login");
    int index_password = q.record().indexOf("password");
    int index_keyword = q.record().indexOf("keyword");
    bool found = false;
    while(q.next()){
        //qDebug() << q.value(index_login).toString() << "\t" << q.value(index_password).toString();
        login = q.value(index_login).toString();
        password = q.value(index_password).toString();
        keyword = q.value(index_keyword).toString();
        if(login == logEdit->text()){
            found = true;
            //ForgotWindow fw(login, password, keyword, this);
            fw->setData(login, password, keyword);
            fw->show();
            this->close();
            break;
        }
    }
    if(!found){
        info->setText("Invalid Login");
    }
}
