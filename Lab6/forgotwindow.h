#ifndef FORGOTWINDOW_H
#define FORGOTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QPushButton>

class ForgotWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ForgotWindow(QWidget *parent = 0);
    
private:
    QLabel *info;
    QLineEdit *keyEdit;
    QPushButton *bClose;
    QString login, password, keyword;

public:
    void setData(QString log, QString pas, QString key);
    
public slots:
    void checkKeyWord();
    void returnToLogin();

signals:
    void callLoginAgain();
};

#endif // FORGOTWINDOW_H
