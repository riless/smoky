#include "DetectedFacesItem.h"
#include <QDebug>

DetectedFacesItem::DetectedFacesItem(QImage face, QWidget *parent) : QPushButton( parent )
{
    //smoky_db = DataBase::dbi();

    this->setCheckable( true );
    // this->setProperty("faceItemType", personType);

    QGridLayout *layout = new QGridLayout( this );
    layout->setContentsMargins(0,0,0,0);
    layout->setColumnStretch(1, 2);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    layout->addWidget( avatarLbl = new QLabel(), 0, 0, 2, 1);
    avatarLbl->setObjectName( "avatarLbl" );
    QPixmap *pixmapAvatar;
    avatarLbl->setPixmap( pixmapAvatar->fromImage( face.scaled(QSize(48, 48), Qt::KeepAspectRatio, Qt::SmoothTransformation) ) );

    layout->addWidget( completeNameLbl = new QLabel( "" ), 0, 1);
    completeNameLbl->setObjectName( "completeNameLbl" );

    layout->addWidget( infoLbl = new QLabel( "" ), 1, 1);
    infoLbl->setObjectName( "infoLbl" );

}

void
DetectedFacesItem::is(QString firstName, QString lastName){
    if ( firstName != "" && lastName != ""){
        setStat("known");
        completeNameLbl->setText( firstName + " " + lastName );
        // infoLbl->setText( result.value("birthday").toString() );
    } else {
        setStat("unknown");
        completeNameLbl->setText( firstName + " " + lastName );
    }
}

void
DetectedFacesItem::setStat(QString stat){
    this->setProperty("faceItem", stat);
    this->setStyle( QApplication::style() );
}
/*
void
DetectedFacesItem::onItemClicked(){
    emit ItemClicked( itemRectId );
}
*/
