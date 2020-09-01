#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QtGlobal>
#include <QTime>
#include <QMouseEvent>
#include "stone.h"

class board : public QWidget
{
    Q_OBJECT
public:
    explicit board(QWidget *parent = nullptr);

    stone _s[32];//棋子
    int _r;//半径。
    int D=0;//死亡计数器。
    int _selectid;//选中的棋子ID。
    int c[32];//随机化数组安排棋子位置。
    bool _redstep;//红黑子轮流走棋
    bool start=false;

    void paintEvent(QPaintEvent *);
    void drawstone(QPainter &painter,int id);
    void mouseReleaseEvent(QMouseEvent*);//选取棋子信号
    bool getRowcol(QPoint pt,int &row,int &col);//判断是否点到棋子以及得到棋子行列号
    bool CanWin(int a,int b);
    bool canmove(int id,int row,int col);
    void srand();//随机化
    int getstoneatLine(int row_f,int col_f,int row_d,int col_d);
    QPoint center(int row,int col);
    QPoint center(int id);

signals:

public slots:
};

#endif // BOARD_H
