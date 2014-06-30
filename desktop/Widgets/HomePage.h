#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "../core.h"

#include "ProcessImage.h"
#include "DropArea.h"
#include "LoadView.h"

class HomePage: public QStackedWidget
{
    Q_OBJECT
public:

    QScrollArea *processImageScrollArea;
    class ProcessImage *processImage;
    class LoadView *loadView;
    QVBoxLayout *layout;
    HomePage(QWidget* parent = 0);

public slots:
    void setCurrentView(int viewIndex);
    void onFilesLoaded( QStringList files );
};

#endif // HOMEPAGE_H
