#ifndef THEATER_H
#define THEATER_H

#include "../core.h"

#include "GLabel.h"
#include "FaceRect.h"
#include "ShowCase.h"

class Theater: public QFrame
{
    Q_OBJECT
public:
    Theater(QWidget* parent = 0);
    void addRect(QRect rect, int index);
    void clearFaces();
    QList<FaceRect*> faceRects;
    QVBoxLayout *layout;
    void clearImage();
    // int getPersonId( int id );
    // void setPersonId(int id, int personId);
    int currentRect();
    FaceRect* rectAt(int rectId);
    
    
    void showRects(QList<QRect> rects);
    void setImage(QImage image);
    GLabel *imageOverlay;
    QButtonGroup *_buttonGroup;

    QLabel *imageLbl;
private:
    QLabel *overlap;
    
    
    // int _currentRect;

public slots:
    void onRectClicked(int id);

signals:
    void rectClicked( int id );
    
};


#endif // THEATER_H
