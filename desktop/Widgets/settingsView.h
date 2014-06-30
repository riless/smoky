#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QFrame>
#include <QRadioButton>
#include <QTableView>
#include <QHeaderView>
#include <QSpinBox>
#include <QTableWidget>
#include "../core.h"
#include "../Engine/Socket.h"
#include <QComboBox>

class SettingsView : public QFrame
{
    Q_OBJECT
public:
    explicit SettingsView(QWidget *parent = 0);

private:
	QButtonGroup *methodBG;
	QHBoxLayout* topUsersLayout;

	QPushButton* addUserBtn;
	QPushButton* editUserBtn;
	QPushButton* deleteUserBtn;

    QLabel *optionsLbl;
	QLabel *methodLbl;
	QRadioButton *eigenFaceRadio;
	QRadioButton *fisherFaceRadio;
	QRadioButton *LBPHFaceRadio;
	QLabel *thresholdLbl;
	QSpinBox *eigenFaceSbx;
	QSpinBox *fisherFaceSbx;
	QSpinBox *LBPHFaceSbx;
	QPushButton *saveUsers;

	QLabel *usersLbl;
	QTableView *usersTable;
	QStandardItemModel *model;
	QPushButton *saveOptions;

	QFrame *userFormFrame;
	QLabel *userIdLbl;
	QLineEdit *usernameEdt;
	QLineEdit *passwordEdt;
	QComboBox *roleCbx;
	QSpinBox *personIdSbx;
	QPushButton *userConfirmBtn;


signals:

public slots:
	void onSendUser();
	void onAddUser();
	/* void onEditUser(); */
	void onDeleteUser();
	void onUserSelected(QModelIndex item);
	void onSettingsLoaded( QList<QStandardItem*> items, QJsonObject settings );
	void onSaveOptions();
};

#endif // SETTINGSVIEW_H
