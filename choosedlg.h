#ifndef CHOOSEDLG_H
#define CHOOSEDLG_H

#include <QDialog>
#include <QPushButton>

class choosedlg:public QDialog
{
    Q_OBJECT
public:
    explicit choosedlg(QWidget *parent =0);

    QPushButton *button[2];
    int _selectid;

public slots:
    void selection();
};

#endif // CHOOSEDLG_H
