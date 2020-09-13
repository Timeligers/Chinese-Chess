#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "board.h"
#include "ctrlpanel.h"
#include "choosedlg.h"

class mainwidget : public  QWidget
{
    Q_OBJECT
public:
    explicit mainwidget(int gametype,QWidget *parent = 0);

    int _gametype;

    board* game;
    Ctrlpanel* panel;

public slots:
    void changelabel();
    void changelabel1();

};

#endif // MAINWIDGET_H
