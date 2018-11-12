#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    answersGiven = new int[questionCount];
    answersRight = new int[questionCount];

    file.setFileName("../Testing/quiz.txt");

//    qDebug() << QDir::current();

    if(file.open(QIODevice::ReadOnly)){
        int j = 0;
        while(!file.atEnd()){
            qDebug() << "Tab #" << ++j;
            vTemplate.push_back(new TemplateWidget);

            QString task = tr(file.readLine());
            task.chop(1);
            vTemplate.last()->ui->lTask->setText(task);

            int count, right;
            QString ints = tr(file.readLine());
            QStringList splitInts = ints.split(" ");
            count = splitInts.at(0).toInt();
            right = splitInts.at(1).toInt();

            for(int i = 0; i < count; ++i){
                QString answer = tr(file.readLine());
                answer.chop(1);
                qDebug() << answer;
            }

            ui->tabWidget->addTab(vTemplate.last(), "Q");

            file.readLine();
        }
    }
    else{
        qDebug() << "Can't open file";
    }
}

TestWindow::~TestWindow()
{
    delete ui;
}
