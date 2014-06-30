#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QDragEnterEvent>
#include <QRegExp>

#include "Widgets/SideBar.h"
#include "Widgets/Header.h"
#include "Widgets/StackedWidget.h"

#include <QNetworkAccessManager>
#include "inc/qlightboxwidget/qlightboxwidget.h"
#include "Engine/Socket.h"
#include "Engine/Camera.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct{
        class SideBar* sideBar;
        class StackedWidget* stackedWidget;
        class Header* header;
    } ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
	QLightBoxWidget* lightBox;

	QLabel *loginAvatarLbl;
	QLineEdit *loginUserEdt;
	QLineEdit *loginPasswordEdt;
	QLabel *loginIndication;
	QPushButton *loginSubmitBtn;
    QLabel *statLbl;
    QCheckBox *localhostCbx;

    QLineEdit *serverIpEdt;
    QSettings *settings;


    Camera *cam;
    QTimer *mTimer;
    QThread *t;
	bool checkFields( bool setWrong = false );



public slots:
    bool ipCheck( QString ip );
    void onInvalidCred();
    void loadViewData( int index );
    void threadStoped();
    void connectUser();
    void resetThings();
    void onShotSent(QImage face);
    void startCamera();
    void stopCamera();
    void onConnexionError(int);
    void onLocalhostCbx(int stat);
signals:
    void startWork();
    void stopWork();
};

#endif // MAINWINDOW_H
