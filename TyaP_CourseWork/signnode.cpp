#include "signnode.h"

SignNode::SignNode(QString RegEx)
{
	QStringList list = split_to_blocks(RegEx);

	if(list.count() > 1){
		if(list.contains("+")){
			QStringList newList;
			QString str;
			for(auto i : list){
				if(i != "+"){
					str += i;
				}
				else{
					newList << str;
					str.clear();
				}
			}
			newList << str;
			qDebug() << "BlockList:" << newList;
			sign = "+";
			for(auto i : newList){
				nodes << SignNode(i);
			}
		}
		else{
			qDebug() << "BlockList:" << list;
			sign = "x";
			for(auto i : list){
				nodes << SignNode(i);
			}
		}
	}
	else{
		QString str = list.at(0);
		if(str.left(1) == "("){
			qDebug() << "Block:" << str;
			str.remove(0, 1);
			if(str.right(1) == ")"){
				str.chop(1);
				*this = SignNode(str);
			}
			else if(str.right(2) == ")*"){
				str.chop(2);
				sign = "*";
				nodes << SignNode(str);
			}
		}
		else if(str.right(1) == "*"){
			qDebug() << "Block:" << str;
			str.chop(1);
			sign = "*";
			nodes << SignNode(str);
		}
		else{
			sign.clear();
			simbol = str;
		}
	}
}

QStringList SignNode::split_to_blocks(QString RegEx)
{
	//	qDebug() << "RegEx: " << RegEx;
	if(RegEx.size() == 1){
		return QStringList(RegEx);
	}
	QStringList list;
	int left = 0, right = 0, start = -1, i = 0;

	if(RegEx[0] == '('){
		++left;
		++i;
		start = 0;
	}

	for(; i < RegEx.size(); ++i){
		if(RegEx[i] == '('){
			if(start == -1) start = i;
			++left;
		}
		else if(RegEx[i] == ')'){
			++right;
			if(left == right){
				if(i + 1 < RegEx.size()){
					if(RegEx[i + 1] == '*'){
						list << RegEx.mid(start, i - start + 2);
						++i;
						start = -1;
					}
					else{
						list << RegEx.mid(start, i - start + 1);
						start = -1;
					}
				}
				else{
					list << RegEx.mid(start, i - start + 1);
					start = -1;
				}
			}
		}
		else if(start == -1){
			if(i + 1 < RegEx.size()){
				if(RegEx[i + 1] == '*'){
					list << RegEx.mid(i, 2);
					++i;
				}
				else{
					list << RegEx.mid(i, 1);
				}
			}
			else{
				list << RegEx.mid(i, 1);
			}
		}
	}

	//	qDebug() << "SplitList:" << list;
	return list;
}

void SignNode::toTreeWidget(QTreeWidgetItem *parent)
{
	if(nodes.size() && !sign.isEmpty()){
		//		qDebug() << "Sign: " << sign << " \tSize: " << nodes.size();
		QTreeWidgetItem *tree = new QTreeWidgetItem(parent, QStringList(sign));
		for(auto i : nodes){
			i.toTreeWidget(tree);
		}
	}
	else{
		//		qDebug() << simbol;
		parent->addChild(new QTreeWidgetItem(QStringList(simbol)));
	}
}

QStringList SignNode::generate(int min, int max, QMap<QString, QStringList> &mapSave)
{
	QStringList list;

	if(nodes.size() && !sign.isEmpty()){
		if(sign == "+"){
			for(auto i : nodes){
				list << i.generate(min, max, mapSave);
			}
			qDebug() << "Or:" << list;
		}
		else if(sign == "x"){
			list = nodes[0].generate(min, max, mapSave);

			for(int i = 1; i < nodes.size(); i++){
				QStringList temp = nodes[i].generate(min, max, mapSave);
				list = concatenate(list, temp, max);
			}
			qDebug() << "C:" << list;
		}
		else if(sign == "*"){
			QString block = makeBlock();
			if(mapSave.contains(block)){
				list = mapSave[block];
			}
			else{
				list = nodes[0].generate(min, max, mapSave);
				list = multiply(list, max);
				mapSave[block] = list;
			}
			qDebug() << "M:" << list;
		}
	}
	else{
		list << simbol;
	}
	return list;
}

QStringList SignNode::concatenate(QStringList &left, QStringList &right, int max)
{
	QStringList result;
	for(auto i : left){
		for(auto j : right){
			QString tmp = i + j;
			if(tmp.size() <= max)
				result << i + j;
		}
	}
	result.removeDuplicates();
//	qDebug() << "Conc:" << result;
	return result;
}

QStringList SignNode::multiply(QStringList &list, int max)
{
	QStringList result = list;
	int i = 0;
	while(i < result.size()){
		if(result[i].size() < max){
			for(auto j : list){
				QString str = result[i] + j;
				if(str.size() <= max)
					result << str;
			}
		}
		result.removeDuplicates();
		++i;
	}
	result << "";
//	qDebug() << "Mult:" << result;
	return result;
}

QString SignNode::makeBlock()
{
	QString result;

	if(nodes.size() && !sign.isEmpty()){
		if(sign == "+"){
			QStringList temp;
			for(auto i : nodes){
				temp << i.makeBlock();
			}
			result = temp.join("+");
		}
		else if(sign == "x"){
			QStringList temp;
			for(auto i : nodes){
				if(i.sign == "+")
					temp << "(" + i.makeBlock() + ")";
				else
					temp << i.makeBlock();
			}
			result = temp.join("");
		}
		else if(sign == "*"){
			if(nodes[0].sign.isEmpty())
				result = nodes[0].simbol + "*";
			else
				result = "(" + nodes[0].makeBlock() + ")*";
		}
	}
	else{
		result = simbol;
	}

	return result;
}
