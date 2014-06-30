#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include "../core.h" 
#include <QFrame>

class AboutView : public QFrame
{
    Q_OBJECT
public:
    explicit AboutView(QWidget *parent = 0);

private:
	QLabel *pageContent;
signals:

public slots:

};

#endif // ABOUTVIEW_H
