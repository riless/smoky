#include "FaceRect.h"
#include <QVariant>

FaceRect::FaceRect(QWidget* parent, QRect rect)
    : QPushButton( parent )
{
    // _rectId = rectId;

    // listFaces.append( this );
    // this->rect = rect;

    this->setCheckable( true );
    this->setAutoExclusive( true );
    this->setGeometry( rect );
    this->setObjectName( "rect" );
    this->setStat("unknown");

    this->show();

    // connect(this, SIGNAL(clicked()), SLOT( onRectClicked() ) );
}

void
FaceRect::setStat(QString stat){
    this->setProperty("faceRect", stat );
    this->setStyle( QApplication::style() );
}

/*int
FaceRect::getPersonId(){
    return _personId;
}*/

/*void
FaceRect::setPersonId(int id){
    _personId = id;
}*/
/*
void
FaceRect::onRectClicked(){
    emit rectClicked( _rectId );
}*/
