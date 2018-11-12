#include "result.h"
#include "ui_result.h"

Result::Result(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Result)
{
	ui->setupUi(this);
}

Result::~Result()
{
	delete ui;
}

void Result::setData(QStringList n, QStringList ter, QMap<QString, QStringList> r, QString c, int f, int t)
{
	Vn = n;
	Vt = ter;
	Rules = r;
	celevoi = c;
	from = f;
	to = t;

	qDebug() << "Set Data";

	QString Term;
	foreach(QString str, Vt){
		Term.append(str);
	}
	Term.remove("@");

	QString nonTerm;
	foreach(QString str, Vn){
		nonTerm.append(str);
	}
	nonTerm.remove("@");

	Potential.clear();
	for(int i = 0; i < Vn.size(); ++i){
		QStringList tmp = Rules.find(Vn.at(i)).value();
		int min = 0, max = 0;
		if(tmp.size()){
			foreach(QString str, tmp){
				int q = 0;
				while(!str.isEmpty()){
					if(Term.contains(str.right(1))){
						++q;
					}
					str.chop(1);
				}
				if(min > q) min = q;
				if(max < q) max = q;
			}
		}
//		QPair p(min, max);
		Potential[Vn.at(i)] = qMakePair(min, max);
	}

	for(int i = 0; i < Vn.size(); ++i){
		qDebug() << Vn.at(i) << " = " << Potential[Vn.at(i)].first << ";" << Potential[Vn.at(i)].second;
	}

	ui->listWidget->clear();
	ui->treeWidget->clear();

	process();
}

void Result::process()
{

}
