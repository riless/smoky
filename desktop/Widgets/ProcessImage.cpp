#include "ProcessImage.h"


ProcessImage::ProcessImage( QWidget* parent ): DropArea(parent), _currentItem(-1), _currentImage(-1)
{
    layout = new QHBoxLayout( this );
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QFrame *processLeftColFrame = new QFrame();
    processLeftColFrame->setObjectName( "processLeftColFrame" );
    
    QVBoxLayout *leftLayout = new QVBoxLayout( processLeftColFrame );
    layout->addWidget( processLeftColFrame );

        QHBoxLayout *topLayout = new QHBoxLayout();

        topLayout->addWidget( backButton = new QPushButton( this ) );
        backButton->setText( "Choisir un autre media" );
        backButton->setObjectName("backButton");
        connect(backButton, SIGNAL(clicked()), SLOT(onBackButtonClicked()) );

        topLayout->addWidget( newSearch = new QPushButton( "Nouvelle recherche" ) );
        newSearch->setObjectName("newSearch");
        connect( newSearch, SIGNAL(clicked()), SLOT(onNewSearch()) );


        topLayout->addStretch(0);
        topLayout->addWidget( nbFacesLbl = new QLabel( this ) );
        nbFacesLbl->setObjectName( "nbFacesLbl" );


        /*
        overlap = new QLabel( this );
        overlap->setObjectName( "overlapLoader" );
        QMovie *loaderMovie = new QMovie(":/res/loader.gif");
        overlap->setMovie( loaderMovie );
        loaderMovie->start()
        */
        // overlap->setGeometry( imageLbl->geometry() );

        // topLayout->addWidget( overlap );

        leftLayout->addLayout( topLayout );



        leftLayout->addWidget( theater = new Theater( this ) );
        theater->imageOverlay->setOverlayText("Traitement en cours...");
        connect( theater, SIGNAL(rectClicked(int)), SLOT(onPersonSelected(int)) );
        


        QScrollArea *imagesScrollArea = new QScrollArea( this );
        leftLayout->addWidget( imagesScrollArea );

        imagesScrollArea->setWidgetResizable( true );
        imagesScrollArea->setObjectName( "imagesScrollArea" );
        imagesScrollArea->setWidget( showCase = new ShowCase( this ) );


        connect( showCase, SIGNAL(imageSelected(int)), SLOT(loadImageData(int)) );

    // COL 3
    layout->addWidget( sidePanel = new SidePanel(this, "view") );
    connect(sidePanel, SIGNAL(confirmChanges(QJsonObject)), SLOT(savePanelChanges(QJsonObject)) );

    connect( sidePanel, SIGNAL(confirmPerson()), SLOT( onConfirmPerson()) );
    connect( sidePanel, SIGNAL(refutePerson()), SLOT( onRefutePerson()) );



    //COL2
    QVBoxLayout* col2 = new QVBoxLayout();
    col2->setSpacing(0);

    col2->addWidget( detectedFacesScrollArea = new QScrollArea( this ) );
    detectedFacesScrollArea->setWidgetResizable( true );
    detectedFacesScrollArea->setObjectName( "detectedFacesScrollArea" );
    detectedFacesScrollArea->setWidget( detectedFaces = new DetectedFaces( this ) );
    
    layout->addLayout( col2 );
    connect( detectedFaces, SIGNAL(itemClicked(int)), SLOT(onPersonSelected(int)) );

    connect( &Socket::sock(), SIGNAL(faceRecognized(int, int, int, int, int, int)), SLOT(onFaceRecognized(int, int, int, int, int, int)) );
    connect( &Socket::sock(), SIGNAL(faceIs(int, QString, QString)), SLOT(onFaceIs(int, QString, QString)) );
    connect( &Socket::sock(), SIGNAL(got(QJsonObject, QByteArray)), sidePanel, SLOT(populate(QJsonObject, QByteArray)) );
}


void
ProcessImage::onNewSearch(){
    showCase->clear();
    clear();
    
    theater->clearFaces();    // les cadres des visages & données
    detectedFaces->clear();   // les visages detecté & données

    
    theater->clearImage();    // l'image chagré dans theater
    sidePanel->clear();       // les info du sidepanel
}
void
ProcessImage::onRefutePerson(){
    if (showCase->exists( _currentImage, _currentItem ) ){
        Socket::sock().refute( showCase->getRatio( _currentImage, _currentItem ) );
        showCase->setPersonId( _currentImage, _currentItem, -1 );

        showCase->setVoted( _currentImage, _currentItem );
        sidePanel->clear();
        detectedFaces->itemAt(_currentItem)->is( "", "" );
    }
}

void
ProcessImage::onConfirmPerson(){
    if (showCase->exists( _currentImage, _currentItem ) ){
        int personId = sidePanel->get( "personId" ).toInt();
        QImage newFace = sidePanel->faceLbl->pixmap()->toImage();

        qDebug() << "confirm personId " << personId << "with face length " << newFace.size();

        Socket::sock().confirm( personId, MatToQByteArray(QImage2Mat(newFace)), showCase->getRatio( _currentImage, _currentItem ) );
        showCase->setVoted( _currentImage, _currentItem );
    }
}


void
ProcessImage::onBackButtonClicked(){
    emit goToView(0);
}

void
ProcessImage::setNbFaces( int nbFaces ){
    QString nbFacesStr;
    nbFacesStr = ( nbFaces > 0)? QString( "%1 visage(s) détécté(s)" ).arg( nbFaces ) : QString( "Aucun visage détécté" );
    nbFacesLbl->setText( nbFacesStr );
}


void
ProcessImage::onFaceIs(int faceId, QString firstName, QString lastName){
    qDebug() << "face is > " << firstName << lastName;
    detectedFaces->itemAt(faceId)->is( firstName, lastName );
    // Socket::sock().get( personId )
}

void
ProcessImage::loadImageData(int index){

    qDebug() << "load data of image " << index;
    // sidePanel->clear();

    // if (index != _currentImage){
        if ( index > -1){
            if ( showCase->done(index) ){
                theater->imageOverlay->hide();
            } else {
                theater->imageOverlay->show();
            }
            _currentImage = index;
            _currentItem = -1;

            theater->setImage( showCase->getImage( index ) );
            theater->showRects( showCase->getRects( index ) );

            removeSpacers( detectedFaces->layout );
            detectedFaces->addItems( showCase->getFaces( index ) );

            QList<int> personsIds = showCase->getPersonsIds( index );

            for (int i = 0; i < personsIds.size(); ++i){
                if (personsIds.at(i) > 0){
                    theater->rectAt(i)->setStat( "known" );
                    Socket::sock().is(i, personsIds.at(i) );
                    // detectedFaces->itemAt(i)->is( personsIds.at(i) );
                }
            }

            if ( personsIds.size() > 0){
                detectedFacesScrollArea->show();
            } else {
                sidePanel->clear();
                sidePanel->hideVoteButtons();
                sidePanel->setRatio(0,0,0);
            }
            setNbFaces( personsIds.size() );

        } else {
            qWarning() << "index < -1 on ProcessImage::onImageSelected";
        }
    // }
}

void
ProcessImage::savePanelChanges(QJsonObject data){
    /*
    if(personId <= 0) // -1 = nobody detected
        personId = DataBase::dbi().getNextId("persons");

    if ( sidePanel->valuesOk() ){
        DataBase::dbi().save( personId, sidePanel->get("firstName"), sidePanel->get("lastName"), "" );

        // THESE STEPS CAN BE RUN ON A THREAD
        // recognizer->clearNewFaces();
        // recognizer->addNewFace( _detectorData.at(_currentItem), personId ); // IMAGE, LABEL
        // recognizer->updateModels();
        // recognizer->saveModels();

        theater->rectAt(_currentItem)->setStat("known");
        // detectedFaces->itemAt(_currentItem)->is(personId);
    } else {
        // warning wrong inputs
    }
    */
}



void
ProcessImage::onPersonSelected(int itemId){

    if ( _currentItem != itemId ){
        if ( itemId > -1 && itemId < detectedFaces->size() ){
            _currentItem = itemId;

            qDebug() << "voted stats for " << _currentImage << _currentItem << "is: " << showCase->voted( _currentImage, _currentItem );
            // show/hide Votes Buttons
            if ( showCase->voted( _currentImage, _currentItem ) == true ){
                sidePanel->hideVoteButtons();
            } else {
                sidePanel->showVoteButtons();
            }

            theater->rectAt(itemId)->setChecked( true );
            detectedFaces->itemAt(itemId)->setChecked( true );

            detectedFacesScrollArea->ensureWidgetVisible( detectedFaces->itemAt(itemId) );

            int personId = showCase->getPersonId(_currentImage, _currentItem);
            qDebug() << "personId" << personId;
            if (personId > -1){
                Socket::sock().get( personId );


                QList<int> l = showCase->getRatio( _currentImage, _currentItem );
                sidePanel->setRatio( l[0], l[1], l[2] );
            } else {
                sidePanel->setRatio( 0,0,0 );
                sidePanel->clear();
            // sidePanel->load( showCase->getPersonId(_currentImage, _currentItem) );
            }
            sidePanel->show();
        } else {
            qDebug() << "itemId=" << itemId << "index out of range in onPersonSelected";
        }
    }
}


void
ProcessImage::clear(){
    _imagesList.clear();
}


void
ProcessImage::addImages(QStringList imagesList){
    // _imagesList.clear();
    // showCase->clear();

    removeSpacers( showCase->layout );

    // continue from the last counter
    int oldSize = _imagesList.size();
    for (int i = 0; i < imagesList.size(); ++i){
        QImage tmpImg = QImage( imagesList.at(i) );

        _imagesList << tmpImg;

        qDebug() << "add image i =" << i + oldSize;
        showCase->addImage( tmpImg, i+oldSize ); // add image to the showcase with index starting from 0

        process( i + oldSize );
        if (i + oldSize == 0) {
            showCase->at(0)->animateClick();
        }
    }
    showCase->layout->addStretch(0); // compacte images


    // processAll();

}

void
ProcessImage::processAll(){
    for (int i=0; i < _imagesList.size(); ++i){
        process(i);
        if (i == 0) {
            showCase->at(0)->animateClick();
        }

    }
}

void
ProcessImage::insertNewData(int index, QList<struct DetectorData> detectedData){
    showCase->setDone( index );

    if (detectedData.size() == 0){
        detectedFaces->noFace();

    } else {
        QList<struct FaceData> facedata;
        struct FaceData f;
        for (int i = 0; i < detectedData.size(); i++ ){
            f.rect = detectedData.at(i).rect;
            f.image = detectedData.at(i).image;
            f.personId = -1;
            f.eigen_ratio = 0;
            f.fisher_ratio = 0;
            f.lbph_ratio = 0;
            f.voted = false;
            facedata << f;
        }
        showCase->addCoordinates( index, facedata );
    }

    // RECOGNIZE THE FACES
    // recognizer = new Recognizer();
    for (int i = 0; i < detectedData.size(); i++ ){
        qDebug() << "send image " << i << " to recognition";

        Socket::sock().recognize( detectedData.at(i).mat.clone(), index, i );
        // int personId = recognizer->recognize( detectedData.at(i) );
        // showCase->setPersonId(index, i, personId); // image index, face index, person id
    }

    /* show if current */
    if (index == _currentImage){
        loadImageData( index );
    }
}

void
ProcessImage::onFaceRecognized(int imageId, int faceId, int personId, int eigen_ratio, int fisher_ratio, int lbph_ratio ){
    qDebug() << "imageId, faceId, personId, eigne, fisher, lbph" << imageId << faceId << personId << eigen_ratio << fisher_ratio << lbph_ratio;
    showCase->setPersonId(imageId, faceId, personId); // image index, face index, person id
    showCase->setRecRatio(imageId, faceId, eigen_ratio, fisher_ratio, lbph_ratio); // image index, face index, person id

    if ( _currentImage == imageId && personId > -1){
        theater->rectAt(faceId)->setStat( "known" );
        
        sidePanel->setRatio(eigen_ratio, fisher_ratio, lbph_ratio);


        Socket::sock().get( personId );
        Socket::sock().is(faceId, personId );
        // detectedFaces->itemAt(i)->is( personsIds.at(i) );
    }

}

void
ProcessImage::process(int index){

    if (index > -1 && index < _imagesList.size() ){

        // DETECTE FACES
        FaceDetector *detector = new FaceDetector( index, QImage2Mat( _imagesList.at( index ) ) );
        detector->start();
        connect( detector, SIGNAL(detectionFinished( int, QList<struct DetectorData> )), SLOT( insertNewData(int, QList<struct DetectorData>) ) );
        // _detectorData = detector->getData();

        
    } else {
        qWarning() << "index not in -1 > index > _imagesList.size() rang in ProcessImage::process" << index;
    }
}

