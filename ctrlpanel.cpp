#include "ctrlpanel.h"
#include <QGridLayout>
#include <QVBoxLayout>

Ctrlpanel::Ctrlpanel(QWidget *parent):QWidget (parent)
{
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;

    grid->setColumnStretch(0, 1);//第0列的伸展因子为1；
    grid->setColumnStretch(2, 1);//第2列的伸展因子为1；
    grid->setRowStretch(0, 1);//第0行的伸展因子为1；
    grid->setRowStretch(2, 1);//第2行的伸展因子为1；
    grid->addLayout(vLay, 1, 1);//vlay放入第1行第1列。

    vLay->addWidget(steps);
    vLay->addWidget(this->_regret = new QPushButton("悔棋"));
    vLay->addWidget(this->_newgame = new QPushButton("开始新一局游戏"));
    connect(this->_regret, SIGNAL(clicked()), this, SIGNAL(sigback()));
}
