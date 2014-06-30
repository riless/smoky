#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include "../core.h"
#include <QDateEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QPlainTextEdit>
#include <QJsonObject>

class SidePanel: public QFrame
{
    Q_OBJECT
public:
    SidePanel(QWidget* parent = 0, QString type = "");
    void load(int personId);
    QString get(QString field);
    bool valuesOk();
    void clear();
    QString regexCheck(QString str, QString regex);
    QLabel *faceLbl;

    QPushButton *refuteBtn;
    QPushButton *confirmBtn;
private:
    /* FORM */
    QGridLayout *topGrid;

    QLineEdit *fullNameEdt;
    QLineEdit *personIdEdt;

    QFormLayout *formLayout;
    QPlainTextEdit *addressTxt;
    QLineEdit *emailEdt;
    QComboBox* sexeCbx;
    QLineEdit *nssEdt;

    QHBoxLayout *dateLayout;
    QSpinBox *daySpn;
    QComboBox *monthCbx;
    QSpinBox *yearSpn;

    QLineEdit *birthCityEdt;
    QLineEdit *phoneEdt;
    QLineEdit *professionEdt;

    QLabel *tauxRecLbl;
    QLabel *eigenRatioLbl;
    QLabel *fisherRatioLbl;
    QLabel *lbphRatioLbl;

    QString m_type;
    
    QHBoxLayout *bottomLayout;
    QLabel *sendLoader;
    QPushButton *confirmChangesBtn;

    // void loadCompleterList();

public slots:
    void setRatio(int eigen_ratio, int fisher_ratio, int lbph_ratio);
    void populate(QJsonObject jsonObj, QByteArray raw);
    // void confirmChangesBtnClicked();
    // void completerItemSelected(QModelIndex CompleterItemId);
    void confirmChangesBtnClicked();
    void onConfirm();
    void onRefute();
    // void onIgnore();
    void showVoteButtons();
    void hideVoteButtons();

signals:
    void confirmPerson();
    void refutePerson();
    void confirmChanges(QJsonObject formData);
};

#endif // SIDEPANEL_H
