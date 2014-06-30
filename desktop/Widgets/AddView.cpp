#include "AddView.h"



AddView::~AddView()
{
    t->quit();
    t->wait();
}

AddView::AddView(QWidget *parent) :
    DropArea(parent), m_currentSelected(-1), m_currentButton(0)
{
	QHBoxLayout *layout = new QHBoxLayout( this );

	if (!_cascade.load("res/haarcascade_frontalface_alt.xml") ) {
        qDebug() << "Erreur lors du chargement du haar cascade frontalface...";
        exit(0);
    }


	// COL 01
	QVBoxLayout *leftLayout = new QVBoxLayout();
	layout->addLayout( leftLayout );

		QHBoxLayout *topLeftLayout = new QHBoxLayout();

		topLeftLayout->addWidget( addLbl = new QLabel( "Prenez au moins 5 photos" ) );
		addLbl->setObjectName( "addLbl" );

		topLeftLayout->addStretch( 0 );

        topLeftLayout->addWidget( newPersonBtn = new QPushButton("Nouvel Individu") );
		newPersonBtn->setObjectName( "newPersonBtn" );
		connect( newPersonBtn, SIGNAL(clicked()), SLOT(onNewPersonClicked()) );

		connect( &Socket::sock(), SIGNAL(personAdded()), SLOT(onNewPersonClicked()) );
		
		leftLayout->addLayout( topLeftLayout );

		QHBoxLayout *centerLeftLayout = new QHBoxLayout();
		leftLayout->addLayout( centerLeftLayout );

		centerLeftLayout->addWidget( chooseMediaFrm = new QFrame() );


		chooseMediaFrm->setObjectName( "chooseMediaFrm" );
        QVBoxLayout *chooseMediaLayout = new QVBoxLayout( chooseMediaFrm );
        chooseMediaLayout->setContentsMargins(0,0,0,0);
        chooseMediaLayout->setSpacing( 0 );

		m_chooseMediaBG = new QButtonGroup();
        m_chooseMediaBG->setExclusive( true );

		chooseMediaLayout->addWidget( chooseCam = new QPushButton("Cam") );
        chooseCam->setCheckable( true );
		chooseCam->setObjectName( "chooseCam" );
		m_chooseMediaBG->addButton( chooseCam, 0 );



		chooseMediaLayout->addWidget( chooseImages = new QPushButton("Image") );
        chooseImages->setCheckable( true );
		chooseImages->setObjectName( "chooseImages" );
		m_chooseMediaBG->addButton( chooseImages, 1 );

/*
		chooseMediaLayout->addWidget( chooseVideo = new QPushButton("Video") );
        chooseVideo->setCheckable( true );
		chooseVideo->setObjectName( "chooseVideo" );
		m_chooseMediaBG->addButton( chooseVideo );
*/

		connect( m_chooseMediaBG, SIGNAL(buttonClicked(int)), SLOT(onChooseMedia(int)) ); 
        chooseMediaLayout->addStretch(0);


		centerLeftLayout->addWidget( theater = new Theater( this ) );
		theater->imageLbl->setProperty("type", "camera");
		theater->imageOverlay->setOverlayText("Ouverture de la camera...");

        // connect( theater, SIGNAL(rectClicked(int)), SLOT(onPersonSelected(int)) );
		centerLeftLayout->addStretch(0);

////////////////////
		cam = new Camera();
		t = new QThread();
		mTimer = new QTimer();
		mTimer->setInterval(1000/30);


		connect(mTimer, SIGNAL(timeout()), cam, SLOT(queryFrame()) );
		connect( this, SIGNAL(startWork()), cam, SLOT(openCamera()) );
        connect( this, SIGNAL(startWork()), mTimer, SLOT(start()) );

	    connect( cam, SIGNAL(stopCamera()), mTimer, SLOT(stop()) );
	    connect( this, SIGNAL(stopWork()), cam, SLOT(closeCamera()) );
		connect(t, SIGNAL(finished()), cam, SLOT(deleteLater()));

		connect( cam, SIGNAL(stopCamera()), SLOT(onStopCamera()) );
       	cam->moveToThread( t );
		mTimer->moveToThread( t );
		t->start( QThread::IdlePriority );
		

////////////////////

        connect(cam, SIGNAL(sendFrame(QImage)), SLOT( onSendFrame(QImage)) );
        connect(cam, SIGNAL(sendShot(QImage, QRect)), SLOT( onSendShot(QImage, QRect)) );
        
        QHBoxLayout *bottomLeftLayout = new QHBoxLayout();
        leftLayout->addLayout( bottomLeftLayout );

            imagesScrollArea = new QScrollArea( this );
            bottomLeftLayout->addWidget( imagesScrollArea );

            imagesScrollArea->setWidgetResizable( true );
            imagesScrollArea->setObjectName( "imagesScrollArea" );
            imagesScrollArea->setWidget( showCase = new ShowCase( this ) );

            connect( showCase, SIGNAL(imageSelected( int )), SLOT(onImageSelected(int)) );
            connect( showCase, SIGNAL(buttonSelected(QAbstractButton*)), SLOT(onButtonSelected(QAbstractButton*)) );

/*
            bottomLeftLayout->addWidget( counterLbl = new QLabel("0") );
            counterLbl->setObjectName( "counterLbl" );
            */

            bottomLeftLayout->addWidget( takePic = new QPushButton() );
            takePic->setObjectName( "takePic" );
            connect( takePic, SIGNAL(clicked()), SLOT(onTakePic()) );

	        bottomLeftLayout->addWidget( delPic = new QPushButton() );
            delPic->setObjectName( "delPic" );
            connect( delPic, SIGNAL(clicked()), SLOT(onDelPic()) );

		leftLayout->addStretch(0);

	// COL 02
	layout->addWidget( sidePanel = new SidePanel(this, "add") );
    connect( sidePanel, SIGNAL(confirmChanges(QJsonObject)), SLOT(addPerson(QJsonObject)) );


    connect( this, SIGNAL(filesLoaded(QStringList)), SLOT(onFilesLoaded(QStringList)) );
}


void
AddView::onNewPersonClicked(){
	showCase->clear();
	sidePanel->clear();
    m_savedFaces.clear();
    sidePanel->faceLbl->setText( QString::number( m_savedFaces.size() ) );

}

void
AddView::onSendFace( QImage faceImg ){

	removeSpacers( showCase->layout );
	showCase->addImage(faceImg, m_savedFaces.size() );
	showCase->layout->addStretch(0); // compacte images
    m_savedFaces << faceImg;

    imagesScrollArea->ensureWidgetVisible( showCase->at( m_savedFaces.size() ) );

    sidePanel->faceLbl->setText( QString::number( m_savedFaces.size() ) );

}
void
AddView::onFilesLoaded( QStringList files ){


    foreach (QString imageFile, files){
        FaceDetector *detector = new FaceDetector( 0, QImage2Mat( QImage( imageFile )) ); // index does not matter in this case
        detector->start();
        connect( detector, SIGNAL(sendFace( QImage )), SLOT( onSendFace( QImage ) ) );
 	}
}


void
AddView::onStopCamera(){
	theater->clearFaces();
	theater->clearImage();
	theater->imageLbl->setText( "Déposez vos images ICI<br><small>Elles seront automatiquement analysées</small>" );
}

void
AddView::onChooseMedia(int buttonId){
	if (buttonId == 1){ // images
		stopWork();
		theater->imageLbl->setProperty("type", "drop");
	    theater->imageLbl->setStyle( QApplication::style() );
	    theater->imageOverlay->hide();
       
	} else {
		theater->imageLbl->setText( "" );
		theater->imageOverlay->show();
        if ( !cam->working )
        	startWork();
        
        theater->imageLbl->setProperty("type", "camera");
        theater->imageLbl->setStyle( QApplication::style() );
	}
}


void
AddView::addPerson(QJsonObject formData){
    if( m_savedFaces.size() >= 1 ){
        Socket::sock().addPerson(formData, m_savedFaces);
    };
}

void
AddView::onSendShot(QImage face, QRect rect){
	m_currentFace = face;

    theater->addRect( rect, m_savedFaces.size() );
}


void
AddView::onTakePic(){
    if (m_currentFace.height() == 0 || m_currentFace.width() == 0 ){
        return;
    }
    removeSpacers( showCase->layout );
	showCase->addImage(m_currentFace, m_savedFaces.size() );
	showCase->layout->addStretch(0); // compacte images
    m_savedFaces << m_currentFace;

	imagesScrollArea->ensureWidgetVisible( showCase->at( m_savedFaces.size() ) );
    sidePanel->faceLbl->setText( QString::number( m_savedFaces.size() ) );
}

void 
AddView::onDelPic(){
	qDebug() << "m_currentSelected: " << m_currentSelected;

	if (m_currentSelected > -1){
		showCase->deleteButton( m_currentButton );
        m_savedFaces.removeAt( m_currentSelected );
		sidePanel->faceLbl->setText( QString::number( m_savedFaces.size() ) );
	 }

	 if (m_savedFaces.size() > 0){
	 	showCase->at(0)->animateClick();
	 }
}

void
AddView::onRemovePic(int index){
	
}

void
AddView::onSendFrame(QImage frame){
    if ( cam->working ){
		theater->imageOverlay->hide();
	    theater->setImage( frame );
	    theater->clearFaces();
	 }
}


void
AddView::startCamera(){
	 emit startWork();
/*	cam->openCamera();
    mTimer->start();*/
}

void
AddView::stopCamera(){
	theater->imageOverlay->show();
	emit stopWork();
}

void
AddView::onButtonSelected(QAbstractButton* b){
	m_currentButton = b;
}

void
AddView::onImageSelected(int index){
	m_currentSelected = index;
}
