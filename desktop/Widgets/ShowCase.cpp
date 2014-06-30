#include "ShowCase.h"

ShowCase::ShowCase( QWidget* parent ): QFrame(parent)
{	
	// _facesData.voted = 0
    layout = new QHBoxLayout( this );
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    _buttonGroup = new QButtonGroup( this );
    _buttonGroup->setExclusive( true );
    connect( _buttonGroup, SIGNAL( buttonClicked(int) ), SLOT(onImageSelected(int) )  );
    connect( _buttonGroup, SIGNAL( buttonClicked(QAbstractButton*) ), SLOT(onButtonSelected(QAbstractButton*) )  );
}

QImage
ShowCase::getImage(int index){
	return _images.at( index );
}

QAbstractButton*
ShowCase::at(int index){
    return _buttonGroup->button( index );
}

QList<QRect>
ShowCase::getRects(int key){
	QList<QRect> rects;
	for (int i = 0; i < _facesData[key].size(); ++i){
		rects << _facesData[key].at(i).rect;
	}
	return rects;
}

QList<QImage>
ShowCase::getFaces(int key){
    QList<QImage> images;
	for (int i = 0; i < _facesData[key].size(); ++i){
		images << _facesData[key].at(i).image;
	}
	return images;
}

QList<int>
ShowCase::getPersonsIds(int key){
	QList<int> personsIds;
	for (int i = 0; i < _facesData[key].size(); ++i){
		personsIds << _facesData[key].at(i).personId;
	}
	return personsIds;
}

void
ShowCase::setDone( int imageId ){
    _stat[imageId] = true;
}

bool
ShowCase::done(int imageId ){
    return _stat.at( imageId );
}

void
ShowCase::addImage(QImage image,int index){
	_images << image;
    _stat << false;

    QPushButton *imageBtn = new QPushButton( this );
    imageBtn->setObjectName("imageBtn");
    imageBtn->setCheckable( true );
    _buttonGroup->addButton( imageBtn, index );

	QVBoxLayout *btnLayout = new QVBoxLayout( imageBtn );
	btnLayout->setContentsMargins(0,0,0,0);
	btnLayout->setSpacing(0);

	QLabel *imageLbl = new QLabel();
	imageLbl->setObjectName( "imageLbl" );
    imageLbl->setPixmap( QPixmap::fromImage( image.scaled(QSize(75, 75), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation) ) );
	btnLayout->addWidget( imageLbl );

	_buttons << imageBtn;
	layout->addWidget( imageBtn );
}

void
ShowCase::onButtonSelected(QAbstractButton* button){
    emit buttonSelected( button );
}

void
ShowCase::onImageSelected(int index){
	emit imageSelected( index );
}

int
ShowCase::getPersonId(int key, int index){
	return _facesData[key].at(index).personId;
}

void
ShowCase::setVoted(int key, int index){
	qDebug() << key << index << "voted stat is set to true";
    _facesData[key][index].voted = true;
}

bool
ShowCase::voted( int key, int index ){
    return _facesData[key].at(index).voted;
}

QList<int>
ShowCase::getRatio(int key, int index){
	QList<int> l;
	l << _facesData[key].at(index).eigen_ratio;
	l << _facesData[key].at(index).fisher_ratio;
	l << _facesData[key].at(index).lbph_ratio;
	return l;
}

void
ShowCase::addCoordinates( int key, QList<struct FaceData> facedata ){

	_facesData[key] = facedata;

}

bool
ShowCase::exists(int key, int index){

    if (_facesData.contains(key) && _facesData[key].size() < index )
		return true;
	return false;
}

void
ShowCase::setRecRatio(int key, int index, int eigen_ratio, int fisher_ratio, int lbph_ratio){
	_facesData[key][index].eigen_ratio = eigen_ratio;	
	_facesData[key][index].fisher_ratio = fisher_ratio;	
	_facesData[key][index].lbph_ratio = lbph_ratio;	
}

void 
ShowCase::setPersonId(int key, int index, int personId){
    _facesData[key][index].personId = personId;
}

void
ShowCase::deleteButton(QAbstractButton* b){
    if ( _buttonGroup->id(b) > -1 ){
        _buttonGroup->removeButton( b );
        delete b;

        // reconstruct
        QList<QAbstractButton*> buttons = _buttonGroup->buttons();
        foreach (QAbstractButton* b, buttons){
            _buttonGroup->removeButton(b);
        }

        int i = 0;
        foreach (QAbstractButton* b, buttons){
            _buttonGroup->addButton(b, i++);
        }
    }
}

void
ShowCase::deleteAt( int index ){
	// QAbstractButton* b = _buttonGroup->button( index );
	QList<QAbstractButton*> buttonList = _buttonGroup->buttons();

    qDebug() << index << "must be < to " << buttonList.size() << " to delete";

	if (index < buttonList.size() ){
    	_buttonGroup->removeButton( buttonList[index] );
        delete buttonList[index];
	}
}

void
ShowCase::clear(){
/*	foreach(QAbstractButton *button, _buttonGroup->buttons()) {
	   _buttonGroup->removeButton( button );
	}*/

/*	QLayoutItem* item;
    while ( ( item = layout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }*/

	foreach(QAbstractButton* b, _buttons){
        if ( _buttonGroup->id(b) > -1 ){
            _buttonGroup->removeButton( b );
            delete b;
        }
	}


	_buttons.clear();
	_stat.clear();
	_images.clear();
	_facesData.clear();

}
