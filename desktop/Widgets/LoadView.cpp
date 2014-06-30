#include "LoadView.h"

LoadView::LoadView(QWidget* parent):DropArea( parent )
{

    QVBoxLayout *layout = new QVBoxLayout( this );

    layout->addWidget( tempAccountLbl = new QLabel(
        "<center>Ceci est un compte administrateur temporaire<br>"
        "Veuillez<ol><li>Enregistrer vos coordonnées dans le système en cliquant sur <strong>\"Ajouter\"</strong></li>"
        "<li>Vous rendre dans <strong>\"Réglages\"</strong> afin de créer votre compte administrateur</li></ol></center>"
        ));
    tempAccountLbl->setObjectName( "tempAccountLbl" );
    tempAccountLbl->hide();
    connect( &Socket::sock(), SIGNAL(loggedAsTempAdmin()), SLOT(onLoggedAsTempAdmin()) );
    connect( &Socket::sock(), SIGNAL(loggedAsUser()), SLOT(onLogged()) );
    connect( &Socket::sock(), SIGNAL(loggedAsAdmin()), SLOT(onLogged()) );

    QLabel *loadImageLbl = new QLabel( "Pour commencer" );
    loadImageLbl->setObjectName("loadImageLbl");
    layout->addWidget(loadImageLbl);

    layout->addStretch(0);

    QHBoxLayout *hLayout;
    layout->addLayout( hLayout = new QHBoxLayout() );

        QFrame *dropZone = new QFrame( this );
        dropZone->setObjectName( "dropZone" );
        hLayout->addWidget( dropZone, Qt::AlignCenter );

        QVBoxLayout *dropZoneLayout = new QVBoxLayout( dropZone );
        QLabel *deposezFichierLbl = new QLabel( dropZone );
        deposezFichierLbl->setObjectName( "deposezFichierLbl" );
        deposezFichierLbl->setText( 
        "<center>"
        "Chargez une ou plusieurs photos depuis votre ordinateur"
        "</center>"
        );
        dropZoneLayout->addWidget( deposezFichierLbl, 1, Qt::AlignCenter );

        QPushButton *browseImagesBtn = new QPushButton( dropZone );
        browseImagesBtn->setText( "Séléctionnez des Images" );
        browseImagesBtn->setObjectName( "browseImagesBtn" );
        dropZoneLayout->addWidget( browseImagesBtn, 1, Qt::AlignCenter );

        connect( browseImagesBtn, SIGNAL( clicked() ), SLOT( onBrowseImages() ) );
    
    layout->addStretch(0);

}

void
LoadView::onLoggedAsTempAdmin(){
    qDebug() << "temp admin";
    tempAccountLbl->show();
}

void
LoadView::onLogged(){
    tempAccountLbl->hide();
}
