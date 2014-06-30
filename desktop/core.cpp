#include "core.h"

void
loadCSS(){
    QFile File("res/style.css");
    File.open(QFile::ReadOnly);
    qApp->setStyleSheet( File.readAll() );
    qApp->setStyle( QApplication::style() );
}

QByteArray MatToQByteArray(cv::Mat mat){
    if (mat.cols == 0 || mat.rows == 0){
        return QByteArray();
    }
    vector<uchar> buf;
    cv::imencode( ".png", mat, buf ); // Encoder
    return QByteArray( (const char *) buf.data(), buf.size() ); // puis sauveguarder
}


QImage QByteArrayToQImage(QByteArray array){

    // qDebug() << "IMAGE: " << array;
    // qDebug() << "IMAGE SIZE: " << array.size();
    if ( array.size() == 0 ){
        qDebug() << "QByteArrayToQImage: array id empty !";
        return QImage();
    }

    QImage *qImage = new QImage();
    qImage->loadFromData( array );

    return *qImage;

    // return QImage ( (uchar*) array.data(), 48, 48, 144, QImage::Format_RGB888 );

    // QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888)
}

void
removeSpacers(QLayout* layout){
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *layoutItem = layout->itemAt(i);
        if (layoutItem->spacerItem()){
            layout->removeItem(layoutItem);
            // You could also use: layout->takeAt(i);
            delete layoutItem;
            --i;
        }
    }
}


// Converts the images given in src into a row matrix.
Mat asRowMatrix(const vector<Mat>& src, int rtype, double alpha = 1, double beta = 0) {
    // Number of samples:
    size_t n = src.size();
    // Return empty matrix if no matrices given:
    if(n == 0)
        return Mat();
    // dimensionality of (reshaped) samples
    size_t d = src[0].total();
    // Create resulting data matrix:
    Mat data(n, d, rtype);
    // Now copy data:
    for(unsigned int i = 0; i < n; i++) {
        //
        if(src[i].empty()) {
            string error_message = format("Image number %d was empty, please check your input data.", i);
            CV_Error(CV_StsBadArg, error_message);
        }
        // Make sure data can be reshaped, throw a meaningful exception if not!
        if(src[i].total() != d) {
            string error_message = format("Wrong number of elements in matrix #%d! Expected %d was %d.", i, d, src[i].total());
            CV_Error(CV_StsBadArg, error_message);
        }
        // Get a hold of the current row:
        Mat xi = data.row(i);
        // Make reshape happy by cloning for non-continuous matrices:
        if(src[i].isContinuous()) {
            src[i].reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        } else {
            src[i].clone().reshape(1, 1).convertTo(xi, rtype, alpha, beta);
        }
    }
    return data;
}


// Normalizes a given image into a value range between 0 and 255.
Mat norm_0_255(const Mat& src) {
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}


QImage
Mat2QImage(cv::Mat const& src)
{
       QImage dest;
     cv::Mat temp; // make the same cv::Mat
     if (src.channels() == 3) {
         cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
         dest = QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     } else {
        dest = QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Mono );
     } 
     QImage dest2(dest);
     dest2.detach(); // enforce deep copy
     return dest2;
}

//cv::Mat
//QImage2Mat( const QImage &img, bool inCloneImageData )
//   {
//    QImage inImage = img.convertToFormat( QImage::Format_RGB888 );
//      switch ( inImage.format() )
//      {
//         // 8-bit, 4 channel
//         case QImage::Format_RGB32:
//         {
//            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );
 
//            return (inCloneImageData ? mat.clone() : mat);
//         }
 
//         // 8-bit, 3 channel
//         case QImage::Format_RGB888:
//         {
//            if ( !inCloneImageData )
//               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
 
//            QImage   swapped = inImage.rgbSwapped();
 
//            return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
//         }
 
//         // 8-bit, 1 channel
//         case QImage::Format_Indexed8:
//         {
//            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );
 
//            return (inCloneImageData ? mat.clone() : mat);
//         }
 
//         default:
//            qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
//            break;
//      }
 
//      return cv::Mat();
//   }


cv::Mat
QImage2Mat(QImage const& src)
{
    if ( src.height() == 0 || src.width() == 0 )
        return cv::Mat();
    QImage converted = src.convertToFormat( QImage::Format_RGB888 );
    QImage swapped = converted.rgbSwapped();
    cv::Mat out( swapped.height(),swapped.width(),CV_8UC3,(uchar*)swapped.bits(),swapped.bytesPerLine() );
    return out.clone();
}


QSize
keepAspectRatio(const int w, const int h, const int max_width, const int max_height){
    double wRatio = (double) w/max_width;
    double hRatio = (double) h/max_height;
    int width = w, height = h;
    if ( wRatio > 1 || h > 1) {
        if ( wRatio > hRatio ){
            width = round( w / wRatio );
            height = round( h / wRatio );
        } else {
            width = round( w / hRatio );
            height = round( h / hRatio );
        }
    }
    return QSize(width, height);
}

QSize
keepAspectRatio(QSize size){
    return keepAspectRatio( size.width() , size.height() );
}
