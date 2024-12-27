#include "singlegame.h"
#include <QMessageBox>

singlegame::singlegame(QWidget *parent) :board (parent)
{
    level=4;
}

void singlegame::click(int id,int row,int col)
{
    if(start==false)
    {
        _redfirst=_s[id]._red;
        _redstep=_s[id]._red;
        start=true;
    }
    if(_redfirst==_redstep)
    {
        board::click(id,row,col);
        if(start==false)
            return;
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
        if(_redfirst)
        {
            QMessageBox::information(NULL,"游戏结束","红方胜利");
            new_game();
            return;
        }
        else
        {
            QMessageBox::information(NULL,"游戏结束","黑方胜利");
            new_game();
            return;
        }
    }
    if(step->_killid!=-1)
    {
        savestep(step->_moveid,step->_killid,steps);
        moveStone(step->_moveid, step->_killid);
        _s[step->_killid]._dead=true;
        D++;
    }
    else
    {
        savestep(step->_moveid,step->_rowTo,step->_colTo,steps);
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
    QVector<step*> STEPS;
    getAllpossiblemoveMain(STEPS);
    int maxInAllMinScore = -300000;
    while(STEPS.count())
    {
        step* step = STEPS.last();
        STEPS.removeLast();
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

    QVector<step*> STEPS;
    getAllpossiblemove(_redfirst,STEPS);
    int minInAllMaxScore = 300000;
    while(STEPS.count())
    {
        step* Step = STEPS.last();
        STEPS.removeLast();
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
            while(STEPS.count())
            {
                step* step = STEPS.last();
                STEPS.removeLast();
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

    QVector<step*> STEPS;
    getAllpossiblemove(!_redfirst,STEPS);
    int maxInAllMinScore = -300000;

    while(STEPS.count())
    {
        step* Step = STEPS.last();
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
            while(STEPS.count())
            {
                step* step = STEPS.last();
                STEPS.removeLast();
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
        int c = generator->bounded(0,32);
        if(!_s[c]._turnup)
        {
            savestep(c,_s[c]._row,_s[c]._col,steps);
            break;
        }
    }
}

void singlegame::back()
{
    if(_redstep==_redfirst)
    {
        step* Step1=steps.last();
        steps.removeLast();
        step* Step2=steps.last();
        steps.removeLast();

        if(Step2->_colTo==Step2->_colFrom&&Step2->_rowTo==Step2->_rowFrom)
        {
            QMessageBox::information(nullptr,"悔棋","己方翻棋无法悔棋");
            delete Step1;
            delete Step2;
            return;
        }
        if(Step1->_colTo==Step1->_colFrom&&Step1->_rowTo==Step1->_rowFrom)
        {
            QMessageBox::information(nullptr,"悔棋","电脑翻棋无法悔棋");
            delete Step1;
            delete Step2;
            return;
        }
        backone(Step1);
        backone(Step2);
    }
}
