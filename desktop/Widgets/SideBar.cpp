#include "../core.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../Engine/Socket.h"
#include <QDebug>
#include "SideBar.h"


QAbstractButton* newButton(const QString& text, QButtonGroup* buttonGroup, QWidget* parent = 0){
    QAbstractButton* pushButton = new QPushButton(parent);
    pushButton->setText(text);
    pushButton->setCheckable(true);
    pushButton->setAutoExclusive( true );
    buttonGroup->addButton(pushButton);
    return pushButton;
}

void
SideBar::clickOnHomeBtn(){
    ui.homeBtn->click();
}


SideBar::SideBar(QWidget *parent)
    :QFrame(parent), _currentButton(0)
{
    // SIDEBAR

    QVBoxLayout* leftLayout = new QVBoxLayout( this );

    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(0);

    _buttonGroup = new QButtonGroup( this );
    _buttonGroup->setExclusive( true );

    connect(_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(onButtonClicked(QAbstractButton*)));

    leftLayout->addWidget( ui.homeBtn = newButton( tr( "Accueil" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.homeBtn->setObjectName( "homeBtn" );

    leftLayout->addWidget( ui.addBtn = newButton( tr( "Ajouter" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.addBtn->setObjectName( "addBtn" );

    leftLayout->addWidget( ui.searchBtn = newButton( tr( "Rechercher" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.searchBtn->setObjectName( "searchBtn" );

    leftLayout->addWidget( ui.statsBtn = newButton( tr( "Statistique" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.statsBtn->setObjectName( "statsBtn" );
    ui.statsBtn->hide();

    leftLayout->addWidget( ui.historyBtn = newButton( tr( "Historique" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.historyBtn->setObjectName( "historyBtn" );
    ui.historyBtn->hide();

    leftLayout->addWidget( ui.settingsBtn = newButton( tr( "Paramètres" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.settingsBtn->setObjectName( "settingsBtn" );
    ui.settingsBtn->hide();

    leftLayout->addWidget( ui.aboutBtn = newButton( tr( "A Propos" ), _buttonGroup, this ), Qt::AlignHCenter );
    ui.aboutBtn->setObjectName( "aboutBtn" );

    leftLayout->addStretch(0);

    ui.homeBtn->click();

    connect( &Socket::sock(), SIGNAL(loggedAsAdmin()), SLOT(onLoggedAsAdmin()) );
    connect( &Socket::sock(), SIGNAL(loggedAsUser()), SLOT(onLoggedAsUser()) );

    // END SIDEBAR
}

void
SideBar::onLoggedAsUser(){
    ui.statsBtn->hide();
    ui.historyBtn->hide();
    ui.settingsBtn->hide();
}

void
SideBar::onLoggedAsAdmin(){
    ui.statsBtn->show();
    ui.historyBtn->show();
    ui.settingsBtn->show();
}

QString
SideBar::currentCategory() const
{
    if ( _buttonGroup->checkedButton() == ui.homeBtn )             return HOME_CATEGORY;
    else if ( _buttonGroup->checkedButton() == ui.searchBtn )      return SEARCH_CATEGORY;
    else if ( _buttonGroup->checkedButton() == ui.statsBtn )       return STATS_CATEGORY;
    else if ( _buttonGroup->checkedButton() == ui.historyBtn )     return HISTORY_CATEGORY;
    else if ( _buttonGroup->checkedButton() == ui.settingsBtn )    return SETTINGS_CATEGORY;
    else if ( _buttonGroup->checkedButton() == ui.aboutBtn )        return HELP_CATEGORY;
    return "Unknown";
}


void SideBar::onButtonClicked(QAbstractButton* button){

    int index = layout()->indexOf( button );

    if (button != _currentButton){
        // qDebug() << index << currentCategory();
    }


    button = _currentButton;

    emit currentChanged( index );

}
