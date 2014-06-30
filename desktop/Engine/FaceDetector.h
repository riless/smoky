#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H
#include <QRunnable>
#include "../core.h"
#include <QThread>

class FaceDetector: public QThread
{
	Q_OBJECT
public:
	FaceDetector();
    FaceDetector(int index, cv::Mat src, QObject* parent = 0);
    QList<struct DetectorData> getData();
    void run();


private:
	VideoCapture _cap;
    cv::Mat _image;
    int _index;
    QList<struct DetectorData> detectorData;
    double _ratio;
public slots:

signals:
    void sendFace( QImage );
	void detectionFinished(int index, QList<struct DetectorData> );
};

#endif // FACEDETECTOR_H
