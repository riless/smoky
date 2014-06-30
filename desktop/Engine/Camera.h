#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "../core.h"
#include <QThread>
#include <QTimer>

class Camera : public QObject
{
    Q_OBJECT
public:
    ~Camera(){
        closeCamera();
        qDebug() << "~camera destructed !";
    }

    explicit Camera(QObject *parent = 0): QObject(parent)
    {
       working = false;
       _cap = new VideoCapture();
       
    }
    bool working;
public slots:

    void
    openCamera(){
        qDebug() << "OpenCamera " << _cap;
        working = true;
        _cap->open(0);
         // qDebug() << "try to open camera" << _cap;

        if(!_cap->isOpened()) {
            qDebug() << "Cannot open camera !" ;
            exit(0);
        }

        // qDebug() << "Camera Opened :)";

        // load face information for Viola and Jones algorithme
        // CascadeClassifier cascade;

        if (!_cascade.load("res/haarcascade_frontalface_alt.xml") ) {
            qDebug() << "Erreur lors du chargement du haar cascade frontalface...";
            exit(0);
        }

        // queryFrame();

    }

    void
    closeCamera(){
        emit stopCamera();
        qDebug() << "closeCamera " << _cap;
        if( _cap->isOpened()  ) {
            working = false;
            // delete _cap;
            _cap->release();
            qDebug() << "Camera released :)";
        }
        // qDebug() << "SIGNAL: stopCamera sent";
        

    }

    void
    queryFrame(){
        // static int n = 0;
        // qDebug() << "querying frame" << n++;
        if ( _cap->isOpened() && working ){
            
            Mat frame;
            _cap->read( frame ); // get a new frame from camera

            if(frame.cols ==0 || frame.rows==0){
                qDebug() << "Invalid frame: skipped !";
                return;
            }

            emit sendFrame( Mat2QImage(frame) );
            // qDebug() << "Frame" << n++ << "sent !";

            Mat gray;
            cvtColor(frame, gray, CV_BGR2GRAY);
            equalizeHist(gray, gray);

            vector<Rect> faces;
            _cascade.detectMultiScale(gray, faces, 1.2, 3, 0, cvSize(96,96));

            if ( !faces.size() ){
                // qDebug() << "No face in this frame";
                return;
            }


            Mat cropped;
            for (unsigned int i = 0; i < faces.size(); ++i){
                Rect r = faces[i];

                getRectSubPix(frame, Size(r.width, r.height), Point2f(r.x + r.width/2, r.y + r.height / 2), cropped);
                cv::resize( cropped, cropped, Size(96,96) );
                emit sendShot( Mat2QImage( cropped ), QRect(r.x, r.y, r.width, r.height) );
                // qDebug() << "face sent !";
            }

        } else {
            qDebug() << "cam not opened !";
        }

    }


private:
    CascadeClassifier _cascade;
    VideoCapture *_cap;
    QTimer *mTimer;

signals:
    void sendShot( QImage, QRect );
	void sendFrame( QImage );
    void stopCamera();

};

#endif // CAMERA_H
