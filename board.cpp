#include "board.h"
#include <QDebug>

board::board(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(_r*35+1, _r*30+1);
    new_game();
}

board::~board()
{

}

void board::new_game()
{
    QVector<step*> pNullVector;
    steps.swap(pNullVector);
    srand();
    for(int i=0;i<32;i++)
    {
        _s[i].init(i,c[i]);
        _s[i]._dead=false;
        _s[i]._turnup=false;
    }
    _selectid=-1;
    start=false;
    D=0;
    update();
    return;
}

void board::srand()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
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

void board::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.save();
    drawPlate(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    drawStone(p);
    p.restore();
}

void board::drawPlate(QPainter &painter)
{
    int d=_r*2;
    for(int i=1;i<=10;i++)//画10条横线。
        {
            painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
            painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));//两点连线。
        }

    //画9条竖线。
    for(int i=1;i<=9;i++)
        {
            painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
            if(i==1||i==9)
                painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
            else
            {
                painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
                painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
            }
        }
}

void board::drawPlace(QPainter &painter)
{
    int d=2*_r;
    //九宫格。
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));
    return;
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

void board::drawStone(QPainter &painter)
{
    for(int i=0;i<32;i++)
    {
        drawStone(painter,i);
    }
}

void board::drawStone(QPainter &painter,int id)
{
    QPoint c;
    QRect rect;
    if(_s[id]._dead)
    {
        _s[id]._row=_s[id]._deadnum/8+5;
        _s[id]._col=_s[id]._deadnum%8;
    }

    c=center(id);
    rect=QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);

    QColor color=Qt::black;

    painter.setPen(QPen(QBrush(color), 1.5));

    if(id == _selectid) painter.setBrush(Qt::gray);
    else painter.setBrush(Qt::yellow);

    painter.drawEllipse(center(_s[id]._row,_s[id]._col),_r,_r);

    if(_s[id]._red) color = Qt::red;
    painter.setPen(QPen(QBrush(color),1.5));
    if(_s[id]._turnup)
    {
        painter.setFont(QFont("system", _r*1.2, 700));
        painter.drawText(rect, _s[id].getText(), QTextOption(Qt::AlignCenter));
    }
}

void board::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
    {
        return;
    }

    click(event->pos());
}

void board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);
    if(!bClicked) return;

    int clickid = getStoneId(row,col);
//    if(start==false)//判定先手。
//    {
//        _redstep=_s[clickid]._red;
//        _redfirst=_s[clickid]._red;
//        start=true;
//    }
    int id = getStoneId(row, col);
    click(id, row, col);
}

void board::click(int clickid, int row, int col)
{
    if(start==false)//判定先手。
    {
        _redstep=_s[clickid]._red;
        _redfirst=_s[clickid]._red;
        start=true;
    }
    if(_selectid==-1)//选择要操作的棋子。
    {
        if(clickid!=-1)//是否点中了棋子。
        {
            if(_s[clickid]._turnup)//棋子是否已经被翻开。
            {
                if(_s[clickid]._red==_redstep)
                {
                    _selectid=clickid;
                    clickid=-1;
                    update();
                }
            }
            else//翻棋。
            {
                    _s[clickid]._turnup=true;
                    savestep(clickid,_s[clickid]._row,_s[clickid]._col,steps);
                    _redstep=!_redstep;
                    clickid=-1;
                    update();
            }
        }
    }
    else
    {
        if(clickid!=-1)//吃棋
        {
            if(_s[clickid]._turnup)
            {
                if(_s[_selectid]._red==_s[clickid]._red)
                {
                    _selectid=clickid;
                    update();
                    return;
                }
                if(canwin(_selectid,clickid))
                {
                    moveStone(_selectid,clickid);
                    killStone(clickid);
                    D++;
//                    if(victory())
//                        new_game();
                }
            }
        }
        else//走棋。
        {
            if(canmove(_selectid,row,col))
            {
                moveStone(_selectid,row,col);
            }
        }
        _selectid=-1;
        update();
    }
}

bool board::canwin(int a, int b)//吃子
{
   if(canmove(a,_s[b]._row,_s[b]._col))
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
        else if(a>21&&b/2==0)
            return true;
    }
    else
    {
       if ((a/2==9||a/2==10)&&(b<14||b>17)&&getstoneatLine(_s[a]._row,_s[a]._col,_s[b]._row,_s[b]._col)==1)//炮
            return true;
    }
    return false;
}

int board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !_s[i]._dead)
            return i;
    }
    return -1;
}

bool board::getClickRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
                return true;
        }
    }
    return false;
}

bool board::canmove(int id, int row, int col)
{
    if((qAbs(row-_s[id]._row)==1&&col==_s[id]._col)||(qAbs(col-_s[id]._col)==1&&row==_s[id]._row))
        return true;
    else
        return false;

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
    return ret;
}

void board::moveStone(int moveid, int killid)
{
    savestep(moveid,killid,steps);
    _s[moveid]._row=_s[killid]._row;
    _s[moveid]._col=_s[killid]._col;
    _redstep=!_redstep;
    //changestep();
}

void board::moveStone(int id, int row, int col)
{
    savestep(id,row,col,steps);
    _s[id]._row=row;
    _s[id]._col=col;
    _redstep=!_redstep;
    //changestep();
}

void board::killStone(int id)
{
    if(id==-1)
        return;
    _s[id]._dead=true;
    _s[id]._deadnum=D;
    return;
}

void board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}

void board::savestep(int moveid, int killid, QVector<step *> &steps)
{
    step *Step=new step;
    Step->_colFrom=_s[moveid]._col;
    Step->_rowFrom=_s[moveid]._row;
    Step->_colTo=_s[killid]._col;
    Step->_rowTo=_s[killid]._row;
    Step->_moveid=moveid;
    Step->_killid=killid;

    steps.append(Step);
}

void board::savestep(int moveid, int rowto,int colto, QVector<step *> &steps)
{
    step *Step=new step;
    Step->_colFrom=_s[moveid]._col;
    Step->_rowFrom=_s[moveid]._row;
    Step->_colTo=colto;
    Step->_rowTo=rowto;
    Step->_moveid=moveid;
    Step->_killid=-1;

    steps.append(Step);
}
