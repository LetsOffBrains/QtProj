#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	nil = "@";
	sep = " ";
	modified = true;

	term = ui->leTerm->text().split(sep, QString::SkipEmptyParts);
	mag = ui->leMag->text().split(sep, QString::SkipEmptyParts);
	states = ui->leStates->text().split(sep, QString::SkipEmptyParts);
	finish = ui->leFinish->text().split(sep, QString::SkipEmptyParts);
	on_leStates_editingFinished();
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_leTerm_editingFinished()
{
	term = ui->leTerm->text().split(sep, QString::SkipEmptyParts);
	term.removeDuplicates();
	term.removeAll(nil);
	ui->leTerm->setText(term.join(sep));
	//check rules
	check_all_rules();
}

void Widget::on_leMag_editingFinished()
{
	mag = ui->leMag->text().split(sep, QString::SkipEmptyParts);
	mag.removeDuplicates();
	ui->leMag->setText(mag.join(sep));

	//check startMag
	on_leStartMag_editingFinished();
	//check rules
	check_all_rules();
}

void Widget::on_leStates_editingFinished()
{
	states = ui->leStates->text().split(sep, QString::SkipEmptyParts);
	states.removeDuplicates();
	ui->leStates->setText(states.join(sep));


	ui->cbStart->clear();
	if(states.size()){
		ui->cbStart->addItems(states);
		ui->cbStart->setEnabled(true);
	}
	else{
		ui->cbStart->setEnabled(false);
	}
	//check rules
	check_all_rules();
}

void Widget::on_leFinish_editingFinished()
{
	finish = ui->leFinish->text().split(sep, QString::SkipEmptyParts);
	int i = 0;
	while(i < finish.size()){
		QString tmp = finish.at(i);
		if(states.contains(tmp)){
			++i;
		}
		else{
			finish.removeAll(tmp);
		}
	}

	finish.removeDuplicates();
	ui->leFinish->setText(finish.join(sep));
}

void Widget::on_cbStart_currentIndexChanged(const QString &arg1)
{
	if(arg1.isEmpty()){
		ui->cbStart->setEnabled(false);
	}
	else{
		ui->cbStart->setEnabled(true);
	}
	start = arg1;
}

void Widget::on_leStartMag_editingFinished()
{
	QString txt = ui->leStartMag->text();
	if(txt.isEmpty() && mag.size()){
		ui->leStartMag->setText(mag.at(0));
	}

	for(int i = 0; i < mag.size(); ++i){
		txt.remove(mag.at(i));
	}
	if(!txt.isEmpty()){
		QString right = ui->leStartMag->text();
		for(int i = 0; i < txt.size(); ++i){
			right.remove(txt.at(i));
		}
		ui->leStartMag->setText(right);
	}
}

void Widget::on_pbAdd_clicked()
{
	int row = ui->twRules->rowCount();
	ui->twRules->insertRow(row);
	ui->twRules->setItem(row, 0, new QTableWidgetItem(QString()));
	ui->twRules->setItem(row, 1, new QTableWidgetItem(QString()));
	check_all_rules();
}

void Widget::on_pbDelete_clicked()
{
	ui->twRules->removeRow(ui->twRules->currentRow());
	check_all_rules();
}

void Widget::on_twRules_cellChanged(int row, int column)
{
	QStringList list = ui->twRules->item(row, column)->text().split(sep, QString::SkipEmptyParts);
//	qDebug() << row << ";" << column;
//	qDebug() << "Size = " << list.size();
	if(column == 0){ // Left side of rule
		if(list.size() != 3){
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}

		if(!states.contains(list.at(0))){ // State
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}

		if(list.at(1) != nil && !term.contains(list.at(1))){ // Term
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}

		if(!mag.contains(list.at(2))){ // Mag
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}
	}
	else if(column == 1){ // Right side of rule
		if(list.size() != 2){
//			qDebug() << "Size != 2 =" << list.size();
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}

		if(!states.contains(list.at(0))){ // State
//			qDebug() << "No State:" << list.at(0);
			ui->twRules->item(row, column)->setText("");
			ui->twRules->item(row, column)->setBackground(Qt::red);
			return;
		}

		QString tmp = list.at(1); // Mag
		if(tmp != nil && !mag.contains(tmp)){
//			qDebug() << tmp << " !=" << nil;
			QStringList leftSide = ui->twRules->item(row, 0)->text().split(sep, QString::SkipEmptyParts);
			if(leftSide.size() == 3){
				QString leftMag = leftSide.at(2);
//				qDebug() << "leftMag =" << leftMag << leftMag.size();
				if(tmp.right(leftMag.size()) == leftMag){
					tmp.chop(leftMag.size());
//					qDebug() << "New tmp =" << tmp;
					if(!mag.contains(tmp)){
						ui->twRules->item(row, column)->setText("");
						ui->twRules->item(row, column)->setBackground(Qt::red);
						return;
					}
				}
				else{
					ui->twRules->item(row, column)->setText("");
					ui->twRules->item(row, column)->setBackground(Qt::red);
					return;
				}
			}
			else{
				ui->twRules->item(row, column)->setText("");
				ui->twRules->item(row, column)->setBackground(Qt::red);
				return;
			}
		}
	}

	if(row % 2){
		ui->twRules->item(row, column)->setBackground(Qt::white);
	}
	else{
		ui->twRules->item(row, column)->setBackground(Qt::lightGray);
	}
}

void Widget::check_all_rules()
{
	for(int i = 0; i < ui->twRules->rowCount(); ++i){
		for(int j = 0; j < ui->twRules->columnCount(); ++j){
			on_twRules_cellChanged(i, j);
		}
	}
	modified = true;
}

int Widget::create_rules()
{
	rules.clear();
	for(int i = 0; i < ui->twRules->rowCount(); ++i){
		QStringList left = ui->twRules->item(i, 0)->text().split(sep, QString::SkipEmptyParts);
		QStringList right = ui->twRules->item(i, 1)->text().split(sep, QString::SkipEmptyParts);
		if(left.size() == 3 && right.size() == 2){
			rules[left.at(0)][left.at(1)][left.at(2)] = right;
		}
	}
	return rules.size();
}

void Widget::on_leInput_editingFinished()
{
	if(modified){ // Create rules
		qDebug() << "Modified!";
		if(!create_rules()){
			qDebug() << "No rules!";
			return;
		}
		modified = false;
	}

	ui->lwOutput->clear();

	bool flag = true;
	QString curText = ui->leInput->text();
	ui->lwOutput->clear();
	QStringList allSteps;
	QString curState = start;
	QString curMag = ui->leStartMag->text();
	while(!curMag.isEmpty() && flag){ /*!curText.isEmpty() && */
		QString step = "(" + curState + ", " + curText + ", " + curMag + ")";
		QString first;
		if(!curText.isEmpty()){
			first= curText.left(1);
		}
		else{
			first = nil;
		}
		QString m = curMag.left(1);

		if(first != nil && term.contains(first) && rules[curState].contains(first)){ // Found simbol alright
			if(rules[curState][first].contains(m)){ // Found mag Simbol too
				QStringList right = rules[curState][first][m];
				curState = right.at(0);
				if(right.at(1) == nil){ // Delete
					curMag.remove(0, 1);
				}
				else if(mag.contains(right.at(1))){ // Nothing

				}
				else{ // Split to Add
					QString tmp = right.at(1);
					tmp.chop(m.size());
					curMag.insert(0, tmp);
				}
				step += " -> (" + right.join(", ") + ") ->";
				ui->lwOutput->addItem(step);
				curText.remove(0, 1);
			}
			else{ // Did not founf mag simbol
				ui->lwOutput->addItem(step);
				allSteps << QString::fromUtf8("Не найден переход для текущего состояния с таким содержимым магазина");
				flag = false;
				break;
			}
		}
		else if(rules[curState].contains(nil)){ // Going for empty step
			qDebug() << "Going for empty step";
			if(rules[curState][nil].contains(m)){ // Found mag Simbol for Empty Step
				qDebug() << "Found mag Simbol for Empty Step" << curMag;
				QStringList right = rules[curState][nil][m];
				curState = right.at(0);
				if(right.at(1) == nil){ // Delete
					curMag.remove(0, 1);
				}
				else if(mag.contains(right.at(1))){ // Nothing

				}
				else{ // Split to Add
					QString tmp = right.at(1);
					tmp.chop(m.size());
					curMag.insert(0, tmp);
				}
				step += " -> (" + right.join(", ") + ") ->";
				ui->lwOutput->addItem(step);
			}
			else{ // Did not found mag Simbol for Empty Step
				qDebug() << "Did not found mag Simbol for Empty Step";
				ui->lwOutput->addItem(step);
				allSteps << QString::fromUtf8("Не найден переход для текущего состояния с таким содержимым магазина");
				flag = false;
				break;
			}
		}
		else{ // No simbol no empty step
			ui->lwOutput->addItem(step);
			allSteps << QString::fromUtf8("Не найден символ и пустой переход для текущего состояния");
			flag = false;
			break;
		}
	}

	QString rest = "(" + curState + ", ";

	bool Alright = true;
	if(curText.isEmpty()){
		allSteps << QString::fromUtf8("Строка прочитана полностью");
		rest += nil + ", ";
	}
	else{
		allSteps << QString::fromUtf8("Строка не дочитана");
		Alright = false;
		rest += curText + ", ";
	}

	if(curMag.isEmpty()){
		allSteps << QString::fromUtf8("Магазин пуст");
		rest += nil + ")";
	}
	else{
		allSteps << QString::fromUtf8("Магазин не пуст");
		Alright = false;
		rest += curMag + ")";
	}

	if(finish.contains(curState)){
		allSteps << QString::fromUtf8("Состояние конечное");
	}
	else{
		allSteps << QString::fromUtf8("Состояние не конечное");
		Alright = false;
	}

	if(Alright){
		allSteps << QString::fromUtf8("Строка принята");
	}
	else{
		allSteps << QString::fromUtf8("Строка не принята");
	}

	ui->lwOutput->addItem(rest);

	ui->lwOutput->addItems(allSteps);
}


