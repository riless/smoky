#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include "../core.h"
#include "SidePanel.h"
#include <QJsonObject>
#include "../Engine/Socket.h"
#include "DetectedFacesItem.h"

class SearchPage: public QFrame
{
    Q_OBJECT
public:
    SearchPage(QWidget *parent = 0);

private:
	QFrame *searchBar;
	QFrame *searchPanel;
	QLineEdit *searchField;

	QLineEdit *search_nameEdt;
	QComboBox *search_sexeCbx;
	QLineEdit *search_professionEdt;
	QLineEdit *search_personIdEdt;
	QLineEdit *search_addressEdt;
	QLineEdit *search_emailEdt;
	QLineEdit *search_phoneEdt;
	QLineEdit *search_birthCityEdt;
	QLineEdit *search_birthdayEdt;
	QSpinBox *search_page;
	QPushButton *search_btn;

	QButtonGroup *m_buttonGroup;

	QLabel *numberResultLbl;
	SidePanel *sidePanel;

	QScrollArea *searchResultScroll;
	QFrame *searchResult;
	QGridLayout *searchResultLayout;

	QList<DetectedFacesItem*> m_newItems;
	QList<int> m_newPersonsId;
	int current_item_index;

public slots:
    void onConfirmChanges(QJsonObject data);
	void launchSearch();
    void onSearch( QList<QJsonObject> items, QList<QByteArray> images );
	void clear();
	void onItemSelected(int index);
signals:
    void search(QList<QJsonObject>, QList<QByteArray>);
    void itemClicked( int );

};

#endif // SEARCHPAGE_H
