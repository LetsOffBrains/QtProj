#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QMap>
#include <QFileDialog>
#include "signnode.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList forbiden;
    QString separator;

	QStringList vTerm, result;
	QString toRepeat, subString;
    int toMul;

	QString oldRegEx;

	QMap<QString, QStringList> mapSave;

	SignNode *signTree;

public slots:
	bool check_RegEx(QString RegEx);
	void cutResult(QStringList &list, int min, int max);

private slots:
    void on_leTerm_editingFinished();

    void on_cbRepeat_currentIndexChanged(const QString &arg1);

    void on_sbMul_valueChanged(int arg1);

    void on_leSubString_editingFinished();

    void on_sbFrom_valueChanged(int arg1);

	void on_pbCreateReg_clicked();

	void on_leSubString_returnPressed();

	void on_pbOutput_clicked();

	void on_aSave_triggered();

	void on_aTheme_triggered();

	void on_aAuthor_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
