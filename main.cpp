#include "board.h"
#include "stone.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    board w;
    w.show();

    return a.exec();
}
