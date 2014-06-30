#ifndef HEADER_H
#define HEADER_H

#include "../core.h"

class Header: public QFrame
{
    Q_OBJECT
public:
    Header(QWidget *parent = 0);

private:
	QLabel *logo;
	QLabel *salutationLbl;
	QPushButton *disconnectBtn;
public slots:
	void onUserConnected(QString firstName);
	void onUserDisconnected();
	void onDisconnectClicked();
    void onRefreshClicked();

signals:
	void disconnectClicked();
};

#endif // HEADER_H
