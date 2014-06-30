#ifndef STATSVIEW_H
#define STATSVIEW_H

#include "../core.h"
#include <QJsonObject>
#include "../inc/qcustomplot/qcustomplot.h"
#include "../Engine/Socket.h"
#include <QBrush>

class StatsView: public QFrame
{
    Q_OBJECT
public:
    StatsView(QWidget *parent = 0);

private:
	QVBoxLayout* layout;
	QVector<double> recData, unrecData, ignData;
	QCustomPlot *customPlot;
	QVector<double> ticks;
	QCPBars *recFaces;
	QCPBars *unrecFaces;
	QCPBars *ignFaces;

public slots:
    void setData(QJsonObject data);
};

#endif // STATSVIEW_H
