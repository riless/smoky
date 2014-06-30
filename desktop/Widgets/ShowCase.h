#ifndef SHOWCASE_H
#define SHOWCASE_H

#include "../core.h"

class ShowCase: public QFrame
{
    Q_OBJECT
public:
    ShowCase(QWidget *parent);

    void deleteAt( int index );
    void addImage(QImage image, int index);
    void addCoordinates( int key, QList<struct FaceData> facedata );
    void setDone( int imageId );
    void clear();
    void setPersonId(int key, int index, int personId);
    bool done( int imageId );
    void setRecRatio(int key, int index, int eigen_ratio, int fisher_ratio, int lbph_ratio);

    QList<int> getRatio(int key, int index);
    QList<QImage> getFaces(int key);
    QList<QRect> getRects(int key);
    QList<int> getPersonsIds(int key);
    int getPersonId(int key, int index);
    QImage getImage(int index);
    QAbstractButton *at(int index);

    QHBoxLayout *layout;

    void setVoted(int key, int index);
    bool voted(int key, int index);
    bool exists(int key, int index);
private:
    QList<QImage> _images;
    QList<QPushButton*> _buttons;
    QList<bool> _stat;
    QMap<int, QList<struct FaceData> > _facesData;
    QButtonGroup *_buttonGroup;
public slots:
	void onImageSelected(int index);
    void onButtonSelected(QAbstractButton*);
    void deleteButton(QAbstractButton* b);
signals:
    void buttonSelected(QAbstractButton*);
    void imageSelected(int index);
};

#endif // SHOWCASE_H
