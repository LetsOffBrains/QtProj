#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    tileSheet = new QPixmap(":/betty.png");
    tiles = new QGraphicsPixmapItem*[4];
    for(int i = 0; i < 4; ++i){
        tiles[i] = new QGraphicsPixmapItem[4];
        for(int j = 0; j < 4; ++j){
            tiles[i][j].setPixmap(tileSheet->copy(48 * i, 48 * j, 48, 48));
            tiles[i][j].setOffset(24, 24);
        }
    }
    //betty = tiles[0][0].pixmap();
    betty = scene->addPixmap(tiles[0][0].pixmap());
    frame = 0;
    animation = 0;
    x = 0; y = 0;
    speed = 3;

    ui->graphicsView->setScene(scene);

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::step()
{
    ++frame;
    if(frame >= 4){
        //animation = (animation + 1) % 4;
        frame = 0;
    }
    betty->setPixmap(tiles[animation][frame].pixmap());
    betty->setPos(x, y);
    //scene->clear();
    //scene->addPixmap(betty);

}

void MainWindow::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    //qDebug() << key;
    if(key == Qt::Key_S){
        animation = 0;
        y += speed;
    }
    else if(key == Qt::Key_A){
        animation = 1;
        x -= speed;
    }
    else if(key == Qt::Key_W){
        animation = 2;
        y -= speed;
    }
    else if(key == Qt::Key_D){
        animation = 3;
        x += speed;
    }
}
