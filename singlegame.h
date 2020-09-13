#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include <QDebug>
#include <QTimer>
#include "board.h"

class singlegame:public board
{
    Q_OBJECT
public:
    singlegame(QWidget *parent=0);

    void click(int id,int row,int col);


    step* getBestMove();
    void getAllpossiblemoveMain(QVector<step*>& steps);
    void getAllpossiblemove(bool redfirst,QVector<step*>&steps);
    void fakemove(step*step);
    void unfakemove(step*step);
    int getminscore(int level,int curmaxscore);
    int getmaxscore(int level,int curminscore);
    int score();

    void back();

    int level;


signals:

public slots:
    void computerMove();
};

#endif // SINGLEGAME_H
