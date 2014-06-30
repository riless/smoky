#include "DropArea.h"

DropArea::DropArea( QWidget* parent): QFrame(parent) {
    delete this->layout();
    this->setAcceptDrops( true );
    this->setProperty("stat", "dropLeave");
}


QStringList
DropArea::filter( QStringList files ){


    QStringList allowedExt, filesToKeep;
    allowedExt << "jpeg" << "tiff" << "png" << "bmp" << "pgm";

    for (int i = 0; i < files.size(); ++i){
        if ( !files.empty() && allowedExt.contains( QImageReader::imageFormat( files.at(i) ) ) ){
            filesToKeep << files.at(i);
        }
    }

    return filesToKeep;
}

void
DropArea::onBrowseImages(){
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Selectionnez un ou plusieurs fichiers",
         QDir::homePath()+"//Desktop",
        "Images (*.png *.xpm *.jpg *.bmp *.pgm)");
    emit filesLoaded ( filter( files ) );
}

void
DropArea::dragEnterEvent(QDragEnterEvent * event){
    event->accept();
    this->setProperty("stat", "dropEnter");
    this->setStyle(QApplication::style());

}

void
DropArea::dragLeaveEvent(QDragLeaveEvent * event){
    event->accept();
    this->setProperty("stat", "dropLeave");
    this->setStyle(QApplication::style());
}

void
DropArea::dragMoveEvent(QDragMoveEvent * event){
    event->accept();
}

void
DropArea::dropEvent(QDropEvent * event){
    event->accept();

    // Convert list to LocalFiles
    QList<QUrl> filesUrl = event->mimeData()->urls();
    QStringList files;
    for ( int i=0; i < filesUrl.size(); ++i ){
        // qDebug() << filesUrl.at(i).toLocalFile();
        files.append( filesUrl.at(i).toLocalFile() );
    }
    this->setProperty( "stat", "dropLeave" );
    this->setStyle(QApplication::style());

    QStringList filesToKeep = filter( files );
    if ( filesToKeep.size() > 0){
        emit filesLoaded( filesToKeep );
    }
}

void
DropArea::enterEvent(QEvent * event){
    event->accept();
}
