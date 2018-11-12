#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "xmlreader.h"
#include "saxhandler.h"

namespace Ui {
class MainWidget;
}

static const QString TEMP_FILE_POSTFIX = "_temp";
static const QString TEMP_PATH = "/tmp/";

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
private slots:
    void on_bOpen_clicked();

    void on_bClear_clicked();

    void on_bSave_clicked();

    void on_bSaveAs_clicked();

    void on_bExit_clicked();

    void on_rbDom_clicked();

    void on_rbSAX_clicked();

    void on_bRefresh_clicked();

    void save_progressBar_step();
    void refresh_progressBar_step();

    void on_textEdit_textChanged();

    void autosave();
    void save();

private:
    Ui::MainWidget *ui;
    QString sFile;
    XMLReader *xml;
    saxHandler *sax;
    QTimer saveTimer, refreshTimer, autoSaveTimer;
    int timer_interval;

};

#endif // MAINWIDGET_H
