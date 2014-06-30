#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap("res/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    a.processEvents();

    /* operations avant le démarrage */
    loadCSS();

    // afficher le spalsh screen pendant 3s au moins
    QThread::sleep( 3 );

    MainWindow w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
