#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QStringList>
#include <QMap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

	bool modified;
	QString start, nil, sep;
	QStringList term, trans, mag, states, finish;
	QMap<QString, QMap<QString, QMap<QString, QStringList> > > rules; // <State, <Input, <Mag, QString List RightSide> > >
    
private slots:
	void on_leTerm_editingFinished();

	void on_leMag_editingFinished();

	void on_leStates_editingFinished();

	void on_leFinish_editingFinished();

	void on_cbStart_currentIndexChanged(const QString &arg1);

	void on_leStartMag_editingFinished();

	void on_pbAdd_clicked();

	void on_pbDelete_clicked();

	void on_twRules_cellChanged(int row, int column);

	void check_all_rules();

	void on_leInput_editingFinished();

	int create_rules();

	void on_leTransTerm_editingFinished();

	void on_leNil_editingFinished();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
