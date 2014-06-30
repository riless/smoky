#include "Theater.h"
#include <QGraphicsDropShadowEffect>
#include <QMovie>

Theater::Theater(QWidget* parent ): QFrame( parent ) //,_currentRect(-1)
{
    layout = new QVBoxLayout( this );


    // layout->setContentsMargins(0,0,0,0);

   
    
    layout->addWidget( imageLbl = new QLabel( this ) );
    imageLbl->setObjectName( "theaterLbl" );
    imageLbl->setAlignment(Qt::AlignCenter);


    imageOverlay = new GLabel(imageLbl); // overlay the image
    
    // imageOverlay->hide();
 
    // ADD SHADOW
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setColor( QColor(0,0,0) );
    effect->setOffset(0,0);
    effect->setBlurRadius(7);
    imageLbl->setGraphicsEffect( effect );




    _buttonGroup = new QButtonGroup( this ); // les rect vont appartenir a ce groupe
    connect(_buttonGroup, SIGNAL(buttonClicked(int)), SLOT(onRectClicked(int)) );


}


void
Theater::onRectClicked(int id){
    // _currentRect = id;
    emit rectClicked( id );
}

void
Theater::setImage(QImage image){
    imageLbl->setPixmap( QPixmap::fromImage(image).scaled(
         QSize(MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT),
         Qt::KeepAspectRatio,
         Qt::SmoothTransformation  ) );

}


void
Theater::showRects(QList<QRect> rects){

    clearFaces(); // clear existing rects
    for (int i = 0; i < rects.size(); ++i){
        // must add an index starting from 0, because of QButtonGroup::addButton()
        addRect( rects.at(i), i ); 
    }
}

void
Theater::addRect( QRect rect, int index ){
    int leftMargin = int( ( MAX_DISPLAY_WIDTH - imageLbl->pixmap()->width() )/ 2 );
    int topMargin = int( ( MAX_DISPLAY_HEIGHT - imageLbl->pixmap()->height() )/ 2 );
    
    QRect newRect = QRect(
          rect.left() + imageLbl->geometry().left() + leftMargin,
          rect.top() + imageLbl->geometry().top() + topMargin,
          rect.width(),
          rect.height()
    );
    FaceRect *faceRect = new FaceRect( this, newRect );
    _buttonGroup->addButton( faceRect, index );
    faceRects << faceRect;

    
}

FaceRect*
Theater::rectAt(int rectId){
    return faceRects.at(rectId);
}



void
Theater::clearImage(){
    imageLbl->setPixmap( QPixmap() );
}


void
Theater::clearFaces(){
   foreach(FaceRect* b, faceRects){
        delete b;
   }
   faceRects.clear();
}
