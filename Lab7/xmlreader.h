#ifndef XMLREADER_H
#define XMLREADER_H

#include <QWidget>
#include <QTreeWidget>
#include <QIODevice>
#include <QtXml/QDomDocument>
#include <QHash>

class XMLReader : public QTreeWidget
{
    Q_OBJECT
public:
    explicit XMLReader(QWidget *parent = 0);
    
    bool read(QIODevice *device);

signals:
    
public slots:

private:
    void parseFolderElement(const QDomNode *node, QTreeWidgetItem *treeRootItem);

    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomNode> domElementForItem;
    
};

#endif // XMLREADER_H
