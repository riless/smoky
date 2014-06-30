#ifndef GPUSHBUTTON_H
#define GPUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class GPushButton: public QPushButton
{
    Q_OBJECT
public:
    GPushButton(QWidget *parent = 0): QPushButton(parent)
    {

    }

protected:
    void
    mousePressEvent(QMouseEvent *event){
        if(event->button() == Qt::RightButton)
            emit dblClicked();
        else
            QPushButton::mousePressEvent(event);
    }

signals:
    void dblClicked();
};

#endif // GPUSHBUTTON_H
