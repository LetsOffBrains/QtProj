#include "questionstruct.h"

QuestionStruct::QuestionStruct(QString t, int c, int r, QStringList a):
    task(t), count(c), right(r), answers(a)
{
}
