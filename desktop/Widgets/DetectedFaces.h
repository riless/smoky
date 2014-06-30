#ifndef DETECTEDFACES_H
#define DETECTEDFACES_H

#include "DetectedFacesItem.h"
#include "../core.h"

class DetectedFaces: public QFrame
{
    Q_OBJECT
public:

    DetectedFaces(QWidget *parent = 0);
    QVBoxLayout *layout;
    QList<DetectedFacesItem*> detectedFaces;
    void clear();
    void noFace();
    DetectedFacesItem* itemAt(int rectId);
    void addItems(QList<QImage> imgs);
    int size();
    
private:
    QButtonGroup *_buttonGroup;
    QLabel *noFaceLbl;
    void addItem( QImage face, int addItem);

public slots:
    void onItemSelected( int id );

signals:
    void itemClicked( int id );
};

#endif // DETECTEDFACES_H
