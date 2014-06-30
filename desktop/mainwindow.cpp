#include "mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include "inc/qlightboxwidget/qlightboxwidget.h"
#include <QTimer>
#include <QThread>
#include "Widgets/HomePage.h"



MainWindow::~MainWindow()
{
    t->quit();
    t->wait();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setCentralWidget(new QWidget);
    QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    layout->addWidget( ui.header = new Header( this ) );
    connect( &Socket::sock(), SIGNAL(userLoggedIn(QString)), ui.header, SLOT(onUserConnected(QString)) );
    connect( &Socket::sock(), SIGNAL(userLoggedOut()), ui.header, SLOT(onUserDisconnected()) );
    connect( ui.header, SIGNAL(disconnectClicked()), &Socket::sock(), SLOT(logout()) );
    // mettre obligatoirement ici pour corriger un petit bug d'affichage 
    
//    /*temp refresh button*/
//    QPushButton *refreshBtn = new QPushButton( "Refresh" );
//    refreshBtn->setObjectName( "refreshBtn" );
//    mainLayout->addWidget( refreshBtn );
//    connect(refreshBtn, SIGNAL(clicked()), ui.header, SLOT( onRefreshClicked() ) );

    QHBoxLayout* centralLayout = new QHBoxLayout( );
    centralLayout->addWidget( ui.sideBar = new SideBar( this ) );
    centralLayout->addWidget( ui.stackedWidget = new StackedWidget( this ) );

    layout->addLayout( centralLayout );

    connect(ui.sideBar, SIGNAL( currentChanged( int ) ), ui.stackedWidget, SLOT( setCurrentIndex( int ) ) );
    connect(ui.sideBar, SIGNAL( currentChanged( int ) ), SLOT( loadViewData( int ) ) );


    // LOG
    connect( &Socket::sock(), SIGNAL(clientEvent(int)), &Socket::sock(), SLOT(log(int)) );


    // LIGHT BOX
    lightBox = new QLightBoxWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout( lightBox );

    QWidget *fm = new QWidget();
    fm->setObjectName("connexionFrame");
    mainLayout->addWidget( fm );

    QGridLayout *lightBoxLayout = new QGridLayout( fm );
    lightBoxLayout->setRowStretch(1, 1);

    settings = new QSettings("smoky.ini",QSettings::IniFormat);


    lightBoxLayout->addWidget( new QLabel( "IP du serveur" ), 0, 0);
    lightBoxLayout->addWidget( serverIpEdt = new QLineEdit(), 0, 1 );
    serverIpEdt->setInputMask( "000.000.000.000; " );
    serverIpEdt->setText( settings->value("serverIp").toString() );
    serverIpEdt->setObjectName( "serverIpEdt" );

    lightBoxLayout->addWidget( localhostCbx = new QCheckBox("localhost"), 1,1);
    localhostCbx->setObjectName( "localhostCbx" );
    connect( localhostCbx, SIGNAL( stateChanged(int)), SLOT(onLocalhostCbx(int)) );

    lightBoxLayout->addWidget( loginIndication = new QLabel(), 1, 0);
    loginIndication->setObjectName( "loginIndication" );
    loginIndication->setText("<center><small>Placez votre visage<br>face à la caméra</small></center>");

    lightBoxLayout->addWidget( loginAvatarLbl = new QLabel(""), 2, 0, 2, 1);
    loginAvatarLbl->setObjectName( "loginAvatarLbl" );

    lightBoxLayout->addWidget( loginUserEdt = new QLineEdit(), 2, 1);
    loginUserEdt->setPlaceholderText("Nom d'utilisateur");
    loginUserEdt->setObjectName( "loginUserEdt" );

    lightBoxLayout->addWidget( loginPasswordEdt = new QLineEdit(), 3, 1);
    loginPasswordEdt->setObjectName( "loginPasswordEdt" );
    loginPasswordEdt->setEchoMode( QLineEdit::Password );
    loginPasswordEdt->setPlaceholderText("Mot de passe");


    lightBoxLayout->addWidget( loginSubmitBtn = new QPushButton("Connexion"), 4, 1);
    loginSubmitBtn->setObjectName( "loginSubmitBtn" );

    lightBoxLayout->addWidget( statLbl = new QLabel( "" ), 5,0, 1, 2 );
    statLbl->setObjectName( "statLbl" );

    lightBox->show();



    Camera *cam = new Camera();
    t = new QThread();

    mTimer = new QTimer();
    mTimer->setInterval(1000/24);
    connect(mTimer, SIGNAL(timeout()), cam, SLOT(queryFrame()) );
    connect( this, SIGNAL(startWork()), cam, SLOT(openCamera()) );
    connect( this, SIGNAL(startWork()), mTimer, SLOT(start()) );

    connect( cam, SIGNAL(stopCamera()), mTimer, SLOT(stop()) );
    connect( this, SIGNAL(stopWork()), cam, SLOT(closeCamera()) );

    /*destruction*/
    // connect( qApp, SIGNAL(aboutToQuit()),mTimer, SLOT(stop()) );
    // connect( qApp, SIGNAL(aboutToQuit()),mTimer, SLOT(deleteLater()) );

    // connect( qApp, SIGNAL(aboutToQuit()), cam, SLOT(closeCamera()) );
    // connect( qApp, SIGNAL(aboutToQuit()), cam, SLOT(deleteLater()) );

    connect(t, SIGNAL(finished()), cam, SLOT(deleteLater()));
    // connect(this, &Controller::operate, worker, &Worker::doWork);

    //connect( qApp, SIGNAL(aboutToQuit()), t, SLOT(quit()) );
    // connect( t, SIGNAL(finished()), t, SLOT(deleteLater()) );

    // user logged out
    connect( &Socket::sock(), SIGNAL(userLoggedOut()), SLOT(startCamera()) );

    // user logged in
    connect( &Socket::sock(), SIGNAL(userLoggedIn(QString)), SLOT(stopCamera()) );

    cam->moveToThread( t );
    mTimer->moveToThread( t );
    t->start( QThread::IdlePriority );

    emit startWork();

    connect(cam, SIGNAL(sendShot(QImage, QRect)), SLOT(onShotSent(QImage)) ); // receive shots

    connect( &Socket::sock(), SIGNAL(userLoggedIn(QString)), loginPasswordEdt, SLOT(clear()) );
    connect( &Socket::sock(), SIGNAL(userLoggedIn(QString)), lightBox, SLOT(hide()) );

    // rendre la page d'accueil
    connect( &Socket::sock(), SIGNAL(userLoggedOut()), SLOT(resetThings()) );

    connect( &Socket::sock(), SIGNAL(invalidCred()), SLOT(onInvalidCred()) );

    connect( loginSubmitBtn, SIGNAL(clicked()), SLOT(connectUser()) );


    connect( &Socket::sock(), SIGNAL(connexionError(int)), SLOT(onConnexionError(int)) );

    // move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
/*
    QDesktopWidget desktop;
    QRect desktop_geometry = desktop.screenGeometry();
*/

    // resize( 780, 650 );

    /*int x = desktop_geometry.width()/2 - width()/2;
    int y = desktop_geometry.height()/2 - height()/2;
    move( x, y );*/

    // showMaximized();
}


void MainWindow::onLocalhostCbx(int stat){
    if (stat == Qt::Checked){
        serverIpEdt->setText("127.0.0.1");
        serverIpEdt->setDisabled( true );
    } else {
        /* serverIpEdt->setText(""); */
        serverIpEdt->setDisabled( false );
    }
}
void
MainWindow::onConnexionError(int error){
    QString error_msg = "";
    switch(error){
        case 1: error_msg = "Le délai d'attente est dépassé"; break;
        case 2: error_msg = "La connexion avec le serveur a été fermé"; break;
        case 3: error_msg = "La connexion avec le serveur a été refusé"; break;
        default: error_msg = "Erreur de connexion au serveur"; break;
    }

    statLbl->setText( error_msg );
}

void
MainWindow::startCamera(){


    emit startWork();

}

void
MainWindow::stopCamera(){
    emit stopWork();
}


void
MainWindow::threadStoped(){
    qDebug() << "thread STOPPED";
}

void
MainWindow::onShotSent(QImage face){
    loginAvatarLbl->setPixmap( QPixmap::fromImage( face ) );
}

void
MainWindow::resetThings(){
    qDebug() << "resetThings";
    //clear internal data
    ui.stackedWidget->homePage->processImage->showCase->clear();
    ui.stackedWidget->homePage->processImage->clear();

    // both
    ui.stackedWidget->homePage->processImage->theater->clearFaces();    // les cadres des visages & données
    ui.stackedWidget->homePage->processImage->detectedFaces->clear();   // les visages detecté & données

    // clear widgets
    ui.stackedWidget->homePage->processImage->theater->clearImage();    // l'image chagré dans theater
    ui.stackedWidget->homePage->processImage->sidePanel->clear();       // les info du sidepanel

    ui.stackedWidget->showLoadView();
    ui.sideBar->clickOnHomeBtn();
    loginAvatarLbl->setPixmap( QPixmap() );

    lightBox->show();


}

void
MainWindow::loadViewData( int index ){
    // qDebug() << "sidebar index :" << index;
    ui.stackedWidget->addView->stopCamera();
    switch (index){
        case 1: ui.stackedWidget->addView->chooseCam->animateClick(); // select
        case 3: Socket::sock().getStats();
        case 4: Socket::sock().getHistory();
        case 5: Socket::sock().getParameters();
    }
}


void
MainWindow::connectUser(){
    QString serverIp = "";
    if ( ipCheck(serverIpEdt->text()) ){
        statLbl->setText("");

        serverIp = serverIpEdt->text();
        settings->setValue("serverIp", serverIp );

        const QPixmap *avatar = loginAvatarLbl->pixmap();
        QImage resizedAvatar;
        if (avatar != NULL){
            resizedAvatar = avatar->scaled(WORK_SIZE, WORK_SIZE).toImage();
        }

        if ( checkFields() ){
            Socket::sock().login(
                serverIp,
                loginUserEdt->text(),
                loginPasswordEdt->text(),
                QImage2Mat( resizedAvatar )
            );
        }
    } else {
        statLbl->setText("Adresse IP incorrecte");
    }
}

bool
MainWindow::ipCheck( QString ip ){
    QRegExp rx("^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$");
    return rx.exactMatch( ip );        // returns a bool
}


void
MainWindow::onInvalidCred(){
    checkFields(true);
}

bool
MainWindow::checkFields( bool setWrong ){
    
    bool ok = true;

    if ( setWrong ){
        loginUserEdt->setProperty("login", "invalid");
        loginPasswordEdt->setProperty("login", "invalid");
        loginAvatarLbl->setProperty("login", "invalid");
        ok = false;
    } else {
        loginUserEdt->setProperty("login", "");
        loginPasswordEdt->setProperty("login", "");
        loginAvatarLbl->setProperty("login", "");

        if (loginUserEdt->text() == "") {
            loginUserEdt->setProperty("login", "invalid");
            ok = false;
        }
        
        if (loginPasswordEdt->text() == "") {
            loginPasswordEdt->setProperty("login", "invalid");
            ok = false;
        }

        if (loginAvatarLbl->pixmap() != NULL ) {
            ok = true;
        }
    }

    loginUserEdt->setStyle( QApplication::style() ); // refresh style
    loginPasswordEdt->setStyle( QApplication::style() );
    loginAvatarLbl->setStyle( QApplication::style() );
    return ok;
}

