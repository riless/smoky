#ifndef GLABEL_H
#define GLABEL_H

#include "Overlay.h"
#include "../core.h"

class GLabel: public Overlay
{
    Q_OBJECT
public:
    GLabel(QWidget *parent = 0): Overlay(parent) {
        
		setAttribute(Qt::WA_TranslucentBackground);
	}

    void
    setOverlayText(QString text){
        _text = text;
    }
private:
    QString _text;

protected:
    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        p.fillRect(rect(), QColor(0, 0, 0, 220));
        p.setPen(QColor(200, 200, 255, 255));
        p.setFont(QFont("arial,helvetica", 18));
        p.drawText(rect(), _text, Qt::AlignHCenter | Qt::AlignVCenter);
    }

};

#endif // GLABEL_H
