#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QtXml/QXmlReader>
#include <QDebug>
#include <QDate>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowTitle("Lab 7");
    ui->lAutoSave->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->progressBar_refresh->setVisible(false);

    xml = new XMLReader(ui->treeWidget);
//    sax = new saxHandler(ui->treeWidget);

    timer_interval = 1;

    saveTimer.setSingleShot(true);
    refreshTimer.setSingleShot(true);

    connect(&saveTimer, SIGNAL(timeout()), this, SLOT(save_progressBar_step()));
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(refresh_progressBar_step()));

    autoSaveTimer.start(10000);
    connect(&autoSaveTimer, SIGNAL(timeout()), this, SLOT(autosave()));
}

MainWidget::~MainWidget()
{
    if (!sFile.isEmpty()) {
        QString tmpFile = sFile + TEMP_FILE_POSTFIX;
        QFile::remove(tmpFile);
    }

    delete ui;
}

void MainWidget::on_bOpen_clicked()
{
    sFile = QFileDialog::getOpenFileName(this, QObject::tr("Open File..."), QDir::currentPath(), QObject::tr("XML files (*.xml)"));
    QFile fFile(sFile);
    QTextStream textStream(&fFile);
    if(fFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textEdit->setText(textStream.readAll());
        ui->lName->setText(sFile);
        ui->bRefresh->setEnabled(true);
    }
    else{
        QMessageBox::warning(this, "File Error", tr("Can't open %1\n%2").arg(sFile).arg(fFile.errorString()), QMessageBox::Ok);
    }
    fFile.close();
}

void MainWidget::on_bSave_clicked()
{
    if (sFile.isEmpty()) {
        sFile = QFileDialog::getSaveFileName(0, "Save File", "", "*.xml");
    }
    save();
}

void MainWidget::on_bSaveAs_clicked()
{
    sFile = QFileDialog::getSaveFileName(0, "Save File As", "", "*.*");
    save();
}

void MainWidget::on_bClear_clicked()
{
    ui->textEdit->clear();
    xml->clear();
    ui->treeWidget->clear();
}

void MainWidget::on_bExit_clicked()
{
    close();
}

void MainWidget::on_rbDom_clicked()
{
    ui->rbSAX->setChecked(false);
}

void MainWidget::on_rbSAX_clicked()
{
    ui->rbDom->setChecked(false);
}

void MainWidget::on_bRefresh_clicked()
{
    ui->progressBar_refresh->setVisible(true);
    refreshTimer.start(timer_interval);

    QFile fFile(sFile);
    if(!fFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox::warning(this, "File Error", "Can't open the file", QMessageBox::Ok);
    }
    else{
        xml->hide();
        if(ui->rbDom->isChecked()){
            xml->read(&fFile);
            xml->resize(ui->treeWidget->width(), ui->treeWidget->height());
            xml->show();
        }else if(ui->rbSAX->isChecked()){
            ui->treeWidget->clear();
            QXmlInputSource source(&fFile);
            QXmlSimpleReader reader;

            sax = new saxHandler(ui->treeWidget);
            reader.setContentHandler(sax);
            if(!reader.parse(source)){
                QMessageBox::warning(this, "SAX Error", "SAX Parse Error", QMessageBox::Ok);
            }
//            qDebug() << sax->tagNames;

        }
        else{
            QMessageBox::warning(this, "Mode Error", "Wrong Mode", QMessageBox::Ok);
        }
    }
    fFile.close();
}

void MainWidget::save_progressBar_step()
{
    int value = ui->progressBar->value();
    if( value < ui->progressBar->maximum()){
        ui->progressBar->setValue(value + 1);
        saveTimer.start(timer_interval);
    }
    else{
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
        ui->lAutoSave->setVisible(false);
    }
}

void MainWidget::refresh_progressBar_step()
{
    int value = ui->progressBar_refresh->value();
    if( value < ui->progressBar_refresh->maximum()){
        ui->progressBar_refresh->setValue(value + 1);
        refreshTimer.start(timer_interval);
    }
    else{
        ui->progressBar_refresh->setValue(0);
        ui->progressBar_refresh->setVisible(false);
        xml->resize(ui->treeWidget->width(), ui->treeWidget->height());
    }
}

void MainWidget::on_textEdit_textChanged()
{
    ui->bRefresh->setEnabled(false);
}

void MainWidget::autosave()
{
    QString tempFile;
    if (!sFile.isEmpty()){
        tempFile = sFile + TEMP_FILE_POSTFIX;
    }
    else {
        tempFile = TEMP_PATH + QDate::currentDate().toString().replace(" ", "");
    }

    QFile file(tempFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        saveTimer.start(timer_interval);
        ui->progressBar->setVisible(true);
        ui->lAutoSave->setVisible(true);

        QTextStream out(&file);
        //out.setCodec("UTF-8");
        QString text = ui->textEdit->toPlainText();

        out << text;

        file.close();

        ui->bRefresh->setEnabled(true);
    }
}

void MainWidget::save()
{
    QFile file(sFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->lName->setText(sFile);
        QTextStream out(&file);
        //out.setCodec("UTF-8");
        out << ui->textEdit->toPlainText();
        file.close();

        ui->bRefresh->setEnabled(true);
    }
}

