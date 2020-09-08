#include "board.h"
#include "singlegame.h"
#include "netgame.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMessageBox::StandardButton ret;
    ret=QMessageBox::question(nullptr,"server or client","作为服务器启动");

    bool bserver = false;
    if(ret==QMessageBox::Yes)
    {
        bserver = true;
    }
    netgame w(bserver);
    //singlegame w;
    w.show();

    return a.exec();
}
