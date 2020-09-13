#include "choosedlg.h"
#include <QVBoxLayout>


choosedlg::choosedlg(QWidget *parent):QDialog (parent)
{
    this->setFixedSize(300,200);
    QVBoxLayout* lay =new QVBoxLayout(this);
    lay->addWidget(button[0]=new QPushButton("人机对战"));
    lay->addWidget(button[1]=new QPushButton("人人对战"));

    for(int i=0;i<2;i++)
    {
        connect(button[i],SIGNAL(clicked()),this,SLOT(selection()));
    }

}

void choosedlg::selection()
{
    QObject* s=sender();//指针 得到哪个按钮被点击的信号。
    for(int i=0;i<2;i++)
    {
        if(button[i]==s)
        {
            this->_selectid=i;
            break;
        }
    }
    accept();
}
