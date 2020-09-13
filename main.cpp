#include "board.h"
#include "singlegame.h"
#include "choosedlg.h"
#include "mainwidget.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    choosedlg dlg;
    if(dlg.exec()!=QDialog::Accepted)
        return 0;
    mainwidget w(dlg._selectid);
    w.show();



    return a.exec();
}
