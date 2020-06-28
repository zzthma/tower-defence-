#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QList>
#include "basictower.h"
#include <QMediaPlayer>

class towerposition;
class route;
class enemy1;
class attack;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int a, int b, int c,QWidget *parent = 0);
    ~Dialog();

    void getHPdamage(int damage = 1);
    void removedenemy(enemy1 *enemy);
    void removedbullet(attack *attack);
    void addbullet(attack *attack);
    void awardgold(int gold);

    QList<enemy1 *> enemyList() const;

protected:
 virtual   void paintEvent(QPaintEvent *);
 virtual   void mousePressEvent(QMouseEvent *);

public slots:
    void updateMap();
    void gameStart();

protected:
  virtual  void loadtowerpositions();
   virtual void addroute();
    bool loadWave();
    bool canBuyTower1() const;
    bool canBuyTower2() const;
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();
  virtual  void preLoadWavesInfo();


protected:
    Ui::Dialog *		ui;
    int						_waves;
    int						_playerHp;
    int						_playrGold;
    bool					_gameEnded;
    bool					_gameWin;
    QList<QVariant>			_wavesInfo;
    QList<towerposition>	_towerPositionsList;
    QList<basictower *>			_towersList;
    QList<route *>		_wayPointsList;
    QList<enemy1 *>			_enemyList;
    QList<attack *>			_bulletList;
};

class first:public Dialog
{
    Q_OBJECT
public:
    explicit first(QWidget *parent = 0);
    ~first();
    void musiscplay();
protected:
    void loadtowerpositions();
    void addroute();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void preLoadWavesInfo();

private:
    Ui::Dialog *		ui;
    QMediaPlayer *player;

};

class nextlevel:public Dialog
{
    Q_OBJECT
public:
    explicit nextlevel(QWidget *parent = 0);
    ~nextlevel();
   void musiscplay();
protected:
    void loadtowerpositions();
    void addroute();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void preLoadWavesInfo();

private:
    Ui::Dialog *		ui;
    QMediaPlayer *player;
};


#endif // DIALOG_H
