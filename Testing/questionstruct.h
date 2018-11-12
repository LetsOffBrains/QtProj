#ifndef QUESTIONSTRUCT_H
#define QUESTIONSTRUCT_H

#include <QStringList>
#include <QString>

class QuestionStruct
{
public:
    QuestionStruct(){}
    QuestionStruct(QString t, int c, int r, QStringList a);

    QString task;
    int count;
    int right;
    QStringList answers;
};

#endif // QUESTIONSTRUCT_H
