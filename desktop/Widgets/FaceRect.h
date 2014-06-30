#ifndef FACERECT_H
#define FACERECT_H

#include "../core.h"

class FaceRect: public QPushButton
{
    Q_OBJECT
public:
    FaceRect(QWidget* parent, QRect rect);
    // QString faceRec;
    // QList<QPushButton*> listFaces;
    // QRect rect;
    // int getPersonId();
    // void setPersonId(int id);
    void setStat(QString stat);


public slots:
    //void onRectClicked();

signals:
    //void rectClicked( int rectId );
};

#endif // FACERECT_H
