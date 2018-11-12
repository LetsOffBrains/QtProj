#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTableWidgetItem>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

	QStringList Simbols, States, FinishStates;
	QString StartState, Sep, Nil;
	QMap<QString, QMap<QString, QString> > Rules; // Found State => Simbol => Result State
    
private slots:
	void on_leTerm_editingFinished();

	void on_leEmpty_editingFinished();

	void on_leStates_editingFinished();

	void on_cbStart_currentIndexChanged(const QString &arg1);

	void on_leFinish_editingFinished();

	void build_rules_table();

	void on_tableWidget_itemChanged(QTableWidgetItem *item);

	void enable_input();

	bool check_input_to_proceed();

	void on_leInput_editingFinished();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
