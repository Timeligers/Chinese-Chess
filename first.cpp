#include "first.h"

First::First(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800,600);
    btn1 = new QPushButton(this);
    btn1->setText("棋手对弈（本地）");
    connect(btn1,&QPushButton::clicked,this,&First::toboard);
    connect(&chess_board,SIGNAL(mySignal()),this,SLOT(back()));
}

First::~First()
{
}

void First::toboard()
{
    this->setVisible(false);
    chess_board.show();
}

void First::back()
{
    this->show();
    chess_board.hide();
}
