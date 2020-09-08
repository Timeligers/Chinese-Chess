#include "netgame.h"

netgame::netgame(bool server,QWidget* parent):board (parent)
{
    _server =nullptr;
    _socket =nullptr;
    _bServer=server;

    if(_bServer)
    {
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any, 9899);
        connect(_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    }
    else
    {
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"), 9899);
        connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));
    }
}

void netgame::slotNewConnection()
{
    if(_socket) return;

    _socket = _server->nextPendingConnection();
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slotDataArrive()));

    /* 产生随机数来决定谁走红色 */
    First = qrand()%2>0;
    new_game();

    /* 发送给对方 */
    QByteArray buf;
    buf.append(1);
    buf.append(First?0:1);
    _socket->write(buf);
}

void netgame::back()
{
//    if(_bRedTurn != _bSide)
//        return;
//    backOne();
//    backOne();

//    QByteArray buf;
//    buf.append(3);
//    _socket->write(buf);
}

void netgame::click(int id, int row, int col)
{
    if(start==false)
    {
        if(!First) return;
        else
        {
            start=true;
            _redfirst=_s[id]._red;
            _redstep=_s[id]._red;
            _takered=_s[id]._red;

            QByteArray Buf;
            Buf.append(4);
            Buf.append(_takered?0:1);
            Buf.append(_redstep);
            Buf.append(_redfirst);
            _socket->write(Buf);
        }
    }

    if(_takered!=_redstep)
        return;

    board::click(id, row, col);

    /* 发送给对方 */
    QByteArray buf;
    buf.append(2);
    buf.append(id);
    buf.append(row);
    buf.append(col);
    _socket->write(buf);
}

void netgame::backFromNetwork(QByteArray)
{
//    backOne();
//    backOne();
}
void netgame::clickFromNetwork(QByteArray buf)
{
    board::click(buf[1], buf[2], buf[3]);
}
void netgame::initFromNetwork(QByteArray buf)
{
    First=buf.at(1);
    new_game();
}

void netgame::startgame(QByteArray buf)
{
    _takered=buf.at(1);
    _redstep=buf.at(2);
    _redfirst=buf.at(3);
}
void netgame::slotDataArrive()
{
    QByteArray buf = _socket->readAll();
    switch (buf.at(0)) {
    case 1:
        initFromNetwork(buf);
        break;
    case 2:
        clickFromNetwork(buf);
        break;
    case 3:
        backFromNetwork(buf);
        break;
    case 4:
        startgame(buf);
        break;
    }
}
