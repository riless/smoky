#include "HistoryView.h"
#include <QHeaderView>

HistoryView::HistoryView(QWidget *parent) :
    QFrame(parent)
{
    //FILTER BAR
    QVBoxLayout *layout = new QVBoxLayout( this );
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget( filterBar = new QFrame() );
    filterBar->setObjectName( "filterBar" );
    QHBoxLayout *filterBarLayout = new QHBoxLayout( filterBar );

    filterBarLayout->addWidget( filterUserEdt = new QLineEdit() );
    filterUserEdt->setPlaceholderText( "Nom d'utilisateur" );
    filterUserEdt->setObjectName( "filterUserEdt" );

    filterBarLayout->addWidget( filterOperation = new QComboBox() );
    filterOperation->setObjectName( "filterOperation" );
    
    opList << "Toutes les opérations" << "Connection" << "Deconnexion" << "Ajout d'utilisateur" << "Mise a jour de l'utilisateur" << "Suppression d'utilisateur";
    opList << "Ajout" << "Suppression" << "Modification" << "Recherche" << "Recherche par image" << "Consultation";
    filterOperation->addItems( opList );

   	filterBarLayout->addWidget( filterDateEdt = new QLineEdit() );
    filterDateEdt->setPlaceholderText( "aaaa-mm-jj" );
    filterDateEdt->setObjectName( "filterDateEdt" );

    filterBarLayout->addWidget( new QLabel("Page") );
   	filterBarLayout->addWidget( pageSpn = new QSpinBox() );
    pageSpn->setMinimum( 1 );
    pageSpn->setObjectName( "pageSpn" );


    filterBarLayout->addWidget( filterBtn = new QPushButton( "Filtrer" ) );
    filterBtn->setObjectName( "filterBtn" );
    connect( filterBtn, SIGNAL(clicked()), SLOT(search()));


    // HISTORY TABLE
    layout->addWidget( historyTbl = new QTableView() );
    historyTbl->setObjectName( "historyTbl" );

    model = new QStandardItemModel(20,3, this);
    model->setHorizontalHeaderItem(0, new QStandardItem("UTILISATEUR") );
    model->setHorizontalHeaderItem(1, new QStandardItem("OPERATION") );
    model->setHorizontalHeaderItem(2, new QStandardItem("DATE & HEURE") );
    historyTbl->setModel(model);

    historyTbl->verticalHeader()->hide();
    historyTbl->verticalHeader()->setDefaultSectionSize( 40 );
    historyTbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTbl->setFocusPolicy(Qt::NoFocus);

    historyTbl->setColumnWidth(0, 100);
    historyTbl->setColumnWidth(2, 150);
    historyTbl->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch);

    connect( &Socket::sock(), SIGNAL(historyLoaded(const QList<QStandardItem *>)), SLOT(updateHistory(const QList<QStandardItem *>)) );

/*
    // PAGINATION
    QHBoxLayout *pagination = new QHBoxLayout();
    layout->addLayout( pagination );

    pagination->addStretch(0);

    pagination->addWidget( prevBtn = new QPushButton("<") );
    connect( prevBtn, SIGNAL(clicked()), SLOT(prevBtnClicked()));

    pagination->addWidget( pageNumberEdt = new QLineEdit() );

    pagination->addWidget( nextBtn = new QPushButton(">") );
    connect( nextBtn, SIGNAL(clicked()), SLOT(nextBtnClicked()));
*/

    filterBarLayout->addStretch(0);

    // connect( &Socket::sock(), SIGNAL(userLoggedIn(QString)), SLOT(loadLastLogs()) );

}

void
HistoryView::updateHistory(const QList<QStandardItem *> & items){
    model->removeRows(0,model->rowCount());


    const int COLS = 3;
    for (int i=0; i<items.size(); ++i){
    	model->setItem(i / COLS, int(i % COLS), items.at(i) );
    }
}

void
HistoryView::loadLastLogs(){
   
}

void
HistoryView::search(){
	/* getHistory(QString, int, QString, int) */
	Socket::sock().getHistory(filterUserEdt->text(), filterOperation->currentIndex(), filterDateEdt->text(), pageSpn->value() );
}

void
HistoryView::prevBtnClicked(){

}

void
HistoryView::nextBtnClicked(){

}
