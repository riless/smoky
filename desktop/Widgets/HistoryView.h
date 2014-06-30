#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QTableView>
#include "../Engine/Socket.h"
#include "../core.h"

class HistoryView : public QFrame
{
    Q_OBJECT
public:
    explicit HistoryView(QWidget *parent = 0);
private:
	QStringList opList;
	
	QFrame *filterBar;
	QLineEdit *filterUserEdt;
	QComboBox *filterOperation;
	QLineEdit *filterDateEdt;
	QSpinBox *pageSpn;
	QPushButton *filterBtn;
	QTableView *historyTbl;


	QPushButton *prevBtn;
	QLineEdit *pageNumberEdt;
	QPushButton *nextBtn;

	QStandardItemModel *model;
signals:

public slots:
    void updateHistory(const QList<QStandardItem *> & items);
    void loadLastLogs();
	void prevBtnClicked();
	void nextBtnClicked();
    void search();

};

#endif // HISTORYVIEW_H
