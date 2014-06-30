#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "../core.h"

class SideBar: public QFrame
{
    Q_OBJECT
public:
    explicit SideBar(QWidget* parent=0);
    QString currentCategory() const;

private:
    struct
        {
            class QAbstractButton* homeBtn;
            class QAbstractButton* addBtn;
            class QAbstractButton* searchBtn;
            class QAbstractButton* historyBtn;
            class QAbstractButton* statsBtn;
            class QAbstractButton* settingsBtn;
            class QAbstractButton* aboutBtn;
    } ui;

class QButtonGroup* _buttonGroup;
class QAbstractButton* _currentButton;

signals:
    void currentChanged( int index );


public slots:
    void onButtonClicked(QAbstractButton* button);
    void clickOnHomeBtn();
    void onLoggedAsUser();
    void onLoggedAsAdmin();


};



#endif // SIDEBAR_H
