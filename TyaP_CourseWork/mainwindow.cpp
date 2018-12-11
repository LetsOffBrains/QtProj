#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	forbiden << "*" << "(" << ")" << "+" << "[" << "]" << "|" << "\\";
	separator = " ";

	toMul = 1;

	connect(ui->aTheme, SIGNAL(triggered()), this, SLOT(on_aTheme_activated()));
	connect(ui->aAuthor, SIGNAL(triggered()), this, SLOT(on_aAuthor_activated()));
	connect(ui->aSave, SIGNAL(triggered()), this, SLOT(on_aSave_activated()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_aTheme_activated()
{
	QMessageBox::about(this, "О Теме",
					   "Тема:\n"
					   "Написать программу, которая по предложенному описанию языка построит"
					   "регулярное выражение, задающее этот язык, и сгенерирует с"
					   "его помощью все цепочки языка в заданном диапазоне длин. Предусмотреть"
					   "также возможность генерации цепочек по введённому пользователем"
					   "РВ (в рамках варианта).\n"
					   "Вариант 9:\n"
					   "Алфавит, кратность вхождения некоторого символа алфавита во"
					   "все цепочки языка и заданная подцепочка всех цепочек языка.");
}

void MainWindow::on_aAuthor_activated()
{
	QMessageBox::about(this, "Об Авторе",
					   "Красильников Антон\n"
					   "Студент группы ИП-512");
}

void MainWindow::on_aSave_activated()
{

}

void MainWindow::on_leTerm_editingFinished()
{
	vTerm = ui->leTerm->text().split("", QString::SkipEmptyParts);
	vTerm.removeAll(separator);
	vTerm.removeDuplicates();
	for(int i = 0; i < forbiden.size(); ++i){
		vTerm.removeAll(forbiden.at(i));
	}
	ui->leTerm->setText(vTerm.join(separator));

	if(vTerm.size()){
		ui->cbRepeat->setEnabled(true);
		ui->cbRepeat->clear();
		ui->cbRepeat->addItems(vTerm);

		ui->leSubString->setEnabled(true);

		ui->pbCreateReg->setEnabled(true);
	}
	else{
		ui->cbRepeat->setEnabled(false);
		ui->cbRepeat->clear();

		ui->leSubString->setEnabled(false);
		ui->leSubString->clear();

		ui->pbCreateReg->setEnabled(false);
	}

}

void MainWindow::on_cbRepeat_currentIndexChanged(const QString &arg1)
{
	toRepeat = arg1;
}

void MainWindow::on_sbMul_valueChanged(int arg1)
{
	toMul = arg1;
}

void MainWindow::on_leSubString_editingFinished()
{
	QStringList list = ui->leSubString->text().split("", QString::SkipEmptyParts);
	list = list.filter(QRegExp("[" + vTerm.join("") + "]"));
	subString = list.join("");
	ui->leSubString->setText(subString);
}

void MainWindow::on_sbFrom_valueChanged(int arg1)
{
	ui->sbTo->setMinimum(arg1);
}

void MainWindow::on_pbCreateReg_clicked()
{
	QString blockRegEx;
	if(toMul > 1){
		QStringList tmp = vTerm;
		tmp.removeOne(toRepeat);
		blockRegEx = "(" + tmp.join("+") + ")*";
	}
	else{
		blockRegEx = "(" + vTerm.join("+") + ")*";
	}

	QString str;

	bool debug = true;

	if(subString.length()){
		if(toMul > 1){
			QString side = "(" + blockRegEx;
			for(int i = 0; i < toMul; ++i){
				side += toRepeat + blockRegEx;
			}
			side += ")*";
			if(debug) side += '\n';

			QString mid;
			int n = toMul - subString.count(toRepeat);
			while(n <= 0) n += toMul;
			while(n > toMul) n -= toMul;

			mid = "(";
			if(debug) mid += '\n';

			if(toMul == n){
				mid += "(" + blockRegEx + subString + blockRegEx + ")";
				if(debug) mid += '\n';
				mid += "+";
				if(debug) mid += '\n';
			}

			for(int i = (toMul > n ? 0 : 1); (toMul > n ? i <= n : i < n); ++i){
				mid += "(" + blockRegEx;
				for(int j = 0; j < i; ++j){
					mid += toRepeat + blockRegEx;
				}
				mid += subString + blockRegEx;
				for(int k = i; k < n; ++k){
					mid += toRepeat + blockRegEx;
				}
				mid += ")";
				if(toMul > n ? i < n : i < n - 1) {
					if(debug) mid += '\n';
					mid += "+";
					if(debug) mid += '\n';
				}
			}


			if(debug) mid += '\n';
			mid += ")";
			if(debug) mid += '\n';

			str = side + mid + side;
		}
		else{
			str = blockRegEx + subString + blockRegEx;
		}
	}
	else{
		if(toMul > 1){
			str = "(" + blockRegEx;
			for(int i = 0; i < toMul; ++i){
				str += toRepeat + blockRegEx;
			}
			str += ")*";
		}else{
			str = blockRegEx;
		}
	}

	ui->teRegEx->setPlainText(str);
}

void MainWindow::on_leSubString_returnPressed()
{
	on_leSubString_editingFinished();
	on_pbCreateReg_clicked();
}

void MainWindow::on_pbOutput_clicked()
{
	ui->teOutput->clear();

	QString RegEx = ui->teRegEx->toPlainText();
	RegEx.remove(' ');
	RegEx.remove('\n');
	RegEx.remove('\t');

	if(check_RegEx(RegEx)){
		mapBlocks.clear();
		split_RegEx(RegEx, "Start");
	}
	else{
		ui->teOutput->setPlainText("Не верное регулярное выражение");
	}

//	RegEx.replace('+', '|');
//	QRegExp re(RegEx);
//	if(re.isValid()){

//	}
//	else{
//		ui->teOutput->setPlainText(re.errorString());
//	}
}

bool MainWindow::check_RegEx(QString RegEx)
{
	QRegExp simbols("[" + vTerm.join("") + "]");
	for(int i = 0; i < RegEx.length(); ++i){
		if(!simbols.exactMatch(RegEx.mid(i, 1))
				&& RegEx[i] != '(' && RegEx[i] != ')'
				&& RegEx[i] != '*' && RegEx[i] != '+'){
			qDebug() << "Найден недопустимый символ в позиции: " << simbols.matchedLength();
			return false;
		}
	}

	int left = 0, right = 0, last = -1;
	for(int i = 0; i < RegEx.length(); ++i){
		if(RegEx[i] == '('){
			++left;
			last = i;
		}
		else if(RegEx[i] == ')'){
			++right;
			if(left < right){
				qDebug() << "Найдена закрывающая скобка без открывающей в позиции: " << i;
				return false;
			}
			if(i == last + 1){
				qDebug() << "Найден пустой блок скобок в позиции: " << i;
				return false;
			}
		}
		else if(RegEx[i] == '*'){
			if(i == 0){
				qDebug() << "Найдена звездочка в начале";
				return false;
			}
			else{
				if(RegEx[i - 1] == '+' || RegEx[i - 1] == '(' || RegEx[i - 1] == '*'){
					qDebug() << "Найден недопустимый символ перед звездочкой в позиции: " << i - 1;
					return false;
				}
			}
		}
		else if(RegEx[i] == '+'){
			if(i == 0){
				qDebug() << "Найден плюс в начале";
				return false;
			}
			else{
				if(RegEx[i - 1] == '('){
					qDebug() << "Найден недопустимый символ перед плюсом в позиции: " << i - 1;
					return false;
				}
			}
		}
	}
	if(left != right){
		qDebug() << "Не хватает закрывающей скобки";
		return false;
	}

	return true;
}

void MainWindow::split_RegEx(QString RegEx, QString parent)
{
	qDebug() << "RegEx: " << RegEx;
	QStringList list;
	int left = 0, right = 0, last = 0;
	if(RegEx[0] == '('){
		++left;
	}

	for(int i = 1; i <= RegEx.length(); ++i){
		if(RegEx[i] == '('){
			if(left == right && i != last){
				list << RegEx.mid(last, i - last);
				last = i;
				left = right = 0;
			}
			++left;
		}
		else if(RegEx[i] == ')'){
			++right;
		}
		else if(left == right && i != last){
			if(RegEx[i] == '*'){
				list << RegEx.mid(last, i - last + 1);
				last = i + 1;
				if(RegEx[i - 1] == ')'){
					left = right = 0;
				}
			}
			else{
				list << RegEx.mid(last, i - last);
				last = i;
			}
		}
	}

	if(list.count() > 1){
		qDebug() << "BlockList:" << list;
		for(int i = 0; i < list.count(); ++i){
			if(list.at(i).length() > 1){
				split_RegEx(list.at(i), RegEx);
			}
			else{
				mapBlocks.insert(parent, QStringList(list.at(i)));
			}
		}
	}
	else{
		QString str = list.at(0);
		if(str.left(1) == "("){
			str.remove(0, 1);
			if(str.right(1) == ")"){
				str.chop(1);
				split_RegEx(str, RegEx);
			}
			else if(str.right(2) == ")*"){
				str.chop(2);
				split_RegEx(str, RegEx);
			}
		}
		else{
			mapBlocks.insert(parent, QStringList(str));
		}
	}
}

















