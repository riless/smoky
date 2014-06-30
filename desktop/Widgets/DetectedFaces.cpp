#include "DetectedFaces.h"


DetectedFaces::DetectedFaces( QWidget* parent ): QFrame( parent )
{

    this->setObjectName( "detectedFaces" );
    layout = new QVBoxLayout( this ) ;

    layout->addWidget( noFaceLbl = new QLabel( "Aucun visage n'as été trouvé !" ), 1, Qt::AlignHCenter | Qt::AlignVCenter );
    noFaceLbl->hide();

    _buttonGroup = new QButtonGroup( this );
    _buttonGroup->setExclusive( true );

    connect( _buttonGroup, SIGNAL( buttonClicked(int) ), SLOT(onItemSelected(int) )  );


    // layout->insertStretch(-1, 1);
}

void
DetectedFaces::onItemSelected(int index){
    emit itemClicked( index );
}

DetectedFacesItem*
DetectedFaces::itemAt(int rectId){
    return detectedFaces.at(rectId);
}

void
DetectedFaces::addItems(QList<QImage> imgs){
    clear();
    for (int i = 0; i < imgs.size(); ++i){
        addItem( imgs.at(i), i );
    }
    layout->addStretch(0);

    // SELECT THE FIRST
    if (imgs.size() > 0){
        itemAt(0)->animateClick();
    }
}

void
DetectedFaces::addItem( QImage face, int index ){

     DetectedFacesItem *faceItem;
     layout->addWidget( faceItem = new DetectedFacesItem(face, this) );
     _buttonGroup->addButton( faceItem, index  ); // start index from 0
     detectedFaces << faceItem;

}

int
DetectedFaces::size(){
    return detectedFaces.size();
}


void
DetectedFaces::clear(){
    noFaceLbl->hide();
  
    foreach(DetectedFacesItem* f, detectedFaces){
        delete f;
    }
    
    detectedFaces.clear();
  
}


void
DetectedFaces::noFace(){
    noFaceLbl->show();
}
