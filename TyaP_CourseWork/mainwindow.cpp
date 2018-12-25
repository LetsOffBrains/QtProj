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
}

MainWindow::~MainWindow()
{
	delete ui;
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
	ui->lwOutput->clear();

	QString RegEx = ui->teRegEx->toPlainText();
	RegEx.remove(' ');
	RegEx.remove('\n');
	RegEx.remove('\t');

	if(check_RegEx(RegEx)){
		oldTerm = vTerm;
		oldToRepeat = toRepeat;
		oldSubString = subString;
		oldToMul = toMul;

		qDebug() << "Making SignTree";
		signTree = new SignNode(RegEx);
		qDebug() << "Making TreeWidget";
		ui->treeWidget->clear();
		ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList("Start")));
		signTree->toTreeWidget(ui->treeWidget->topLevelItem(0));
		qDebug() << "Showing TreeWidget";
		ui->treeWidget->expandAll();

		qDebug() << "Generating strings";

		QStringList temp = signTree->generate(ui->sbFrom->value(), ui->sbTo->value(), mapSave);
//		qDebug() << "Strings generated:" << temp;
		mapSave.clear();
		cutResult(temp, ui->sbFrom->value(), ui->sbTo->value());
//		qDebug() << "Strings cut:" << temp;

//		ui->lwOutput->addItems(temp);
		for(auto i : temp){
			ui->lwOutput->addItem(QString::number(i.size()) + " \t" + i);
		}


		ui->lwOutput->sortItems();
	}
	else{
		ui->lwOutput->addItem("Не верное регулярное выражение");
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
//			qDebug() << "Найден недопустимый символ в позиции: " << simbols.matchedLength();
			ui->lwOutput->addItem("Найден недопустимый символ в позиции: " + QString::number(simbols.matchedLength()));
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
				ui->lwOutput->addItem("Найдена закрывающая скобка без открывающей в позиции: " + QString::number(i));
//				qDebug() << "Найдена закрывающая скобка без открывающей в позиции: " << i;
				return false;
			}
			if(i == last + 1){
				ui->lwOutput->addItem("Найден пустой блок скобок в позиции: " + QString::number(i));
//				qDebug() << "Найден пустой блок скобок в позиции: " << i;
				return false;
			}
		}
		else if(RegEx[i] == '*'){
			if(i == 0){
				ui->lwOutput->addItem("Найдена звездочка в начале");
//				qDebug() << "Найдена звездочка в начале";
				return false;
			}
			else{
				if(RegEx[i - 1] == '+' || RegEx[i - 1] == '(' || RegEx[i - 1] == '*'){
					ui->lwOutput->addItem("Найден недопустимый символ перед звездочкой в позиции: " + QString::number(i - 1));
//					qDebug() << "Найден недопустимый символ перед звездочкой в позиции: " << i - 1;
					return false;
				}
			}
		}
		else if(RegEx[i] == '+'){
			if(i == 0){
				ui->lwOutput->addItem("Найден плюс в начале");
//				qDebug() << "Найден плюс в начале";
				return false;
			}
			else{
				if(RegEx[i - 1] == '('){
					ui->lwOutput->addItem("Найден недопустимый символ перед плюсом в позиции: " + QString::number(i - 1));
//					qDebug() << "Найден недопустимый символ перед плюсом в позиции: " << i - 1;
					return false;
				}
			}
		}
	}
	if(left != right){
		ui->lwOutput->addItem("Не хватает закрывающей скобки");
//		qDebug() << "Не хватает закрывающей скобки";
		return false;
	}

	return true;
}

void MainWindow::cutResult(QStringList &list, int min, int max)
{
	qDebug() << "Checking First";
	while(!list.isEmpty() && (list.first().size() < min || list.first().size() > max)){
		list.removeFirst();
	}

	if(!list.isEmpty()){
		auto it = list.begin();
		qDebug() << "Checking Other";
		while (it + 1 != list.end()) {
			auto tmp = it + 1;
			if(tmp->size() < min || tmp->size() > max){
				list.erase(tmp);
			}
			else{
				++it;
			}
		}
	}
	qDebug() << "Checking Complete";
	list.removeDuplicates();
}

void MainWindow::on_aSave_triggered()
{
	QFile file(QFileDialog::getSaveFileName(this, "Сохранить"));

	if(file.open(QIODevice::WriteOnly)){
		file.write(QString("Term: " + oldTerm.join(", ") + '\n').toUtf8());
		file.write(QString("ToRepeat: " + oldToRepeat + '\n').toUtf8());
		file.write(QString("SubString: " + oldSubString + '\n').toUtf8());
		file.write(QString("ToMul: " + QString::number(oldToMul) + '\n').toUtf8());
		QStringList list;
		for(int i = 0; i < ui->lwOutput->count(); ++i){
			list << ui->lwOutput->item(i)->text();
		}
		file.write(QString("Result: " + list.join("\n") + '\n').toUtf8());
		file.close();
	}
}

void MainWindow::on_aTheme_triggered()
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

void MainWindow::on_aAuthor_triggered()
{
	QMessageBox::about(this, "Об Авторе",
					   "Красильников Антон\n"
					   "Студент группы ИП-512");
}
