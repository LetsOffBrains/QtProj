#include "saxhandler.h"
#include <QDebug>
#include <QHeaderView>

saxHandler::saxHandler(QTreeWidget *treeWidget)
{
    tree = treeWidget;
    QStringList labels;
    labels << "Tag" << "Value";
    tree->header()->setResizeMode(QHeaderView::Stretch);
    tree->setHeaderLabels(labels);
}

bool saxHandler::startElement(const QString &, const QString &localName, const QString &, const QXmlAttributes &)
{
//    qDebug() << "Start localName = " << localName;
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, localName);
    items << item;

    return true;
}

bool saxHandler::characters(const QString &ch)
{
    if(ch[0] != QChar('\n')){
//        qDebug() << "ch = " << ch;
        items.last()->setText(1, ch);
    }

    return true;
}

bool saxHandler::endElement(const QString &, const QString &localName, const QString &)
{
    QTreeWidgetItem *item = items.last();

    items.removeLast();
    if(!items.isEmpty()){
        if(localName != items.last()->text(0)){
//            qDebug() << "End localName = " << localName << " is a \tchild of " << items.last()->text(0);
            items.last()->addChild(item);
        }
    }
    else{
//        qDebug() << "End localName = " << localName << " is a \troot " << tree->text(0);
        tree->addTopLevelItem(item);
    }

    return true;
}

bool saxHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() << "Line: " << exception.lineNumber() <<
                ", Column: " << exception.columnNumber() <<
                ", Message: " << exception.message();

    return false;
}
