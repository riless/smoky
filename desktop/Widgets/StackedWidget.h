#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include "../core.h"

#include "HomePage.h"
#include "SearchPage.h"
#include "StatsView.h"
#include "HistoryView.h"
#include "SettingsView.h"
#include "AboutView.h"
#include "AddView.h"

class StackedWidget: public QStackedWidget
{
    Q_OBJECT


    
public:
    StackedWidget(QWidget *parent = 0);

    HomePage *homePage;
    AddView *addView;
    SearchPage *searchPage;
    StatsView *statsView;
    HistoryView *historyView;
    SettingsView *settingsView;
    AboutView * aboutView;

public slots:
	void showLoadView();
};

#endif // STACKEDWIDGET_H
