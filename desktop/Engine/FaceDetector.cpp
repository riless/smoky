#include "FaceDetector.h"

FaceDetector::FaceDetector(int index, Mat originalImage, QObject* parent): QThread( parent )
{
    _image = originalImage;
    _index = index;
}

void
FaceDetector::run(){

    CascadeClassifier cascade;
    if (!cascade.load("res/haarcascade_frontalface_alt.xml") ) {
        qDebug() << "Erreur lors du chargement du haar cascade frontalface...";
        exit(0);
    }

    QSize processSize = keepAspectRatio(_image.cols, _image.rows, MAX_PROCESS_WIDTH, MAX_PROCESS_HEIGHT);
    Mat color;
    if ( _image.cols > processSize.width() ){
        cv::resize( _image, color, cv::Size(processSize.width(), processSize.height() ) );
    } else {
        color = _image.clone();
    }

    QSize displaySize = keepAspectRatio(color.cols, color.rows);
    _ratio = (double) color.cols/displaySize.width();

    Mat gray;
    cvtColor(color, gray, CV_BGR2GRAY);


    //PRETRAITEMENT
    equalizeHist(gray, gray);

    vector<Rect> facesRects;
    cascade.detectMultiScale(gray, facesRects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cvSize(16,16));

    // qDebug() << "detected faces size: " << facesRects.size();
    
    for (unsigned int i = 0; i < facesRects.size(); ++i){

        Rect r = facesRects[i];
        Mat croppedFace;
        getRectSubPix(color, Size(r.width, r.height), Point2f(r.x + r.width/2, r.y + r.height / 2), croppedFace);
        // qImageFaces << Mat2QImage(croppedFace);

        struct DetectorData tmpData;
        tmpData.id = i;
        cv::resize( croppedFace, croppedFace, Size(WORK_SIZE,WORK_SIZE) );
        tmpData.image = Mat2QImage(croppedFace);
        emit sendFace( tmpData.image );
        tmpData.rect = QRect(r.x/_ratio+1, r.y/_ratio+1, r.width/_ratio+1, r.height/_ratio+1);
        tmpData.cvRect = r;
        tmpData.mat = croppedFace;
        cvtColor(croppedFace, tmpData.gray, CV_RGB2GRAY);
        // cv::resize( tmpData.gray, tmpData.gray, Size(WORK_SIZE,WORK_SIZE) );
        
        detectorData << tmpData;
    }
    qRegisterMetaType< QList<struct DetectorData> >( "QList<struct DetectorData>" );
    emit detectionFinished( _index, detectorData );
}

QList<DetectorData>
FaceDetector::getData(){
    return detectorData;
}
