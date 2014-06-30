#include "StackedWidget.h"

#include <QDebug>

StackedWidget::StackedWidget(QWidget* parent): QStackedWidget(parent)
{
    this->addWidget( homePage = new HomePage( this ) );
    this->addWidget( addView = new AddView( this ) );
    this->addWidget( searchPage = new SearchPage( this ) );
    this->addWidget( statsView = new StatsView( this ) );
    this->addWidget( historyView = new HistoryView( this ) );
    this->addWidget( settingsView = new SettingsView( this ) );
    this->addWidget( aboutView = new AboutView( this ) );
}

void
StackedWidget::showLoadView(){
    homePage->setCurrentView(0);
}
