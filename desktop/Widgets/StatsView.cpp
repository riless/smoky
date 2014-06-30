#include "StatsView.h"

StatsView::StatsView(QWidget *parent): QFrame( parent )
{
    layout = new QVBoxLayout( this );


	connect( &Socket::sock(), SIGNAL(statsLoaded(QJsonObject)), SLOT(setData(QJsonObject)) );
}

void
StatsView::setData(QJsonObject data){


	QLayoutItem* item;
    while ( ( item = layout->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

	customPlot = new QCustomPlot();
    layout->addWidget( customPlot );
   
    /*QBrush b;
    b.setColor( QColor(18,19,20,50) );
    customPlot->setBackground( b );
*/
    // create empty bar chart objects:
    recFaces = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    unrecFaces = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    ignFaces = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(recFaces);
    customPlot->addPlottable(unrecFaces);
    customPlot->addPlottable(ignFaces);
    // set names and colors:

	qDebug() << "setData fired!";
    // "EigenFace" << "FisherFace" << "LBPH"
    //  rec
    //  unrec
    //  ign


    QPen pen;
    pen.setWidthF(1.2);

    ignFaces->setName("Visages ignorés");
    pen.setColor(QColor(201, 201, 201));
    ignFaces->setPen(pen);
    ignFaces->setBrush(QColor(201, 201, 201, 50));

    unrecFaces->setName("Visages non reconnus");
    pen.setColor(QColor(255, 131, 0));
    unrecFaces->setPen(pen);
    unrecFaces->setBrush(QColor(255, 131, 0, 50));

    recFaces->setName("Visages reconnus");
    pen.setColor(QColor(150, 222, 0));
    recFaces->setPen(pen);
    recFaces->setBrush(QColor(150, 222, 0, 70));

    // stack bars ontop of each other:
    /*  unrecFaces->moveAbove(ignFaces);
    recFaces->moveAbove(unrecFaces);*/

    // prepare x axis with recognition systems labels:
    QVector<QString> labels;

    ticks << 1 << 2 << 3;
    labels << "EigenFace" << "FisherFace" << "LBPH";
    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    // customPlot->xAxis->setTickLabelRotation(0);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 4);
    // customPlot->xAxis->setGrid(false);
    customPlot->xAxis->setRange(0, 5);

    // prepare y axis:
    customPlot->yAxis->setRange(0, 100);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("\% des visages reconnus/non reconnus/ignorés");
    // customPlot->yAxis->setSubGrid(true);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(46, 47, 48, 20));
    //customPlot->yAxis->setGridPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    //customPlot->yAxis->setSubGridPen(gridPen);

    // Add data:

    recData.clear();
    unrecData.clear();
    ignData.clear();

    int total_eigen  = data["eigenFace_rec"].toString().toInt()  + data["eigenFace_unrec"].toString().toInt() + data["eigenFace_ign"].toString().toInt();
    int total_fisher = data["fisherFace_rec"].toString().toInt()  + data["fisherFace_unrec"].toString().toInt() + data["fisherFace_ign"].toString().toInt();
    int total_LBPH   = data["LBPHFace_rec"].toString().toInt()  + data["LBPHFace_unrec"].toString().toInt() + data["LBPHFace_ign"].toString().toInt();

    if (total_eigen == 0) total_eigen = 1;
    if (total_fisher == 0) total_fisher = 1;
    if (total_LBPH == 0) total_LBPH = 1;


    recData   << data["eigenFace_rec"].toString().toInt() * 100 / total_eigen <<  data["fisherFace_rec"].toString().toInt()	* 100 / total_fisher << data["LBPHFace_rec"].toString().toInt() * 100 / total_LBPH;
    unrecData << data["eigenFace_unrec"].toString().toInt() * 100 / total_eigen << data["fisherFace_unrec"].toString().toInt() * 100 / total_fisher << data["LBPHFace_unrec"].toString().toInt() * 100 / total_LBPH;
    ignData   << data["eigenFace_ign"].toString().toInt() * 100 / total_eigen << data["fisherFace_ign"].toString().toInt() * 100  / total_fisher << data["LBPHFace_ign"].toString().toInt()  * 100 / total_LBPH;


    recFaces->setData(ticks, recData);
    unrecFaces->setData(ticks, unrecData);
    ignFaces->setData(ticks, ignData);


    // setup legend:
    customPlot->legend->setVisible(true);
    // customPlot->legend->setPositionStyle(QCPLegend::psTop);
    customPlot->legend->setBrush(QColor(255, 255, 255, 255));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    customPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(12);
    customPlot->legend->setFont(legendFont);

    // customPlot->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    // customPlot->setRangeZoom(Qt::Horizontal|Qt::Vertical);

    // stack bars ontop of each other:

    customPlot->replot();
}

