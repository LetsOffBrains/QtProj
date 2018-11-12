#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    answersGiven = new int[questionCount];

    file.setFileName("../Testing/quiz.txt");

//    qDebug() << QDir::current();

    if(file.open(QIODevice::ReadOnly)){
        int j = 0;
        while(!file.atEnd()){
            qDebug() << "Tab #" << ++j;

            QString task = tr(file.readLine());
            task.chop(1);

            int count, right;
            QString ints = tr(file.readLine());
            QStringList splitInts = ints.split(" ");
            count = splitInts.at(0).toInt();
            right = splitInts.at(1).toInt();

            QStringList answers;

            for(int i = 0; i < count; ++i){
                QString answer = tr(file.readLine());
                answer.chop(1);
                answers << answer;
            }

            questions << QuestionStruct(task, count, right, answers);

            vTemplate.push_back(new TemplateWidget(0, questions.last()));
            ui->tabWidget->addTab(vTemplate.last(), tr("Вопрос №") + QString::number(j));

            file.readLine();
        }
    }
    else{
        qDebug() << "Can't open file";
    }

    connect(vTemplate.last()->ui->pbNext, SIGNAL(released()), this, SLOT(colorHighlight()));
}

TestWindow::~TestWindow()
{
    delete ui;
    delete answersGiven;
}

void TestWindow::colorHighlight()
{
    qDebug() << tr("Начинаем окрашивание");
    for(int i = 0; i < vTemplate.size() ;++i){
        TemplateWidget* itW = vTemplate.at(i);

        for(int j = 0; j < itW->vRadioAnswers.size() ;++j){
            QRadioButton* rb = itW->vRadioAnswers.at(j);
            if(j + 1 == itW->question.right){
                if(rb->isChecked()){
                    rb->setStyleSheet("QRadioButton { background: green; color: black}");
                    answersGiven[i] = j + 1;
                }
                else{
                    rb->setStyleSheet("QRadioButton { background: yellow; color: black}");
                }
            }
            else if(rb->isChecked()){
                rb->setStyleSheet("QRadioButton { background: red; color: black}");
                answersGiven[i] = j + 1;
            }
            rb->setEnabled(false);
        }
//        qDebug() << answersGiven[i];
    }
    qDebug() << tr("Закончили окрашивание");
}
