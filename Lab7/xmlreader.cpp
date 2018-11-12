#include "xmlreader.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

XMLReader::XMLReader(QWidget *parent)
    : QTreeWidget(parent)
{
    hide();
    resize(parent->width(), parent->height());
}

bool XMLReader::read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QTreeWidgetItem *treeRootItem = new QTreeWidgetItem;
    QDomElement root = domDocument.documentElement();

    clear();

//    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    if(!root.isNull()) {
        QStringList labels;
        labels << tr("Tag") << tr("Value");
        header()->setResizeMode(QHeaderView::Stretch);
        setHeaderLabels(labels);
//        treeRootItem->setText(0, root.tagName());
        parseFolderElement(&root, treeRootItem);
    }

//    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    this->addTopLevelItem(treeRootItem);
    treeRootItem->setExpanded(true);

    show();

    return true;
}

void XMLReader::parseFolderElement(const QDomNode *node, QTreeWidgetItem *treeRootItem)
{
    treeRootItem->setText(0, node->toElement().tagName());
    QDomNode domNode = node->firstChild();
    while (!domNode.isNull()){
        QTreeWidgetItem *treeSiblingItem = new QTreeWidgetItem;
        bool hasChildren = false;
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()){
                treeSiblingItem->setText(0, domElement.tagName());
                if (domElement.hasChildNodes()){
                    QDomNodeList domNodeList = domElement.childNodes();
                    int countChildren = domNodeList.length();

                    if(countChildren > 1){
                        hasChildren = true;
//                        qDebug() << "TagName: " << domElement.tagName() << "\tChildren: " << countChildren;
                    }
                    else{
                        treeSiblingItem->setText(1, domElement.text());
//                        qDebug() << "TagName: " << domElement.tagName() << "\tText: " << domElement.text();
                    }
                }
                treeRootItem->addChild(treeSiblingItem);
            }
        }
        else{
            qDebug() << "Non Element Node";
        }
        if(hasChildren){
            parseFolderElement(&domNode, treeSiblingItem);
            treeRootItem->addChild(treeSiblingItem);
        }
        domNode = domNode.nextSibling();
    }
}
