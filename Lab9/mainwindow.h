#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPixmap *tileSheet;
    QGraphicsPixmapItem **tiles;
    QGraphicsPixmapItem *betty;
    QTimer *timer;
    int frame, animation, x, y, speed;
    //QGraphicsPixmapItem *itemTileSheet;
private slots:
    void step();
};

#endif // MAINWINDOW_H
