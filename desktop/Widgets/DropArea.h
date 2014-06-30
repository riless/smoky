#ifndef DROPAREA_H
#define DROPAREA_H

#include "../core.h"

class DropArea: public QFrame
{
    Q_OBJECT
public:
    // DropArea(){} // prevent instanciation
    DropArea( QWidget* = 0 );
protected:
    void dragEnterEvent(QDragEnterEvent * event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void dragMoveEvent(QDragMoveEvent * event);
    void dropEvent(QDropEvent * event);
    void enterEvent(QEvent * event);
private:
    QStringList filter(QStringList files);
public slots:
    void onBrowseImages();

signals:
    void filesLoaded( QStringList files );
};

#endif // DROPAREA_H
