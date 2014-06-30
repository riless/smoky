#ifndef DETECTEDFACESITEM_H
#define DETECTEDFACESITEM_H

#include "../core.h"

class DetectedFacesItem: public QPushButton
{
    Q_OBJECT
public:
    DetectedFacesItem(QImage face, QWidget *parent = 0);
    void is(QString firstName, QString lastName);
    void setStat(QString stat);
private:
    QLabel *avatarLbl;
    QLabel *completeNameLbl;
    QLabel *infoLbl;
    // DataBase& smoky_db;


signals:
    void ItemClicked( int itemId );
};

#endif // DETECTEDFACESITEM_H
