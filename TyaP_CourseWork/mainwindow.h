#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>

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

	QStringList vTerm, blockRes;
	QString toRepeat, subString, blockRegEx;
    int toMul;
public slots:
	void on_aTheme_activated();
	void on_aAuthor_activated();
	void on_aSave_activated();

private slots:
    void on_leTerm_editingFinished();

    void on_cbRepeat_currentIndexChanged(const QString &arg1);

    void on_sbMul_valueChanged(int arg1);

    void on_leSubString_editingFinished();

    void on_sbFrom_valueChanged(int arg1);

	void on_pbCreateReg_clicked();

	void on_leSubString_returnPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
