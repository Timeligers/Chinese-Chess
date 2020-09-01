#ifndef STONE_H
#define STONE_H

#include <QWidget>
#include <QString>
#include <QDebug>

class stone : public QWidget
{
    Q_OBJECT
public:
    explicit stone(QWidget *parent = nullptr);

    enum Type{jiang,shuai,shi,xiang,ma,che,pao,bing,zu};
    int _row;
    int _col;
    int _id;
    int _deadnum;//死亡顺序。
    bool _dead;//是否死亡。
    bool _red;
    bool _turnup=false;
    Type _type;


    QString getText()
    {
        switch(this->_type)
        {
        case jiang:
            return "将";
        case shuai:
            return "帅";
        case shi:
            if(_red)
            {return "仕";}
            else
            {return "士";}
        case xiang:
            if(_red){
                return "相";
            }
            else {
                return "象";
            }
        case ma:
            return "马";
        case che:
            return "车";
        case pao:
            return "炮";
        case bing:
            return "兵";
        case zu:
            return "卒";
        }
      return "错误";
    }


    void init(int id,int m)
    { Type pos[32]={stone::jiang,stone::shuai,
                    stone::shi,stone::shi,stone::shi,stone::shi,
                    stone::xiang,stone::xiang,stone::xiang,stone::xiang,
                    stone::che,stone::che,stone::che,stone::che,
                    stone::ma,stone::ma,stone::ma,stone::ma,
                    stone::pao,stone::pao,stone::pao,stone::pao,
                    stone::zu,stone::bing,stone::zu,stone::bing,
                    stone::zu,stone::bing,stone::zu,stone::bing,
                    stone::zu,stone::bing};
        _row=m/8;
        _col=m%8;
        _type=pos[id];
        _red=id%2;
        _id=id;
        _dead=false;
    }

signals:

public slots:
};

#endif // STONE_H
