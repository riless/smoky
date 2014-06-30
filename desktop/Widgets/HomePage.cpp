#include "HomePage.h"

    HomePage::HomePage(QWidget* parent):QStackedWidget(parent)
{

    this->addWidget( loadView = new LoadView( this ) );         // 0
    this->addWidget( processImage = new ProcessImage( this ) ); // 1
    connect( loadView, SIGNAL( filesLoaded(QStringList)), SLOT( onFilesLoaded(QStringList)) );

    connect( processImage, SIGNAL(goToView(int)), SLOT(setCurrentView(int)) );
    connect( processImage, SIGNAL(filesLoaded(QStringList)), SLOT( onFilesLoaded(QStringList)) );

}

void
HomePage::setCurrentView(int viewIndex){
    this->setCurrentIndex( viewIndex );
}

void
HomePage::onFilesLoaded( QStringList files ){

    setCurrentView(1);                  // go to processImage after finished
    processImage->addImages( files );   // process filtered files
}


