#include "SearchPage.h"

SearchPage::SearchPage(QWidget *parent): QFrame( parent ), current_item_index(-1)
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setContentsMargins(0,0,0,0);


    layout->addWidget( searchPanel = new QFrame() );
        searchPanel->setObjectName( "searchPanel" );
        QVBoxLayout *searchPanelLayout = new QVBoxLayout( searchPanel );

        searchPanelLayout->addWidget( searchBar = new QFrame() );
            searchBar->setObjectName( "searchBar" );
            QGridLayout *searchBarLayout = new QGridLayout( searchBar );

            // SEARCH BAR
            // LINE 01
            searchBarLayout->addWidget( search_nameEdt = new QLineEdit(), 0,0 );
                search_nameEdt->setPlaceholderText( "prenom NOM" );
                search_nameEdt->setObjectName( "search_nameEdt" );

            searchBarLayout->addWidget( search_sexeCbx = new QComboBox(), 0,1 );
                search_sexeCbx->setObjectName( "search_sexeCbx" );
                search_sexeCbx->addItem("Masculin");            // 0
                search_sexeCbx->addItem("Féminin");             // 1
                search_sexeCbx->addItem("Masculin & Féminin");  // 2
                search_sexeCbx->setCurrentIndex( 2 );

            searchBarLayout->addWidget( search_professionEdt = new QLineEdit(), 0,2,1,2 );
                search_professionEdt->setPlaceholderText( "Etudiant, Docteur, ..." );
                search_professionEdt->setObjectName( "search_professionEdt" );
            
            searchBarLayout->addWidget( search_personIdEdt = new QLineEdit(), 0,4 );
                search_personIdEdt->setPlaceholderText("#id");
                search_personIdEdt->setObjectName( "search_personIdEdt" );

            // LINE 02
            searchBarLayout->addWidget( search_addressEdt = new QLineEdit(), 1,0);
                search_addressEdt->setObjectName( "search_addressEdt" );
                search_addressEdt->setPlaceholderText( "Batiment, cité, ville, région, pays." );

            searchBarLayout->addWidget( search_emailEdt = new QLineEdit(), 1,1);
                search_emailEdt->setObjectName( "search_emailEdt" );
                search_emailEdt->setPlaceholderText( "adresse@domaine.com" );

            searchBarLayout->addWidget( search_phoneEdt = new QLineEdit(), 1,2, 1, 2);
                search_phoneEdt->setObjectName( "search_phoneEdt" );
                search_phoneEdt->setPlaceholderText( "+213 xxx xxx xxx" );


            // LINE 03
            searchBarLayout->addWidget( search_birthCityEdt = new QLineEdit(), 2,0);
                search_birthCityEdt->setObjectName( "search_birthCityEdt" );
                search_birthCityEdt->setPlaceholderText( "Ville de naissance" );

            searchBarLayout->addWidget( search_birthdayEdt = new QLineEdit(), 2,1);
                search_birthdayEdt->setObjectName( "search_birthdayEdt" );
                search_birthdayEdt->setPlaceholderText( "yyyy-mm-jj" );

            searchBarLayout->addWidget( new QLabel( "Page:" ), 2, 2 );
            searchBarLayout->addWidget( search_page = new QSpinBox(), 2, 3);
            search_page->setObjectName( "search_page" );
            search_page->setMinimum( 1 );


            searchBarLayout->addWidget( search_btn = new QPushButton( "Rechercher" ) ,1,4,2,1 );
                search_btn->setObjectName( "search_btn" );
                connect( search_btn, SIGNAL(clicked()), SLOT(launchSearch()));

        searchPanelLayout->addWidget( numberResultLbl = new QLabel("Utilisez le formulaire ci-dessus pour effectuer une recherche") );
            numberResultLbl->setObjectName( "numberResultLbl" );

        // searchResult
        searchPanelLayout->addWidget( searchResultScroll = new QScrollArea( this ) );
            searchResultScroll->setWidgetResizable( true );
            searchResultScroll->setObjectName( "searchResultScroll" );

            searchResultScroll->setWidget( searchResult = new QFrame() );
            searchResult->setObjectName( "searchResult" );
            
            searchResultLayout = new QGridLayout( searchResult );
            
        searchPanelLayout->addStretch(0);


       connect( &Socket::sock(), SIGNAL(search(QList<QJsonObject>, QList<QByteArray>)), SLOT(onSearch(QList<QJsonObject>, QList<QByteArray>)) );


    // sidePanel
    layout->addWidget( sidePanel = new SidePanel( this, "edit" ) );

    m_buttonGroup = new QButtonGroup( this );
    m_buttonGroup->setExclusive( true );

    connect( m_buttonGroup, SIGNAL( buttonClicked(int) ), SLOT(onItemSelected(int) )  );
    connect( &Socket::sock(), SIGNAL(gotPersonInfo(QJsonObject, QByteArray)), sidePanel, SLOT(populate(QJsonObject, QByteArray)) );
    // connect( sidePanel, SIGNAL(confirmChanges(QJsonObject)), &Socket::sock(), SLOT(update(QJsonObject)) );
    connect( sidePanel, SIGNAL(confirmChanges(QJsonObject)), SLOT(onConfirmChanges(QJsonObject)) );

}

void 
SearchPage::onConfirmChanges(QJsonObject data){

    if (current_item_index > -1){
        DetectedFacesItem *item;
        item  = m_newItems[ current_item_index ];
        item->is( data["firstName"].toString(), data["lastName"].toString() );

        Socket::sock().update( data );
    }
}

void
SearchPage::onItemSelected(int index){

    qDebug() << "index selected: " << index;
    qDebug() << "m_newPersonsId.size()" << m_newPersonsId.size();
    qDebug() << "get info for " << m_newPersonsId[index];
    if ( m_newPersonsId.size() > index ) {
        // element exists
        current_item_index = index;

        Socket::sock().getInfo( m_newPersonsId[index] );
    }
}

void
SearchPage::launchSearch(){
    QJsonObject data;  
    data["name"] = search_nameEdt->text();
    data["sexe"] = QString::number( search_sexeCbx->currentIndex() );
    data["profession"] = search_professionEdt->text();
    data["personId"] = search_personIdEdt->text();
    data["address"] = search_addressEdt->text();
    data["email"] = search_emailEdt->text();
    data["phone"] = search_phoneEdt->text();
    data["birthCity"] = search_birthCityEdt->text();
    data["birthday"] = search_birthdayEdt->text();
    data["page"] = search_page->value();

    Socket::sock().launchSearch( data );
}

void
SearchPage::onSearch( QList<QJsonObject> items, QList<QByteArray> images){

    clear(); // remove existing elements

    numberResultLbl->setText( QString("%1 individu(s) trouvé(s)").arg(items.size()) );

    DetectedFacesItem *newItem;
    const int COLS = 5;
    int i = 0;

    foreach (QJsonObject item, items){
        searchResultLayout->addWidget( newItem = new DetectedFacesItem( QByteArrayToQImage( images[i] ) , this), i / COLS, int(i % COLS) );
        newItem->is( item["firstName"].toString(), item["lastName"].toString() );
        m_buttonGroup->addButton( newItem, m_newItems.size()  ); // start index from 0
        m_newItems << newItem;
        m_newPersonsId << item["personId"].toString().toInt();
        i++;
    }

    if (items.size() > 0){
        onItemSelected( 0 ); // select the first item
    }
}

void
SearchPage::clear(){

    foreach(DetectedFacesItem* item, m_newItems){
        delete item;
    }
    sidePanel->clear();
    m_newItems.clear();
    m_newPersonsId.clear();
}

