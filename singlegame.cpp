#include "singlegame.h"

singlegame::singlegame(QWidget *parent) :board (parent)
{
    level=4;
}

void singlegame::click(int id,int row,int col)
{
    if(start==false)//判定先手。
    {
        _redstep=_s[id]._red;
        _redfirst=_s[id]._red;
        start=true;
    }
    if(_redfirst==_redstep)
    {
        board::click(id,row,col);
        if(_redfirst!=_redstep)
        {
             QTimer::singleShot(100,this,SLOT(computerMove()));
        }
    }
}
void singlegame::computerMove()
{
    step* step = getBestMove();
    if(step==nullptr)
    {
        new_game();
        return;
    }
    if(step->_killid!=-1)
    {
        moveStone(step->_moveid, step->_killid);
        _s[step->_killid]._dead=true;
        D++;
    }
    else
    {
        moveStone(step->_moveid,step->_rowTo,step->_colTo);
        if(step->_colTo==step->_colFrom&&step->_rowTo==step->_rowFrom)
        {
            _s[step->_moveid]._turnup=true;
        }
    }
    delete step;
    update();
}

step* singlegame::getBestMove()
{
    step* ret = NULL;
    QVector<step*> steps;
    getAllpossiblemoveMain(steps);
    int maxInAllMinScore = -300000;
    //qDebug()<<steps.count();
    while(steps.count())
    {
        step* step = steps.last();
        steps.removeLast();
        fakemove(step);
        int minScore;
        if(_redfirst)
        {
            minScore = getminscore(this->level-1, maxInAllMinScore);
        }
        else
        {
            minScore = -getminscore(this->level-1, maxInAllMinScore);
        }
        unfakemove(step);


        if(minScore > maxInAllMinScore)
        {
            if(ret) delete ret;

            ret = step;
            maxInAllMinScore = minScore;
        }
        else
        {
            delete step;
        }
    }
    return ret;
}

int singlegame::score()
{
  //  enum TYPE{jiang,shuai,shi,xiang,che,ma,pao,zu,bing};
    static int s[] = {1500,1500,900,750,600,450,350,50,50};
    int scoreBlack = 0;
    int scoreRed = 0;
    for(int i=1; i<32; i+=2)
    {
        if(_s[i]._dead) continue;
        if(_s[i]._turnup) continue;
        scoreRed += s[_s[i]._type];
    }
    for(int i=0; i<32; i+=2)
    {
        if(_s[i]._dead) continue;
        if(_s[i]._turnup) continue;
        scoreBlack += s[_s[i]._type];
    }
    return scoreBlack - scoreRed;
}

int singlegame::getminscore(int Level, int curMin)
{
    if(Level == 0)
        return score();

    QVector<step*> steps;
    getAllpossiblemove(_redfirst,steps);
    int minInAllMaxScore = 300000;
    while(steps.count())
    {
        step* Step = steps.last();
        steps.removeLast();
        fakemove(Step);
        int maxScore ;
        if(_redfirst)
        {
            maxScore = getminscore(Level-1, minInAllMaxScore);
        }
        else
        {
            maxScore = -getminscore(Level-1, minInAllMaxScore);
        }
        unfakemove(Step);
        delete Step;

        if(maxScore <= curMin)
        {
            while(steps.count())
            {
                step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }


    }
    return minInAllMaxScore;
}
int singlegame::getmaxscore(int Level, int curMax)
{
    if(Level == 0)
        return score();

    QVector<step*> steps;
    getAllpossiblemove(!_redfirst,steps);
    int maxInAllMinScore = -300000;

    while(steps.count())
    {
        step* Step = steps.last();
        steps.removeLast();

        fakemove(Step);
        int minScore;
        if(_redfirst)
        {
            minScore = getminscore(Level-1, maxInAllMinScore);
        }
        else
        {
            minScore = -getminscore(Level-1, maxInAllMinScore);
        }
        unfakemove(Step);
        delete Step;

        if(minScore >= curMax)
        {
            while(steps.count())
            {
                step* step = steps.last();
                steps.removeLast();
                delete step;
            }
            return minScore;
        }
        if(minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
        }
    }
    return maxInAllMinScore;
}


void singlegame::fakemove(step *step)
{

    killStone(step->_killid);
    if(step->_colTo==step->_colFrom&&step->_rowTo==step->_rowFrom)
    {
        _s[step->_moveid]._turnup=true;
    }
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void singlegame::unfakemove(step *step)
{
    reliveStone(step->_killid);
    if(step->_colTo==step->_colFrom&&step->_rowTo==step->_rowFrom)
    {
        _s[step->_moveid]._turnup=false;
    }
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

void singlegame::getAllpossiblemoveMain(QVector<step *> &steps)
{
    if(_redfirst)
    {
        getAllpossiblemove(0,steps);
    }
    else
    {
        getAllpossiblemove(1,steps);
    }
}

void singlegame::getAllpossiblemove(bool redfirst,QVector<step *> &steps)
{
    int i;
    if(redfirst) {i=1;}
    else {i=0;}
    for(;i<32;i=i+2)
    {
        if(_s[i]._dead) continue;
        if(_s[i]._turnup)
        {
            for(int row=0;row<4;row++)
            {
                for(int col=0;col<8;col++)
                {
                    int killid=getStoneId(row,col);
                    if(killid==-1)
                    {
                        if(canmove(i,row,col))
                        {
                            savestep(i,row,col,steps);
                        }
                    }
                    else
                    {
                        if(_s[killid]._red==_s[i]._red) continue;
                        if(!_s[killid]._turnup) continue;
                        if(canwin(i,killid))
                        {
                            savestep(i,killid,steps);
                        }
                    }
                }
            }
        }
    }  
    for(int k=0;k<32;k++)
    {
        int c=qrand()%32;
        if(!_s[c]._turnup)
        {
            savestep(c,_s[c]._row,_s[c]._col,steps);
            break;
        }
    }
}
