#ifndef TEMPLATEWIDGET_H
#define TEMPLATEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QVector>
#include <QRadioButton>
#include "questionstruct.h"
#include "ui_templatewidget.h"

namespace Ui {
class TemplateWidget;
}

class TemplateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateWidget(QWidget *parent = 0, QuestionStruct qs = QuestionStruct());
    ~TemplateWidget();

    Ui::TemplateWidget *ui;

    QVector<QRadioButton*> vRadioAnswers;

    QuestionStruct question;
private:

    
};

#endif // TEMPLATEWIDGET_H
