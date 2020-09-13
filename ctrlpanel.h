#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Ctrlpanel: public QWidget
{
    Q_OBJECT
public:
    Ctrlpanel(QWidget *parent = 0);

    QPushButton* _regret;
    QPushButton* _newgame;
    QLabel* steps = new QLabel("等待棋局开始");

signals:
    void sigback();
};

#endif // CTRLPANEL_H
