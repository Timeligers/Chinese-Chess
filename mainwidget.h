#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "board.h"

class mainwidget : public  QWidget
{
    Q_OBJECT
public:
    mainwidget();

    int _gametype;
};

#endif // MAINWIDGET_H
