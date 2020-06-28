#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dialog.h>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,player(new QMediaPlayer)
{
    ui->setupUi(this);
    /*player->setMedia(QUrl("qrc:/sounds/1.mp3"));
    player->setVolume(100);
    player->play();*/
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
}


void MainWindow::on_pushButton_clicked()
{
    class first *a=new first;
    this->close();
    delete player;
    a->show();
    a->musiscplay();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPixmap cachePix(":/1.jpg");
    QPainter cachePainter(&cachePix);
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void MainWindow::musiscplay()
{
    player->setMedia(QUrl("qrc:/sounds/1.mp3"));
    player->setVolume(100);
    player->play();
}

void MainWindow::on_pushButton_2_clicked()
{
    class nextlevel *a=new nextlevel;
    this->close();
    delete player;
    a->show();
    a->musiscplay();
}
