#ifndef SAXHANDLER_H
#define SAXHANDLER_H

#include <QXmlDefaultHandler>
#include <QXmlAttributes>
#include <QXmlParseException>
#include <QString>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QXmlAttributes>
#include <QList>

class saxHandler : public QXmlDefaultHandler
{
public:
    saxHandler(QTreeWidget *treeWidget = 0);
    bool startElement(
            const QString & ,
            const QString & localName,
            const QString & ,
            const QXmlAttributes &  );
    bool characters(
            const QString & ch );
    bool endElement(
            const QString & ,
            const QString & localName,
            const QString &  );
    bool fatalError(
            const QXmlParseException & exception );
private:
    QTreeWidget *tree;
    QList<QTreeWidgetItem*> items;

};

#endif // SAXHANDLER_H
