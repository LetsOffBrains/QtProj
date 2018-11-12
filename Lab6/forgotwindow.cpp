#include "forgotwindow.h"

ForgotWindow::ForgotWindow(QWidget *parent) :
    QWidget(parent)
{
    this->resize(100, 100);
    this->setWindowTitle(login);

    info = new QLabel("Enter KeyWord", this);
    info->setGeometry(10, 10, 80, 20);

    keyEdit = new QLineEdit("KeyWord", this);
    keyEdit->setGeometry(10, 40, 80, 20);

    bClose = new QPushButton("Close", this);
    bClose->setGeometry(20, 70, 60, 20);

    connect(keyEdit, SIGNAL(returnPressed()), this, SLOT(checkKeyWord()));
    connect(bClose, SIGNAL(clicked()), this, SLOT(returnToLogin()));
}

void ForgotWindow::checkKeyWord(){
    if(keyEdit->text() == keyword){
        info->setText(password);
    }
    else{
        info->setText("Invalid KeyWord");
    }
}

void ForgotWindow::setData(QString log, QString pas, QString key){
    login = log;
    password = pas;
    keyword = key;
}

void ForgotWindow::returnToLogin(){
    this->close();
    emit callLoginAgain();
}
