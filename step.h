#ifndef STEP_H
#define STEP_H

#include <QObject>

class step:public QObject
{
    Q_OBJECT
public:
    step();

    int _moveid;
    int _killid;
    int _rowFrom;
    int _colFrom;
    int _rowTo;
    int _colTo;
};

#endif // STEP_H
