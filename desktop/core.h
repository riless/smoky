#ifndef CORE_H
#define CORE_H

#include <QDebug>


#include <QTabWidget>
#include <QImageReader>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QFrame>
#include <QImage>
#include <QRect>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include <QPixmap>
#include <QScrollArea>
#include <QDragEnterEvent>
#include <QApplication>
#include <QMimeData>
#include <QDropEvent>
#include <QFileDialog>
#include <QListWidget>
#include <QDropEvent>
#include <QLineEdit>

#include <QRegExp>
#include <QCompleter>
#include <QListView>
#include <QStandardItem>

#include <opencv2/core/core.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "Widgets/GPushButton.h"

using namespace std;
using namespace cv;

#ifdef unix
    const QString baseDir = "/media/OS/";
#else
    const QString baseDir = "c:/";
#endif

enum PersonType {UNKNOWN_PERSONNE, DAUBT_PERSONNE, KNOWN_PERSONNE};

void loadCSS();

struct DetectorData{
    int id;
    QImage image;
    QRect rect;
    cv::Mat mat;
    cv::Rect cvRect;
    cv::Mat gray;
};
/*
struct imagesData{
    Mat originalImage;
    Mat processImage;
    QPixmap displayImage;
};
*/
struct FaceData {
    int personId;
    int eigen_ratio;
    int fisher_ratio;
    int lbph_ratio;
    bool voted;
    QImage image;
    QRect rect;
};

#define HOME_CATEGORY "Home"
#define SEARCH_CATEGORY "Search"
#define STATS_CATEGORY "Stats"
#define HISTORY_CATEGORY "History"
#define SETTINGS_CATEGORY "Settings"
#define HELP_CATEGORY "Help"

const int MAX_DISPLAY_WIDTH = 640;
const int MAX_DISPLAY_HEIGHT = 480;

const int MAX_PROCESS_WIDTH = 640;
const int MAX_PROCESS_HEIGHT = 480;

const int WORK_SIZE = 48;


cv::Mat QImage2Mat(QImage const& src);
//cv::Mat QImage2Mat( const QImage &inImage, bool inCloneImageData = true );

QImage Mat2QImage(cv::Mat const& src);
QSize keepAspectRatio(const int w, const int h, const int max_width = MAX_DISPLAY_WIDTH, const int max_height = MAX_DISPLAY_HEIGHT);
QSize keepAspectRatio(QSize size);
void removeSpacers(QLayout* layout);
QByteArray MatToQByteArray(cv::Mat mat);
QImage QByteArrayToQImage(QByteArray array);
#endif // CORE_H
