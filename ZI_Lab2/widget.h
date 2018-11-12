#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include "shamir.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT
	
public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

	Shamir shamir;
	
private slots:
	void on_pbOpen_clicked();

	void on_pbProcess_clicked();

private:
	Ui::Widget *ui;
};

#endif // WIDGET_H
