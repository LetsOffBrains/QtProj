#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    sep = ' ';
    nil = '@';
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_leTerm_editingFinished()
{
    Vt = ui->leTerm->text().split(sep, QString::SkipEmptyParts);

    if(Vt.length()){
        QString right;
        for (int i = 0; i < Vt.size(); ++i){
            right.append(Vt.at(i));
            right.append(sep);
        }
        if(right.contains(nil)){
            right.chop(1);
        }
        else{
            right.append(nil);
            Vt << QString(nil);
        }
        ui->leTerm->setText(right);
    }
}

void Widget::on_leNotTerm_editingFinished()
{
    Vn = ui->leNotTerm->text().split(sep, QString::SkipEmptyParts);
    Vn.removeDuplicates();
    ui->cbCelevoi->clear();
    nonTermUsed.clear();
    if(Vn.length()){
        ui->cbCelevoi->setEnabled(true);

        QString right;

        for (int i = 0; i < Vn.size(); ++i){
            ui->cbCelevoi->addItem(Vn.at(i));

//            nonTermUsed[Vn.at(i)] = false;
            right.append(Vn.at(i));
            right.append(sep);
        }
        right.chop(1);
        ui->leNotTerm->setText(right);
    }
    else{
        ui->cbCelevoi->setEnabled(false);
    }
}

void Widget::on_pbAddRule_clicked()
{
    int row = ui->twRules->rowCount();
    ui->twRules->insertRow(row);
    ui->twRules->setItem(row, 0, new QTableWidgetItem(QString()));
    ui->twRules->setItem(row, 1, new QTableWidgetItem(QString()));
}

void Widget::on_pbRemoveRule_clicked()
{
    ui->twRules->removeRow(ui->twRules->currentRow());
}

void Widget::on_twRules_cellChanged(int row, int column)
{
    QString txt = ui->twRules->item(row, column)->text();
    if(column == 0){// Ð  Ð¡ÑšÐ  Ð’Âµ Ð  Ð¡Ñ›Ð  Ð’ÂµÐ ÐŽÐ â€šÐ  Ð¡Â˜Ð  Ð¡â€˜Ð  Ð â€¦Ð  Ð’Â°Ð  Ð’Â»
        bool flag = true;
        for (int i = 0; i < Vn.size(); ++i){
            if(Vn.at(i) == txt){
                flag = false;
                break;
            }
        }

        if(flag){
            ui->twRules->item(row, column)->setText(QString());
        }
    }
    else{// Ð  Ð¡ÑŸÐ ÐŽÐ â€šÐ  Ð’Â°Ð  Ð â€ Ð  Ð¡â€˜Ð  Ð’Â»Ð  Ð¡â€¢
        QStringList list = ui->twRules->item(row, column)->text().split(sep, QString::SkipEmptyParts);
        list.removeDuplicates();

        QString right;
        for (int i = 0; i < list.size(); ++i){
            QString tmp = list.at(i);

            for(int j = 0; j < Vt.size(); ++j){
                tmp.remove(Vt.at(j));
            }
            for(int j = 0; j < Vn.size(); ++j){
                tmp.remove(Vn.at(j));
            }

            if(tmp.isEmpty()){
                right.append(list.at(i));
                right.append(sep);
            }
        }
        right.chop(1);
        ui->twRules->item(row, column)->setText(right);
    }
}

int Widget::form_rules()
{
//    for(QMap<QString, bool>::iterator it = nonTermUsed.begin(); it != nonTermUsed.end(); it++){
//        nonTermUsed[it.key()] = false;
//    }
    Rules.clear();
    int row = 0;
    while(row < ui->twRules->rowCount()){
        if(ui->twRules->item(row, 0)->text().isEmpty() || ui->twRules->item(row, 1)->text().isEmpty()){
            ui->twRules->removeRow(row);
        }
        else{
            bool flag = true;
            for(int i = 0; i < row; ++i){
                if(ui->twRules->item(row, 0)->text() == ui->twRules->item(i, 0)->text()){
                    ui->twRules->removeRow(row);
                    flag = false;
                    break;
                }
            }
            if(flag){
//                nonTermUsed[ui->twRules->item(row, 0)->text()] = true;
                QString nt = ui->twRules->item(row, 0)->text();
                Rules[nt] = ui->twRules->item(row, 1)->text().split(sep, QString::SkipEmptyParts);
                int min = 999, max = 0;
                for(int i = 0; i < Rules[nt].size(); ++i){
                    int pot = term_lenght(Rules[nt][i]); //Rules[nt][i].size();
                    pot += Rules[nt][i].count(nt);
                    if(pot < min) min = pot;
                    if(pot > max) max = pot;
                }
                Potential[nt] = qMakePair(min, max);
                ++row;
            }
        }
    }
    return Rules.size();
}

void Widget::on_cbTemplate_currentIndexChanged(int index)
{
    resultTerm.clear();
    ui->treeWidget->clear();
    ui->listWidget->clear();
    switch(index){
    case 0:{ // Ð  Ð¡ÑŸÐ ÐŽÐ¡â€œÐ ÐŽÐ Ñ“Ð ÐŽÐ²Ð‚Ñ™Ð  Ð¡â€¢
        ui->leTerm->clear(); on_leTerm_editingFinished();
        ui->leNotTerm->clear(); on_leNotTerm_editingFinished();
        ui->cbCelevoi->clear();
        ui->cbCelevoi->setEnabled(false);
        ui->sbFrom->setValue(0);
        ui->sbTo->setValue(1);

        clear_table();
        break;
    }
    case 1:{ // S->S(S)S|@
        ui->leTerm->setText("( ) " + QString(nil)); on_leTerm_editingFinished();
        ui->leNotTerm->setText("S"); on_leNotTerm_editingFinished();
        ui->cbCelevoi->setEnabled(true);
        ui->sbFrom->setValue(0);
        ui->sbTo->setValue(6);

        clear_table();
        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("S"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("S(S)S " + QString(nil)));
        break;
    }
    case 2:{ // S->AAA    A->AAA|0
        ui->leTerm->setText("0"); on_leTerm_editingFinished();
        ui->leNotTerm->setText("S A"); on_leNotTerm_editingFinished();
        ui->cbCelevoi->setEnabled(true);
        ui->sbFrom->setValue(0);
        ui->sbTo->setValue(10);

        clear_table();
        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("S"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("AAA"));

        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("A"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("AAA 0"));
        break;
    }
    case 3:{ // S->aaS|0A1    A->0A1|@
        ui->leTerm->setText("a 0 1 " + QString(nil)); on_leTerm_editingFinished();
        ui->leNotTerm->setText("S A"); on_leNotTerm_editingFinished();
        ui->cbCelevoi->setEnabled(true);
        ui->sbFrom->setValue(0);
        ui->sbTo->setValue(6);

        clear_table();
        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("S"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("aaS 0A1"));

        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("A"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("0A1 " + QString(nil)));
        break;
    }
    case 4:{ // S->A0BA  A->00A|@  B->aBb|@
        ui->leTerm->setText("0 a b " + QString(nil)); on_leTerm_editingFinished();
        ui->leNotTerm->setText("S A B"); on_leNotTerm_editingFinished();
        ui->cbCelevoi->setEnabled(true);
        ui->sbFrom->setValue(0);
        ui->sbTo->setValue(10);

        clear_table();
        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("S"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("A0BA"));

        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("A"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("00A " + QString(nil)));

        ui->twRules->insertRow(ui->twRules->rowCount());
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 0, new QTableWidgetItem("B"));
        ui->twRules->setItem(ui->twRules->rowCount() - 1, 1, new QTableWidgetItem("aBb " + QString(nil)));
        break;
    }
    }
}

void Widget::clear_table()
{
    while(ui->twRules->rowCount())
        ui->twRules->removeRow(0);
}

void Widget::on_pbCalculate_clicked()
{
	qDebug() << "Calculate";
    if(!form_rules()){
        QMessageBox msg;
        msg.setText(QString::fromUtf8("ÐÐµÑ‚ Ð¿Ñ€Ð°Ð²Ð¸Ð»"));
        msg.exec();
    }
    else{
        resultTerm.clear();
        ui->treeWidget->clear();
        ui->listWidget->clear();
        ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(QStringList(ui->cbCelevoi->currentText())));
        step(ui->cbCelevoi->currentText(), ui->treeWidget->topLevelItem(0));
        qDebug() << "Steps Complete!";
        resultTerm.removeDuplicates();
        ui->listWidget->addItems(resultTerm);
        ui->listWidget->sortItems();
        ui->treeWidget->expandAll();
    }
}

void Widget::step(QString str, QTreeWidgetItem *tree)
{
    for(int i = 0; i < str.length(); ++i){
        if(ui->leNotTerm->text().contains(str[i])){
            for(int j = 0; j < Rules[str.at(i)].size(); ++j){ // progonyaem pravila
                QString tmp = str;
                tmp.remove(i, 1);
                if(Rules[str.at(i)][j] != QString(nil)){
                    tmp.insert(i, Rules[str.at(i)][j]);
                }
                if(min_potential(tmp) <= ui->sbTo->value() &&
//                        max_potential(tmp) <= ui->sbTo->value()
                        term_lenght(tmp) <= ui->sbTo->value()
                        ){
                    QTreeWidgetItem *it = new QTreeWidgetItem(QStringList(tmp));
                    tree->addChild(it);
                    if(tmp.length() == term_lenght(tmp)){
                        it->setTextColor(0, QColor(0, 150, 0));
//                        it->setBackgroundColor(0, QColor(150, 0, 0));
                    }
                    step(tmp, it);
                }
            }
            return;
        }
    }
    if(str.length() >= ui->sbFrom->value() && str.length() <= ui->sbTo->value()){
        resultTerm << QString(QString::number(str.length()) + "\t" + str);
    }
}

int Widget::term_lenght(QString str)
{
    int size = 0;
    for(int i = 0; i < str.size(); ++i){
        QString ch = str.at(i);
        if(ui->leTerm->text().contains(ch) && ch != nil){
            ++size;
        }
    }
    return size;
}

int Widget::min_potential(QString str)
{
    int n = 0;
    for(int i = 0; i < str.size(); ++i){
        QString ch = str.at(i);
        if(ui->leNotTerm->text().contains(ch)){
            n += Potential[ch].first;
        }
        else{
            ++n;
        }
    }
    return n;
}

int Widget::max_potential(QString str)
{
    int n = 0;
    for(int i = 0; i < str.size(); ++i){
        QString ch = str.at(i);
        if(ui->leNotTerm->text().contains(ch)){
            n += Potential[ch].second;
        }
        else{
            ++n;
        }
    }
    return n;
}
