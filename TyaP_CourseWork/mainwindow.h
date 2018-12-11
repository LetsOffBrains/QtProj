#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QMap>

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

	QStringList vTerm;
	QString toRepeat, subString;
    int toMul;

	QStringList oldTerm;
	QString oldToRepeat, oldSubString;
	int oldToMul;

	QMap<QString, QStringList> mapBlocks;
	QMap<QString, QStringList> mapSave;
	QMap<QString, int> mapPot;

public slots:
	void on_aTheme_activated();
	void on_aAuthor_activated();
	void on_aSave_activated();

	bool check_RegEx(QString RegEx);
	void split_RegEx(QString RegEx, QString parent);

private slots:
    void on_leTerm_editingFinished();

    void on_cbRepeat_currentIndexChanged(const QString &arg1);

    void on_sbMul_valueChanged(int arg1);

    void on_leSubString_editingFinished();

    void on_sbFrom_valueChanged(int arg1);

	void on_pbCreateReg_clicked();

	void on_leSubString_returnPressed();

	void on_pbOutput_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
