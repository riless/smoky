#ifndef ADDVIEW_H
#define ADDVIEW_H

#include <QFrame>
#include "DropArea.h"
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>
#include <QThread>
#include <QTimer>
#include "ShowCase.h"
#include "../Engine/Camera.h"
#include "SidePanel.h"
#include "Theater.h"
#include "../Engine/Socket.h"
#include "../Engine/FaceDetector.h"
#include "../core.h"

class AddView : public DropArea
{
    Q_OBJECT
public:
    explicit AddView(QWidget *parent = 0);
    ~AddView();
	Camera *cam;
	QThread *t;
	QTimer *mTimer;

    QPushButton *chooseCam;
    QPushButton *chooseImages;
    QPushButton *chooseVideo;
private:
	QScrollArea *imagesScrollArea;

	QLabel *imageLbl;
	QImage m_currentFace;
	QList<QImage> m_savedFaces;

	QLabel *addLbl;
	SidePanel *sidePanel;
	QButtonGroup *_buttonGroup;

    Theater *theater;
	ShowCase *showCase;
	QLabel *counterLbl;
	QPushButton *takePic;
	QPushButton *delPic;

	QFrame *chooseMediaFrm;
	QButtonGroup *m_chooseMediaBG;

	QPushButton *newPersonBtn;

	CascadeClassifier _cascade;

	int m_currentSelected;
	QAbstractButton* m_currentButton;

signals:
	void startWork();
	void stopWork();
public slots:
	void onButtonSelected(QAbstractButton* b);
	void onNewPersonClicked();
	void onFilesLoaded( QStringList files );
	void onSendFace( QImage );
	void onStopCamera();
	void onChooseMedia(int buttonId);
	void addPerson(QJsonObject formData);
	void onSendShot(QImage face, QRect rect);
	void onTakePic();
	void onDelPic();
	void onImageSelected(int index);
	void onSendFrame(QImage frame);
	void startCamera();
	void stopCamera();
    void onRemovePic(int index);

};

#endif // ADDVIEW_H
