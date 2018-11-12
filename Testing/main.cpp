#include <QtGui/QApplication>
#include "testwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    TestWindow w;
    w.show();
    
    return a.exec();
}
