#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include "stone.h"
#include "step.h"
#include <QVector>
#include <QtGlobal>
#include <QTime>
#include <QFrame>
#include <QMouseEvent>

class board : public QFrame
{
    Q_OBJECT

public:
    board(QWidget *parent = 0);
    ~board();

    stone _s[32];
    int c[32];
    int _r=20;//棋子半径
    int _selectid;
    int D;
    bool _redstep;
    bool _redfirst;
    bool start;

    QVector<step*> steps;

    void new_game();
    void srand();

    void paintEvent(QPaintEvent *);
    void drawPlate(QPainter &painter);
    void drawPlace(QPainter &painter);
    void drawStone(QPainter &painter);
    void drawStone(QPainter &painter,int id);
    QPoint center(int id);
    QPoint center(int row,int col);

    void mouseReleaseEvent(QMouseEvent * Event);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    int getStoneId(int row, int col);
    bool getClickRowCol(QPoint pt, int &row, int &col);

    bool canmove(int id,int row,int col);
    bool canmove(int i, int killid, int row, int col);
    bool canwin(int a,int b);
    int getstoneatLine(int row_f, int col_f, int row_d, int col_d);
    void moveStone(int moveid, int killid);
    void moveStone(int id, int row, int col);
    void killStone(int id);
    void reliveStone(int id);

    void savestep(int moveid, int rowto,int colto, QVector<step *> &steps);
    void savestep(int moveid, int killid, QVector<step *> &steps);

};

#endif // BOARD_H
