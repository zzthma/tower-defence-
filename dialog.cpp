#include "dialog.h"
#include "ui_dialog.h"
#include "basictower.h"
#include "route.h"
#include "enemy1.h"
#include "attack.h"
#include "listreader.h"
#include "towerposition.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>

static const int Tower1Cost = 200;
static const int Tower2Cost = 500;

Dialog::Dialog(int a,int b,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
    , _waves(a)
    , _playerHp(b)
    , _playrGold(c)
    , _gameEnded(false)
    , _gameWin(false)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *)
{
}


void Dialog::loadtowerpositions()
{
}

void Dialog::mousePressEvent(QMouseEvent *)
{
}

bool Dialog::canBuyTower1() const
{
    if (_playrGold >=Tower1Cost)
        return true;
    return false;
}

bool Dialog::canBuyTower2() const
{
    if (_playrGold >=Tower2Cost)
        return true;
    return false;
}
void Dialog::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(_waves + 1));
}

void Dialog::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(_playerHp));
}

void Dialog::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(_playrGold));
}

void Dialog::doGameOver()
{
    if (!_gameEnded)
    {
        _gameEnded = true;

    }
    if(!_towerPositionsList.empty())
    {
        for(int i=0;i<_towerPositionsList.size();i++)
            {
                 _towerPositionsList.removeAt(i);
            }
    }
   if(!_towersList.empty()){

      foreach(basictower *tower, _towersList)
            {
                     _towersList.removeOne(tower);
                     delete tower;
                 }
    }
   if(!_enemyList.empty()){

    foreach(enemy1 *enemy, _enemyList)
         {
                         _enemyList.removeOne(enemy);
                         delete enemy;
                     }
        }
   if(!_bulletList.empty()){

    foreach( attack *Attack, _bulletList)
         {
               _bulletList.removeOne(Attack);
                 delete Attack;
                     }
        }
}

void Dialog::awardgold(int gold)
{
    _playrGold += gold;
    update();
}


void Dialog::addroute()
{
}

void Dialog::getHPdamage(int damage)
{
    _playerHp -= damage;
    if (_playerHp <= 0)
        doGameOver();
}

void Dialog::removedenemy(enemy1 *enemy)
{
    Q_ASSERT(enemy);

    _enemyList.removeOne(enemy);
    delete enemy;

    if (_enemyList.empty())
    {
        ++_waves;
        if (!loadWave())
        {
            _gameWin = true;
        }
    }
}

void Dialog::removedbullet(attack *Attack)
{
    Q_ASSERT(Attack);
    _bulletList.removeOne(Attack);
    delete Attack;
}

void Dialog::addbullet(attack *Attack)
{
    Q_ASSERT(Attack);
    _bulletList.push_back(Attack);
}

void Dialog::updateMap()
{
    foreach (enemy1 *enemy, _enemyList)
        enemy->move();
    foreach (basictower * tower, _towersList)
        tower->checkEnemyInRange();
    update();
}

void Dialog::preLoadWavesInfo()
{
}

bool Dialog::loadWave()
{
    if (_waves >= _wavesInfo.size())
        return false;

    route *startWayPoint = _wayPointsList.back();
    QList<QVariant> curWavesInfo = _wavesInfo[_waves].toList();

    for (int i = 0; i < curWavesInfo.size();++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        enemy1 *Enemy1 = new enemy1(startWayPoint, this,QPixmap(":/enemy1.png"),1.0,50);
        enemy1 *Enemy2 = new enemy1(startWayPoint, this,QPixmap(":/enemy2.png"),2.0,30);
        _enemyList.push_back(Enemy1);
        _enemyList.push_back(Enemy2);
        QTimer::singleShot(spawnTime, Enemy1, SLOT(doActivate()));
        QTimer::singleShot(spawnTime, Enemy2, SLOT(doActivate()));
    }

    return true;
}

QList<enemy1 *> Dialog::enemyList() const
{
    return _enemyList;
}

void Dialog::gameStart()
{
    loadWave();
}

first::first(QWidget *parent) :
    Dialog(0,5,1200,parent),
    ui(new Ui::Dialog),
    player(new QMediaPlayer)
{
    ui->setupUi(this);

    preLoadWavesInfo();
    loadtowerpositions();
    addroute();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    QTimer::singleShot(300, this, SLOT(gameStart()));
}
first::~first(){
    delete ui;
    delete player;
}

void first::paintEvent(QPaintEvent *)
{
    if (_gameEnded || _gameWin)
    {
        QString text = _gameEnded ? "YOU LOST!!!" : "YOU WIN!!!(click to start next level)";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    QPixmap cachePix(":/map1.jpg");
    QPainter cachePainter(&cachePix);

    foreach (const towerposition &towerPos, _towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const basictower *tower, _towersList)
        tower->draw(&cachePainter);

    foreach (const enemy1 *enemy, _enemyList)
        enemy->draw(&cachePainter);

    foreach (const attack *attack, _bulletList)
        attack->draw(&cachePainter);

    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);

    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}

void first::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
      {
        if(_gameWin){
            class nextlevel *a=new nextlevel;
            this->close();
            delete player;
            a->show();
            a->musiscplay();
        }
        else{
            QPoint pressPos = event->pos();
            auto it = _towerPositionsList.begin();
            while (it != _towerPositionsList.end())
            {
                if ( it->containPoint(pressPos) && !it->hasTower()&&canBuyTower1())
                {
                    tower1 *Tower1 = new tower1(it->centerPos(), this,QPixmap(":/basic.png"));
                        it->setHasTower(true);
                        _towersList.push_back(Tower1);
                        _playrGold -= Tower1Cost;
                        update();
                        break;
                }
                ++it;
            }
        }

       }

       else if(event->button() == Qt::RightButton)
      {
        QPoint pressPos = event->pos();
        auto it = _towerPositionsList.begin();
        while (it != _towerPositionsList.end())
        {
            if (it->containPoint(pressPos) && !it->hasTower()&&canBuyTower2())
            {
                tower2 *Tower2 = new tower2(it->centerPos(), this,QPixmap(":/basic2.png"));
                    it->setHasTower(true);
                    _towersList.push_back(Tower2);
                    _playrGold -=Tower2Cost;
                    update();
                    break;
            }

            ++it;
        }
       }

}
void first::musiscplay()
{
    player->setMedia(QUrl("qrc:/sounds/first.mp3"));
    player->setVolume(100);
    player->play();
}

void first::addroute()
{
    route *wayPoint1 = new route(QPoint(750, 500));
    _wayPointsList.push_back(wayPoint1);

    route *wayPoint2 = new route(QPoint(70, 500));
    _wayPointsList.push_back(wayPoint2);
    wayPoint2->setnextroute(wayPoint1);

    route *wayPoint3 = new route(QPoint(70, 340));
    _wayPointsList.push_back(wayPoint3);
    wayPoint3->setnextroute(wayPoint2);

    route *wayPoint4 = new route(QPoint(830, 340));
    _wayPointsList.push_back(wayPoint4);
    wayPoint4->setnextroute(wayPoint3);

    route *wayPoint5 = new route(QPoint(830, 180));
    _wayPointsList.push_back(wayPoint5);
    wayPoint5->setnextroute(wayPoint4);

    route *wayPoint6 = new route(QPoint(70, 180));
    _wayPointsList.push_back(wayPoint6);
    wayPoint6->setnextroute(wayPoint5);
}
void first::loadtowerpositions()
{
    QFile file(":/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    listreader reader;
    reader.read(&file);

    QList<QVariant> array = reader.data();
    foreach (QVariant dict, array)
    {
        QMap<QString, QVariant> point = dict.toMap();
        int x = point.value("x").toInt();
        int y = point.value("y").toInt();
        _towerPositionsList.push_back(QPoint(x, y));
    }

    file.close();
}

void first::preLoadWavesInfo()
{
    QFile file(":/Waves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    listreader reader;
    reader.read(&file);


    _wavesInfo = reader.data();

    file.close();
}

nextlevel::nextlevel(QWidget *parent) :
    Dialog(0,8,2000,parent),
    ui(new Ui::Dialog),
    player(new QMediaPlayer)
{
    ui->setupUi(this);
    preLoadWavesInfo();
    loadtowerpositions();
    addroute();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    QTimer::singleShot(300, this, SLOT(gameStart()));
}
nextlevel::~nextlevel(){
    delete ui;
    delete player;
}
void nextlevel::loadtowerpositions()
{
    QPoint pos[10]{
        QPoint(200,360),
        QPoint(200,200),
        QPoint(360,200),
        QPoint(360,360),
        QPoint(520,140),
        QPoint(520,280),
        QPoint(520,400),
        QPoint(680,180),
        QPoint(680,300),
        QPoint(680,410),
    };
    int i;
    for(i=0;i<10;i++)
        {
        _towerPositionsList.push_back(pos[i]);
    }
}
void nextlevel::paintEvent(QPaintEvent *)
{
    if (_gameEnded || _gameWin)
    {
        QString text = _gameEnded ? "YOU LOST!!!" : "YOU WIN!!!(click to back mainwindow)";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    QPixmap cachePix(":/map2.jpg");
    QPainter cachePainter(&cachePix);

    foreach (const towerposition &towerPos, _towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const basictower *tower, _towersList)
        tower->draw(&cachePainter);

    foreach (const enemy1 *enemy, _enemyList)
        enemy->draw(&cachePainter);

    foreach (const attack *attack, _bulletList)
        attack->draw(&cachePainter);

    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);

    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}


void nextlevel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
      {
            if(_gameWin){
                class MainWindow *a=new MainWindow;
                this->close();
                delete player;
                a->show();
                a->musiscplay();
            }
            else{
            QPoint pressPos = event->pos();
            auto it = _towerPositionsList.begin();
            while (it != _towerPositionsList.end())
            {
                if ( it->containPoint(pressPos) && !it->hasTower()&&canBuyTower1())
                {
                    tower1 *Tower1 = new tower1(it->centerPos(), this,  QPixmap(":/basic.png"));
                        it->setHasTower(true);
                        _towersList.push_back(Tower1);
                        _playrGold -=Tower1Cost;
                        update();
                        break;
                }
                if(it->containPoint(pressPos) &&it->hasTower()&&_playrGold>=200&&!it->towergrade())
                {
                    it->telltowergrade(true);
                    _playrGold -=200;
                    for (int i=0;i<_towersList.count();i++) {
                               if(it->containPoint(pressPos) &&it->containPoint(_towersList.at(i)->_pos))
                               {
                                   _towersList.removeAt(i);
                                       break;
                               }}
                    if(!it->towerkind()){
                    tower1 *Tower1 = new tower1(it->centerPos(), this,  QPixmap(":/tower1.png"));
                    Tower1->upgrade();
                    _towersList.push_back(Tower1);
                    }
                    else{
                        tower2 *Tower2 = new tower2(it->centerPos(), this,  QPixmap(":/tower2.png"));
                        Tower2->upgrade();
                        _towersList.push_back(Tower2);
                      }
                    break;
                }
                ++it;
            }
        }
}
       else if(event->button() == Qt::RightButton)
      {
        QPoint pressPos = event->pos();
        auto it = _towerPositionsList.begin();
        while (it != _towerPositionsList.end())
        {
            if (it->containPoint(pressPos) && !it->hasTower()&&canBuyTower2())
            {
                tower2 *Tower2 = new tower2(it->centerPos(), this,QPixmap(":/basic2.png"));
                    it->setHasTower(true);
                    it->telltowerkind(true);
                    _towersList.push_back(Tower2);
                    _playrGold -=Tower2Cost;
                    update();
                    break;
            }
                   if(it->containPoint(pressPos) && it->hasTower()){
                    it->setHasTower(false);
                    it->telltowerkind(false);
                    it->telltowergrade(false);
                    for (int i=0;i<_towersList.count();i++) {
                               if(it->containPoint(pressPos) &&it->containPoint(_towersList.at(i)->_pos))
                               {
                                   _towersList.removeAt(i);
                                       break;
                               }
                           }
                    update();
                    break;
                }
            ++it;
            }
        }
       }

void nextlevel::musiscplay()
{
    player->setMedia(QUrl("qrc:/sounds/next.mp3"));
    player->setVolume(100);
    player->play();
}

void nextlevel::addroute()
{
    route *wayPoint1 = new route(QPoint(745, 480));
    _wayPointsList.push_back(wayPoint1);

    route *wayPoint2 = new route(QPoint(745, 100));
    _wayPointsList.push_back(wayPoint2);
    wayPoint2->setnextroute(wayPoint1);

    route *wayPoint3 = new route(QPoint(600, 100));
    _wayPointsList.push_back(wayPoint3);
    wayPoint3->setnextroute(wayPoint2);

    route *wayPoint4 = new route(QPoint(600, 470));
    wayPoint4->setnextroute(wayPoint3);

    route *wayPoint5 = new route(QPoint(435, 470));
    _wayPointsList.push_back(wayPoint5);
    wayPoint5->setnextroute(wayPoint4);

    route *wayPoint6 = new route(QPoint(435, 120));
    _wayPointsList.push_back(wayPoint6);
    wayPoint6->setnextroute(wayPoint5);

    route *wayPoint7 = new route(QPoint(280, 120));
    _wayPointsList.push_back(wayPoint7);
    wayPoint7->setnextroute(wayPoint6);

    route *wayPoint8 = new route(QPoint(280, 440));
    _wayPointsList.push_back(wayPoint8);
    wayPoint8->setnextroute(wayPoint7);

    route *wayPoint9 = new route(QPoint(125, 440));
    _wayPointsList.push_back(wayPoint9);
    wayPoint9->setnextroute(wayPoint8);

    route *wayPoint10 = new route(QPoint(125, 50));
    _wayPointsList.push_back(wayPoint10);
    wayPoint10->setnextroute(wayPoint9);
}

void nextlevel::preLoadWavesInfo()
{
    QFile file(":/Waves1.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }
    listreader reader;
    reader.read(&file);
    _wavesInfo = reader.data();
    file.close();
}

