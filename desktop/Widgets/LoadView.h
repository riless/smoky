#ifndef LOADVIEW_H
#define LOADVIEW_H

#include "../core.h"
#include "DropArea.h"
#include "../Engine/Socket.h"

class LoadView:public DropArea
{
    Q_OBJECT
public:
    LoadView(QWidget *parent = 0);

private:
	QLabel *tempAccountLbl;

public slots:
	void onLoggedAsTempAdmin();
	void onLogged();
};

#endif // LOADVIEW_H
