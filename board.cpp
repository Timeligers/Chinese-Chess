#include "board.h"
#include <QPainter>

board::board(QWidget *parent) : QWidget(parent)
{
    srand();
    for(int i=0;i<32;i++)
    {
        _s[i].init(i,c[i]);
    }
    _selectid=-1;
}

void board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int d=40;
    _r=d/2;

    //画10条横线。
    for(int i=1;i<=10;i++)
        {
            painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));//两点连线。
        }

    //画9条竖线。
    for(int i=1;i<=9;i++)
        {
            if(i==1||i==9)
                painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
            else
            {
                painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
                painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
            }
        }

    //九宫格。
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    //绘制棋子。
    for(int i=0;i<32;i++)
        drawstone(painter,i);

}

QPoint board::center(int row, int col)
{
    QPoint ret;
    ret.rx()=(col+1)*_r*2+_r;
    ret.ry()=(row+1)*_r*2+_r;
    return ret;
}

QPoint board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

void board::drawstone(QPainter &painter, int id)
{
    QPoint c;
    QRect rect;
    if(_s[id]._dead)//已死亡棋子重新安放位置
    {
        _s[id]._col=_s[id]._deadnum%8;
        _s[id]._row=_s[id]._deadnum/8+5;
    }

    c=center(id);
    rect=QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);

    if(id==_selectid)//点击发生改变。
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));

    painter.setPen(QColor(Qt::black));
    painter.drawEllipse(center(_s[id]._row,_s[id]._col),_r,_r);

    if(_s[id]._red)
        painter.setPen(QColor(Qt::red));
    if(_s[id]._turnup)
    {painter.setFont(QFont("system",_r,700));
     painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));
    }
}

void board::srand()
{qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<32;)
    {   int f=0;
        int m=qrand()%32;
        for(int j=0;j<i;j++)
            if(c[j]==m)
            {
                f=1;
                break;
            }
        if(f==0)
        {
            c[i]=m;//位置代码。
            i++;
        }
    }
   return;
}

bool board::getRowcol(QPoint pt, int &row, int &col)//划分每个棋子的控制区域
{
    for(row=0;row<=9;row++)
        for(col=0;col<=8;col++)
        {
            QPoint c=center(row,col);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<_r*_r)
                return true;
        }
    return false;
}

void board::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pt=event->pos();
    int row,col;
    bool bRet = getRowcol(pt,row,col);
    if(bRet==false)
        return;
    int i;
    int clickid=-1;
    for(i=0;i<32;i++)
    {
        if(_s[i]._row==row&&_s[i]._col==col&&_s[i]._dead==false)
             break;
    }
    if(i<32)
    {
        clickid=i;
    }
    if(start==false)
    {
        _redstep=_s[clickid]._red;
        start=true;
    }
    if(_selectid==-1)
    {
        if(_s[i]._turnup)
        {
            if(clickid!=-1)
            {
                if(_s[clickid]._red==_redstep)
                {
                    _selectid=clickid;
                    clickid=-1;
                    update();
                }
            }
        }
        else
        {
            if(clickid!=-1)
            {
                _s[i]._turnup=true;
                _redstep=!_redstep;
                qDebug()<<_redstep;
                clickid=-1;
                update();
            }
        }
    }
    else
    {
        if(canmove(_selectid,row,col))
        {
            if(clickid!=-1)
            {
                if(CanWin(_selectid,clickid))
                {
                    _s[_selectid]._row=row;
                    _s[_selectid]._col=col;
                    _redstep=!_redstep;
                    qDebug()<<_redstep;
                    _s[clickid]._dead=true;
                    _s[clickid]._deadnum=D;
                    D++;
                    update();
                }
            }
            else
            {
                if(getstoneatLine(_s[_selectid]._row,_s[_selectid]._col,row,col)==0)
                {
                    _s[_selectid]._row=row;
                    _s[_selectid]._col=col;
                    _redstep=!_redstep;
                    qDebug()<<_redstep;
                }

            }
        }
        _selectid=-1;
        update();
    }
}

bool board::CanWin(int a, int b)//吃子
{
    if(_s[a]._red==_s[b]._red||_s[b]._turnup==false)
        return false;
    else
    {
        if(a/2==0&&b<22)//将或帅
            return true;
        else if ((a/2==1||a/2==2)&&b>1)//士
            return true;
        else if ((a/2==3||a/2==4)&&b>5)//象
            return true;
        else if ((a/2==5||a/2==6)&&b>9)//车
            return true;
        else if ((a/2==7||a/2==8)&&b>13)//马
            return true;
        else if ((a/2==9||a/2==10)&&(b<14||b>17)&&getstoneatLine(_s[a]._row,_s[a]._col,_s[b]._row,_s[b]._col)==1)//炮
            return true;
        else if(a>21&&b/2==0)
            return true;
    }
    return false;
}

bool board::canmove(int id,int row,int col)
{
    int c=getstoneatLine(_s[id]._row,_s[id]._col,row,col);
    if(id>17&&id<22)
    {if((row==_s[id]._row||col==_s[id]._col)&&(c==0||c==1))
        return true;
     else
        return false;
    }
    else
    {if((qAbs(row-_s[id]._row)==1&&col==_s[id]._col)||(qAbs(col-_s[id]._col)==1&&row==_s[id]._row))
        return true;
     else
        return false;
    }
}

int board::getstoneatLine(int row_f, int col_f, int row_d, int col_d)
{
    int ret=0;
    if(row_d==row_f&&col_d==col_f)
        return -1;
    if(row_d!=row_f&&col_d!=col_f)
        return -1;
    if(row_d==row_f)
    {
        int max=col_d>col_f?col_d:col_f;
        int min=col_d<col_f?col_d:col_f;
        for(int col=min+1;col<max;col++)
        {
            for(int i=0;i<32;i++)
            {
                if(_s[i]._row==row_d&&_s[i]._col==col)
                    ret++;
            }
        }
    }
    if(col_d==col_f)
    {
        int max=row_d>row_f?row_d:row_f;
        int min=row_d<row_f?row_d:row_f;
        for(int row=min+1;row<max;row++)
        {
            for(int i=0;i<32;i++)
            {
                if(_s[i]._col==col_d&&_s[i]._row==row)
                    ret++;
            }
        }
    }
    qDebug()<<ret;
    return ret;
}
