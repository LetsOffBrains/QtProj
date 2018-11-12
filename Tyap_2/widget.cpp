#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

	Nil = "@";
	Sep = " ";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_leTerm_editingFinished()
{
	Simbols = ui->leTerm->text().split(Sep, QString::SkipEmptyParts);
	if(Simbols.removeDuplicates() || Simbols.removeAll(Nil)){
		ui->leTerm->setText(Simbols.join(Sep));
	}

	build_rules_table();
}

void Widget::on_leEmpty_editingFinished()
{
	if(ui->leEmpty->text().isEmpty()){
		ui->leEmpty->setText(Nil);
	}
	else{
		Nil = ui->leEmpty->text();
	}
}

void Widget::on_leStates_editingFinished()
{
	States = ui->leStates->text().split(Sep, QString::SkipEmptyParts);

	if(States.removeDuplicates() || States.removeAll(Nil)){
		ui->leStates->setText(States.join(Sep));
	}

	ui->cbStart->clear();
	ui->cbStart->addItems(States);

	build_rules_table();
}

void Widget::on_cbStart_currentIndexChanged(const QString &arg1)
{
	if(arg1.isEmpty()){
		ui->cbStart->setEnabled(false);
	}
	else{
		ui->cbStart->setEnabled(true);
	}
	StartState = arg1;
//	qDebug() << "StartState: " << StartState;
}

void Widget::on_leFinish_editingFinished()
{
	FinishStates = ui->leFinish->text().split(Sep, QString::SkipEmptyParts);
	bool flag = false;

	QStringList::iterator it = FinishStates.begin();
	while(it != FinishStates.end()){
		if(!States.contains(*it)){
			FinishStates.removeAll(*it);
			flag = true;
		}
		else{
			++it;
		}
	}

	if(FinishStates.removeDuplicates() || flag){
		ui->leFinish->setText(FinishStates.join(Sep));
	}

	enable_input();
}

void Widget::build_rules_table()
{
	// Deleting
	while(ui->tableWidget->columnCount()){
		ui->tableWidget->removeColumn(0);
	}

	while(ui->tableWidget->rowCount()){
		ui->tableWidget->removeRow(0);
	}

	// Inserting
	for(int i = 0; i < Simbols.size(); ++i){
		ui->tableWidget->insertColumn(0);
	}
	ui->tableWidget->setHorizontalHeaderLabels(Simbols);

	for(int i = 0; i < States.size(); ++i){
		ui->tableWidget->insertRow(0);
		for(int j = 0; j < ui->tableWidget->columnCount(); ++j){
			ui->tableWidget->setItem(i, j, new QTableWidgetItem(""));
		}
	}
	ui->tableWidget->setVerticalHeaderLabels(States);

	//Insert old values
	QMap<QString, QMap<QString, QString> >::iterator itStates = Rules.begin();
	while(itStates != Rules.end()){
//		qDebug() << "Working on State:" << itStates.key();
		if(States.contains(itStates.key())){ // Found State
//			qDebug() << "Found State:" << itStates.key();
			QMap<QString, QString>::iterator itSimbols = Rules[itStates.key()].begin();
			while(itSimbols != Rules[itStates.key()].end()){ // Found Simbol
//				qDebug() << "Found Simbol:" << itSimbols.key();
				if(Simbols.contains(itSimbols.key())){
					ui->tableWidget->setItem(
						States.indexOf(itStates.key()),
						Simbols.indexOf(itSimbols.key()),
						new QTableWidgetItem(Rules[itStates.key()][itSimbols.key()]));
				}
				else{
//					qDebug() << "Did not found Simbol:" << itSimbols.key();
					Rules[itStates.key()].remove(itSimbols.key());
				}
				++itSimbols;
			}
		}
		else{
//			qDebug() << "Did not found State:" << itStates.key();
			Rules.remove(itStates.key());
		}
		++itStates;
//		qDebug() << "Complete working on State:" << itStates.key();
	}

	enable_input();
}

void Widget::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
	QString text = item->text();
	if(States.contains(text)){
		QString sm = Simbols.at(item->column());
		QString st = States.at(item->row());

		Rules[st][sm] = text;
	}
	else{
		item->setText("");
	}

	enable_input();
}

void Widget::enable_input()
{
	if(check_input_to_proceed()){
		ui->leInput->setEnabled(true);
	}
	else{
		ui->leInput->setEnabled(false);
//		ui->leInput->setText(QString::fromUtf8("РљРѕСЂСЂРµРєС‚РЅРѕ Р·Р°РїРѕР»РЅРёС‚Рµ РІСЃРµ РїРѕР»СЏ СЃР»РµРІР°"));
	}
	ui->leInput->setText("");
}

bool Widget::check_input_to_proceed()
{
	if(Simbols.isEmpty() || States.isEmpty() || FinishStates.isEmpty()
			|| Rules.count() != States.count())
	{
		return false;
	}

	QMap<QString, QMap<QString, QString> >::iterator itStates = Rules.begin();
	while(itStates != Rules.end()){
		if(itStates.value().count() != Simbols.count()){
			return false;
		}
		++itStates;
	}

	return true;
}

void Widget::on_leInput_editingFinished()
{
	QString currentState = ui->cbStart->currentText();
	QString currentText = ui->leInput->text();
	QString firstSimbol = currentText.left(1);
	QStringList steps;
	while(!currentText.isEmpty()){
		steps << QString(
			 "(" + currentState + ", " + firstSimbol + ")\t" + currentText
		);
		if(!Simbols.contains(firstSimbol) && !Simbols.contains(Nil)){
			QString err = QString::fromUtf8("Символ ") + firstSimbol
					+ QString::fromUtf8(" не входит в алфавит");
			steps << err;
			steps << QString::fromUtf8("Цепочка не принадлежит данному языку");
			break;
		}
		else if(!Simbols.contains(firstSimbol) && Simbols.contains(Nil)){
			currentState = Rules[currentState][Nil];
			steps << QString::fromUtf8("Пустой такт");
		}
		else{
			currentState = Rules[currentState][firstSimbol];
			currentText.remove(0, 1);
			firstSimbol = currentText.left(1);
		}
	}

	if(currentText.isEmpty()){
		steps << QString(
			"(" + currentState + ", " + Nil + ")\t" + Nil
		);
		if(FinishStates.contains(currentState)){
			steps << QString::fromUtf8("Цепочка прочитана полностью, состояние конечное");
			steps << QString::fromUtf8("Цепочка принадлежит данному языку");
		}
		else{
			steps << QString::fromUtf8("Цепочка прочитана полностью, но состояние не конечное");
			steps << QString::fromUtf8("Цепочка не принадлежит данному языку");
		}
	}


	ui->listWidget->clear();
	ui->listWidget->addItems(steps);
}
