#ifndef FIRST_H
#define FIRST_H

#include <QWidget>
#include <QPushButton>
#include "board.h"

class First : public QWidget
{
    Q_OBJECT
public:
    explicit First(QWidget *parent = nullptr);
    ~First();

    board chess_board;
    QPushButton *btn1;

signals:

public slots:

private slots:
    void toboard();
    void back();
};

#endif // FIRST_H
