#ifndef NETGAME_H
#define NETGAME_H

#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

class netgame:public board
{
    Q_OBJECT
public:
    netgame(bool server,QWidget* parent =nullptr);

    bool _bServer;
    bool First;
    bool _takered;
    QTcpServer* _server;
    QTcpSocket* _socket;

    void back();
    void click(int id, int row, int col);

    void backFromNetwork(QByteArray buf);
    void clickFromNetwork(QByteArray buf);
    void initFromNetwork(QByteArray buf);
    void startgame(QByteArray buf);

public slots:
    void slotNewConnection();
    void slotDataArrive();

};

#endif // NETGAME_H
