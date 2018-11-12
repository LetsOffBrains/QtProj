#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTabWidget>
#include <QVector>
#include <QRadioButton>
#include <QAbstractButton>
#include "templatewidget.h"
#include "questionstruct.h"

namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();
    
public slots:
    void colorHighlight();

private:
    Ui::TestWindow *ui;

    int questionCount;
    int* answersGiven;

    QFile file;

    QList<QuestionStruct> questions;

    QVector<TemplateWidget*> vTemplate;
};

#endif // TESTWINDOW_H
