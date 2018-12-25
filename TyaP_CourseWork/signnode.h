#ifndef SIGNNODE_H
#define SIGNNODE_H
#include <QString>
#include <QList>
#include <QtDebug>
#include <QTreeWidgetItem>

class SignNode
{
public:
	QString sign;
	QString simbol;
	QList<SignNode> nodes;

	SignNode(QString str);

	QStringList split_to_blocks(QString str);

	void toTreeWidget(QTreeWidgetItem *parent);

	QStringList generate(int min, int max, QMap<QString, QStringList> &mapSave);
	static QStringList concatenate(QStringList &left, QStringList &right, int max);
	static QStringList multiply(QStringList &list, int max);
	QString makeBlock();
};

#endif // SIGNNODE_H
