#include "Header.h"
#include <QDebug>
#include "../Engine/Socket.h"

Header::Header( QWidget *parent):
    QFrame( parent )
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setContentsMargins(0,0,0,0);

    layout->addWidget( logo = new QLabel( this ) );
    logo->setObjectName("logo");

    layout->addStretch(0);
/*
    QPushButton *refreshBtn = new QPushButton( "Refresh" );
    refreshBtn->setObjectName( "refreshBtn" );
    layout->addWidget( refreshBtn );
    connect(refreshBtn, SIGNAL(clicked()), SLOT( onRefreshClicked() ) );
*/

    layout->addWidget( salutationLbl = new QLabel());
    salutationLbl->setObjectName("salutationLbl");

    layout->addWidget( disconnectBtn = new QPushButton("Déconnexion") );
    disconnectBtn->setObjectName("disconnectBtn");
    connect( disconnectBtn, SIGNAL(clicked()), SLOT(onDisconnectClicked()) );

    salutationLbl->hide();
    disconnectBtn->hide();
}

void
Header::onUserConnected(QString firstName){
    salutationLbl->setText( QString("Salut <span>%1</span>").arg( firstName ) );
    salutationLbl->show();
    disconnectBtn->show();
}

void
Header::onUserDisconnected(){
    salutationLbl->hide();
    disconnectBtn->hide();
}

void
Header::onDisconnectClicked(){
    emit disconnectClicked();
}

void
Header::onRefreshClicked(){
    loadCSS();
}
