#include "templatewidget.h"
#include "ui_templatewidget.h"

TemplateWidget::TemplateWidget(QWidget *parent, const QuestionStruct qs) :
    QWidget(parent), question(qs),
    ui(new Ui::TemplateWidget)
{
    ui->setupUi(this);

    ui->lTask->setText(question.task);

    for(int i = 0; i < question.answers.size(); ++i){
        vRadioAnswers.push_back(new QRadioButton(question.answers.at(i)));
        ui->vlAnswers->addWidget(vRadioAnswers.last());
    }
}

TemplateWidget::~TemplateWidget()
{
    delete ui;
}

