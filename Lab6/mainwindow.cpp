#include "mainwindow.h"

MainWindow::MainWindow(QSqlDatabase *sdb, QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);
    db = sdb;
    this->setWindowTitle("Producs_Data_Base");

    cpw = new ChangePasswordWindow(db);

    model = new QSqlTableModel(this, *db);
    model->setTable("Producs_Data_Base");

    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    qDebug() << "Rows: " << model->rowCount() << endl << "Columns: " << model->columnCount();

    view = new QTableView(this);
    view->setModel(model);
    view->setGeometry(10, 25, 740, 500);
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setShowGrid(true);

    //this->addToolBar("Main");
    menu = new QMenuBar();
    m = menu->addMenu("Main");
//    m->clear();
    m->addAction("Change User", this, SLOT(returnToLoginAgain()));
    m->addAction("Change Password", cpw, SLOT(show()));
    m->addSeparator();
    actAddUser = m->addAction("Add User");
    this->setMenuBar(menu);

    eFilter = new QLineEdit(this);
    eFilter->setGeometry(10, 535, 200, 20);

    connect(eFilter, SIGNAL(textChanged(QString)), this, SLOT(setFilter()));
}

MainWindow::~MainWindow()
{
    delete model;
    delete view;
}

void MainWindow::setPermissions(){
    if(user != "admin"){
        view->setEditTriggers(QTableView::NoEditTriggers);
        actAddUser->setVisible(false);
    }
    else{
        view->setEditTriggers(QTableView::AllEditTriggers);
        actAddUser->setVisible(true);
    }
    cpw->setData(user);
}

void MainWindow::returnToLoginAgain(){
    this->close();
    emit callLoginAgain();
}

void MainWindow::setFilter(){
    QString txt = eFilter->text();

//    qDebug() << "FilterText = " << txt;
    if(txt.length() > 0){
    model->setFilter(QString("Name LIKE '%1%'").arg(txt));
    }
    else{
        model->setFilter("");
    }
    qDebug() << "Filter = " << model->filter();

    model->select();
}
