#-------------------------------------------------
#
# Project created by QtCreator 2014-03-08T10:37:46
#
#-------------------------------------------------

QT       += core gui widgets network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = smoky2
TEMPLATE = app


# Configure OpenCV
unix {
LIBS += -lopencv_core \
        -lopencv_imgproc \ 
        -lopencv_highgui \
        -lopencv_ml \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_objdetect \
        -lopencv_contrib \
        -lopencv_legacy \
        -lopencv_flann
}

win32 {
INCLUDEPATH += "C:\opencv2\install\include"
LIBS += -L"C:\opencv2\install\x86\mingw\lib" \
        -lopencv_core248d \
        -lopencv_calib3d248d \
        -lopencv_imgproc248d \
        -lopencv_highgui248d \
        -lopencv_ml248d \
        -lopencv_video248d \
        -lopencv_features2d248d \
        -lopencv_objdetect248d \
        -lopencv_contrib248d \
        -lopencv_legacy248d \
        -lopencv_flann248d
}


SOURCES += main.cpp\
        mainwindow.cpp \
    Widgets/HomePage.cpp \
    Widgets/SideBar.cpp \
    Widgets/StackedWidget.cpp \
    Widgets/SearchPage.cpp \
    Widgets/Header.cpp \
    Widgets/DropArea.cpp \
    Widgets/ProcessImage.cpp \
    Engine/FaceDetector.cpp \
    Widgets/Theater.cpp \
    Widgets/FaceRect.cpp \
    Widgets/DetectedFaces.cpp \
    Widgets/DetectedFacesItem.cpp \
    core.cpp \
    Widgets/SidePanel.cpp \
    Widgets/ShowCase.cpp \
    Widgets/LoadView.cpp \
    Widgets/HistoryView.cpp \
    Widgets/settingsView.cpp \
    Widgets/AboutView.cpp \
    inc/qcustomplot/qcustomplot.cpp \
    Widgets/StatsView.cpp \
    Widgets/AddView.cpp

HEADERS  += mainwindow.h \
    Widgets/HomePage.h \
    Widgets/SideBar.h \
    Widgets/StackedWidget.h \
    Widgets/SearchPage.h \
    Widgets/Header.h \
    Widgets/LoadImage.h \
    Widgets/DropArea.h \
    Widgets/ProcessImage.h \
    Engine/FaceDetector.h \
    Widgets/Theater.h \
    Widgets/FaceRect.h \
    Widgets/CamPage.h \
    Widgets/LoadVideo.h \
    Widgets/DetectedFaces.h \
    Widgets/DetectedFacesItem.h \
    core.h \
    Engine/EigenFace.h \
    Widgets/SidePanel.h \
    Widgets/ShowCase.h \
    Widgets/LoadView.h \
    Widgets/GLabel.h \
    Widgets/Overlay.h \
    Engine/Socket.h \
    inc/qlightboxwidget/qlightboxwidget.h \
    Widgets/HistoryView.h \
    Widgets/settingsView.h \
    Widgets/AboutView.h \
    Engine/Camera.h \
    inc/qcustomplot/qcustomplot.h \
    Widgets/StatsView.h \
    Widgets/AddView.h \
    Widgets/GPushButton.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
