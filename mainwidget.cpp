#include "mainwidget.h"
#include <QHBoxLayout>
#include "singlegame.h"
#include "board.h"

mainwidget::mainwidget(int gametype,QWidget *parent):QWidget (parent)
{
    _gametype=gametype;
    if(_gametype==0)
    {
        game=new singlegame;
        panel=new Ctrlpanel;

        QHBoxLayout* hlay =new QHBoxLayout(this);
        hlay->addWidget(game,1);
        hlay->addWidget(panel);
        connect(panel,SIGNAL(sigback()),game,SLOT(slotback()));
        connect(panel->_newgame,SIGNAL(clicked()),game,SLOT(slotnewgame()));
        connect(panel->_newgame,SIGNAL(clicked()),this,SLOT(changelabel1()));
        connect(game,SIGNAL(mySignal()),this,SLOT(changelabel()));
        connect(game,SIGNAL(MySignal()),this,SLOT(changelabel1()));

    }
    else if(_gametype==1)
    {
        game=new board;
        panel=new Ctrlpanel;

        QHBoxLayout*hlay =new QHBoxLayout(this);
        hlay->addWidget(game,1);
        hlay->addWidget(panel);
        connect(panel,SIGNAL(sigback()),game,SLOT(slotback()));
        connect(panel->_newgame,SIGNAL(clicked()),game,SLOT(slotnewgame()));
        connect(panel->_newgame,SIGNAL(clicked()),this,SLOT(changelabel1()));
        connect(game,SIGNAL(mySignal()),this,SLOT(changelabel()));
        connect(game,SIGNAL(MySignal()),this,SLOT(changelabel1()));

    }
}
void mainwidget::changelabel()
{
    if(game->_redstep)
    {
        panel->steps->setText("红方行子");
    }
    else
    {
        panel->steps->setText("黑方行子");
    }
}

void mainwidget::changelabel1()
{
    panel->steps->setText("等待棋局开始");
}
