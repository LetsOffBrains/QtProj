#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
    int m = 123456;
    shamir.test(m);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_pbOpen_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"));
	QFile file(fileName);
	if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
		ui->teFrom->setText(file.readAll());
		file.close();
	}
	else{
		ui->teFrom->setText(QString::fromUtf8("Ошибка чтения файла"));
	}
}

void Widget::on_pbProcess_clicked()
{
	switch(ui->cbMethod->currentIndex()){
	case 0:{ // Shamir Encode
        int p = 100500;
        QString encode = shamir.encode(ui->teFrom->toPlainText(), p);
        ui->teEncode->setText(encode);
        ui->teDecode->setText(shamir.decode(encode, p));
	}break;

    case 1:{ // El-Gaml - encode

	}break;

    case 2:{ // Vernam encode

	}break;

    case 3:{ // RSA encode

	}break;
	}
}


