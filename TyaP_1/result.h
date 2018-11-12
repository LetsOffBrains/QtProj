#ifndef RESULT_H
#define RESULT_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class Result;
}

class Result : public QWidget
{
	Q_OBJECT
	
public:
	explicit Result(QWidget *parent = 0);
	~Result();

	QStringList Vn, Vt;
	QString celevoi;
	QMap<QString, QStringList> Rules;
	QMap<QString, QPair<int, int> > Potential;

	int from, to;

	void setData(QStringList n, QStringList ter, QMap<QString, QStringList> r, QString c, int f, int t);

	void process();
	
private:
	Ui::Result *ui;
};

#endif // RESULT_H
