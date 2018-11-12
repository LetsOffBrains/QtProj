#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QMessageBox>
#include <QTreeWidget>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QStringList Vn, Vt, resultTerm;
    QChar sep, nil;
    QMap<QString, QStringList> Rules;
    QMap<QString, QPair<bool, bool> > nonTermUsed;
    QMap<QString, QPair<int, int> > Potential;
    
private slots:
    void on_leTerm_editingFinished();

    void on_leNotTerm_editingFinished();

    void on_pbAddRule_clicked();

    void on_pbRemoveRule_clicked();

    void on_twRules_cellChanged(int row, int column);

    int form_rules();

    void on_pbCalculate_clicked();

    void on_cbTemplate_currentIndexChanged(int index);

    void clear_table();

    void step(QString str, QTreeWidgetItem *tree);

    int term_lenght(QString str);

    int min_potential(QString str);

    int max_potential(QString str);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
