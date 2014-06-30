#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "../core.h"
#include "../Engine/FaceDetector.h"
#include "../Engine/Socket.h"

#include "ShowCase.h"
#include "SidePanel.h"
#include "Theater.h"
#include "DetectedFaces.h"
#include "DropArea.h"


class ProcessImage: public DropArea
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent = 0);
    class Theater *theater;
    class DetectedFaces *detectedFaces;
    void addImages(QStringList imagesList);
    SidePanel *sidePanel;
    void clear();

    void setNbFaces( int nbFaces );
    ShowCase *showCase;
    QPushButton* newSearch;

private:
    QPushButton* backButton;


    QLabel *nbFacesLbl;

    QScrollArea *detectedFacesScrollArea;
    QHBoxLayout *layout;
    int _currentItem;
    int _currentImage;
    QList<DetectorData> _detectorData;
    QList<QImage> _imagesList;
    void processAll();
    void process(int index);
    // DataBase& smoky_db;


public slots:
    void onNewSearch();
    void onConfirmPerson();
    void onRefutePerson();
    void onBackButtonClicked();
    void onFaceRecognized(int ImageId, int faceId, int personId, int eigen_ratio, int fisher_ratio, int lbph_ratio);
    void onFaceIs(int faceId, QString firstName, QString lastName);
    void loadImageData(int index);
    void onPersonSelected(int rectId);
    void savePanelChanges(QJsonObject data);
    void insertNewData( int index, QList<struct DetectorData> detectorData);
    
    
signals:
    void backButtonClicked(int index);
    void goToView(int index);
};

#endif // IMAGEPROCESS_H
