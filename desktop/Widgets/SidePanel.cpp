#include "SidePanel.h"
#include <QDate>

SidePanel::SidePanel(QWidget *parent, QString type): QFrame(parent)// , panelId(0)
{

        m_type = type;
        // qDebug() << "instantiate SidePanel" << this;
        // smoky_db = DataBase::dbi();
        //COL3
        QVBoxLayout* col3 = new QVBoxLayout( this );
        col3->setContentsMargins(0,0,0,0);
        col3->setSpacing(0);

        QTabWidget *tabWidget = new QTabWidget( this );
        
        tabWidget->setObjectName( "tabWidget" );
        col3->addWidget( tabWidget );

        // info page
        QWidget *infoPage = new QWidget();
        infoPage->setObjectName( "infoPage" );
        QVBoxLayout *infoPageLayout = new QVBoxLayout( infoPage );
        infoPageLayout->setContentsMargins(0,0,0,0);


        // map
        QWidget *mapPage = new QWidget();
        QVBoxLayout *mapPageLayout = new QVBoxLayout();
        mapPage->setLayout( mapPageLayout );

        // casier page
        QWidget *casierPage = new QWidget();
        QVBoxLayout *casierPageLayout = new QVBoxLayout();
        casierPage->setLayout( casierPageLayout );

        // visages page
        QWidget *visagesPage = new QWidget();
        QVBoxLayout *visagesPageLayout = new QVBoxLayout();
        visagesPage->setLayout( visagesPageLayout );

        tabWidget->addTab(infoPage, "Informations");
        /*
        tabWidget->addTab(mapPage, "Carte");
        tabWidget->addTab(casierPage, "Casier judiciaire");
        tabWidget->addTab(visagesPage, "Visages");
        */

        /* GRID */
        QWidget *topPanel;
        infoPageLayout->addWidget( topPanel = new QWidget() );
        topGrid = new QGridLayout( topPanel );
        topPanel->setObjectName( "topPanel" );

        topGrid->addWidget( faceLbl = new QLabel(""), 0, 0, 2, 1 );
        faceLbl->setObjectName( "faceLbl" );
        faceLbl->setAlignment(Qt::AlignCenter);

        topGrid->addWidget( fullNameEdt = new QLineEdit(), 0, 1 );
        fullNameEdt->setObjectName( "fullNameEdt" );
        fullNameEdt->setPlaceholderText( "prenom NOM" );

        topGrid->addWidget( personIdEdt = new QLineEdit("-1"), 1, 1 );
        personIdEdt->hide();
        if (type == "edit"){
            personIdEdt->show();
            personIdEdt->setObjectName( "personIdEdt" );
            personIdEdt->setPlaceholderText( "#id" );
            personIdEdt->setReadOnly( true );
        }

        
        /* FORM */

        infoPageLayout->addLayout( formLayout = new QFormLayout() );
        formLayout->setContentsMargins(10,10,10,10);

        formLayout->addRow( "Adresse de résidance", addressTxt = new QPlainTextEdit() );
        addressTxt->setObjectName( "addressTxt" );
        addressTxt->setPlainText( "Batiment, cité, ville, région, pays." );

        formLayout->addRow( "Sexe", sexeCbx = new QComboBox() );
        sexeCbx->addItem("Masculin");
        sexeCbx->addItem("Féminin");
/*
        formLayout->addRow( "NSS",  nssEdt = new QLineEdit() );
        nssEdt->setObjectName( "nssEdt" );
        nssEdt->setPlaceholderText( "xxxxxxxxxxxxxxx" ); // 13 chiffres et 2 chiffres de controle
*/
        formLayout->addRow( "Email",  emailEdt = new QLineEdit() );
        emailEdt->setObjectName( "emailEdt" );
        emailEdt->setPlaceholderText( "adresse@domaine.com" );

        formLayout->addRow( "Date de naissance", dateLayout = new QHBoxLayout() );
        dateLayout->addWidget( daySpn = new QSpinBox() );
        daySpn->setObjectName( "daySpn" );
        daySpn->setMinimum(1);
        daySpn->setMaximum(31);

        dateLayout->addWidget( monthCbx = new QComboBox() );
        monthCbx->setObjectName( "monthCbx" );
        QStringList months;
        months << "Janvier" << "Février" << "Mars" << "Avril" << "Mai" << "Juin" << "Juillet" << "Août" << "Septembre" << "Octobre" << "Novembre" << "Décembre";
        monthCbx->addItems( months );

        dateLayout->addWidget( yearSpn = new QSpinBox() );
        yearSpn->setObjectName( "yearSpn" );
        yearSpn->setMinimum(1900);
        yearSpn->setMaximum( QDate::currentDate().year() );

        formLayout->addRow( "Ville de naissance",  birthCityEdt = new QLineEdit() );
        birthCityEdt->setObjectName( "birthCityEdt" );
        birthCityEdt->setPlaceholderText( "Entrez la ville de naissance" );

        formLayout->addRow( "Numéro de téléphone",  phoneEdt = new QLineEdit() );
        phoneEdt->setObjectName( "phoneEdt" );
        phoneEdt->setPlaceholderText( "+213 xxx xxx xxx" );

        formLayout->addRow( "Profession, grade",  professionEdt = new QLineEdit() );
        professionEdt->setObjectName( "profession" );
        professionEdt->setPlaceholderText( "Etudiant, Docteur, ..." );

        bottomLayout = new QHBoxLayout();
        bottomLayout->setContentsMargins(10,10,10,10);
        bottomLayout->addStretch(0);

        bottomLayout->addWidget( sendLoader = new QLabel() );
        bottomLayout->setSpacing( 0 );
        sendLoader->setObjectName( "sendLoader" );

        if (type == "view"){
            QVBoxLayout *ratioLayout = new QVBoxLayout();
            ratioLayout->setContentsMargins(10,10,10,10);

            tauxRecLbl = new QLabel( "" );
            tauxRecLbl->setObjectName( "tauxRecLbl" );
            ratioLayout->addWidget( tauxRecLbl );

            eigenRatioLbl = new QLabel();
            eigenRatioLbl->setObjectName( "eigenRatioLbl" );
            ratioLayout->addWidget( eigenRatioLbl );

            fisherRatioLbl = new QLabel();
            fisherRatioLbl->setObjectName( "fisherRatioLbl" );
            ratioLayout->addWidget( fisherRatioLbl );

            lbphRatioLbl = new QLabel();
            lbphRatioLbl->setObjectName( "lbphRatioLbl" );
            ratioLayout->addWidget( lbphRatioLbl );

            infoPageLayout->addLayout( ratioLayout );

            refuteBtn = new QPushButton( "Réfuter" );
            refuteBtn->setObjectName( "refuteBtn" );
            connect( refuteBtn, SIGNAL(clicked()), SLOT(onRefute()) );
            bottomLayout->addWidget( refuteBtn );

/*            QPushButton *ignoreBtn = new QPushButton( "Ignorer" );
            ignoreBtn->setObjectName( "ignoreBtn" );
            connect( ignoreBtn, SIGNAL(clicked()), SLOT(onIgnore()) );
            bottomLayout->addWidget( ignoreBtn );
*/
            confirmBtn = new QPushButton( "Confirmer" );
            confirmBtn->setObjectName( "confirmBtn" );
            connect( confirmBtn, SIGNAL(clicked()), SLOT(onConfirm()) );
            bottomLayout->addWidget( confirmBtn );
            // fullNameEdt->setReadOnly( true );
            // personIdEdt->setReadOnly( true );
            // addressTxt->setReadOnly( true );
            // emailEdt->setReadOnly( true );
            // sexeCbx->setEnabled( true );
            // daySpn->setReadOnly( true );
            // monthCbx->setEnabled( true );
            // yearSpn->setReadOnly( true );
            // birthCityEdt->setReadOnly( true );
            // phoneEdt->setReadOnly( true );
            // professionEdt->setReadOnly( true );
        }


        infoPageLayout->addLayout( bottomLayout );


        if (type == "add" || type == "edit"){
            bottomLayout->addWidget( confirmChangesBtn = new QPushButton("Ajouter") );
            confirmChangesBtn->setObjectName( "confirmChangesBtn" );
            connect(confirmChangesBtn, SIGNAL(clicked()), SLOT(confirmChangesBtnClicked()) );
        }

        if (type == "edit"){
            confirmChangesBtn->setText( "Confirmer les changements" );
        }
        

       

}

void
SidePanel::hideVoteButtons(){
    refuteBtn->hide();
    // ignoreBtn->hide();
    confirmBtn->hide();
}

void
SidePanel::showVoteButtons(){
    refuteBtn->show();
    // ignoreBtn->show();
    confirmBtn->show();
}


void
SidePanel::onConfirm(){
    qDebug() << "onConfirm()";
    emit confirmPerson();
    hideVoteButtons();

}

void
SidePanel::onRefute(){
    qDebug() << "onRefute()";
    emit refutePerson();
    hideVoteButtons();
}

/*void
SidePanel::onIgnore(){
    emit ignorePerson();
    hideVoteButtons();
}*/

/*
void
SidePanel::loadCompleterList(){
    // QMap<int, QString> wordList = DataBase::dbi().getCompleterList();
    QStandardItemModel* model = new QStandardItemModel();
    // model->setRowCount( wordList.size() );
    model->setColumnCount(1);

    // QMapIterator<int, QString> it(wordList);
    int index = 0;
//    while( it.hasNext() ){
//        it.next();
    
        QStandardItem* item = new QStandardItem( it.value() );
        // item->setIcon( QIcon( QString( "db/%1/%2.color.png").arg( it.key() ).arg(DataBase::dbi().getDefaultFace( it.key() ) ) ) );
        item->setSizeHint( QSize(48, 48) );
        item->setToolTip("some tool tip");
        
        item->setData( it.key() );
        model->setItem(index++, item);
    }


    QCompleter *completer = new QCompleter( model, this );
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::UnsortedModel);
    fullNameEdt->setCompleter(completer);

    connect(completer, SIGNAL(activated(QModelIndex)), SLOT(completerItemSelected(QModelIndex)) );
}

void
SidePanel::completerItemSelected(QModelIndex CompleterItemId){
    // THE PERSON ID IS STORED IN Qt::UserRole + 1
    clear();
    qDebug() << "userRole + 1 = " << CompleterItemId.data(Qt::UserRole + 1).toInt();
    // populate( CompleterItemId.data(Qt::UserRole + 1).toInt() );

}
*/

void
SidePanel::clear(){
    fullNameEdt->setText("");
    personIdEdt->setText("");
    addressTxt->setPlainText( "Batiment, cité, ville, région, pays." );
    emailEdt->setText( "" );
    sexeCbx->setCurrentIndex( 0 );
    // nssEdt->setText( info["NSS"].toString() );
    daySpn->setValue( 1 );
    monthCbx->setCurrentIndex( 0 ); // comboboxes starts from 0
    yearSpn->setValue( 1900 );
    birthCityEdt->setText( "" );
    phoneEdt->setText( "" );
    professionEdt->setText(  "" );
    
    faceLbl->setPixmap( QPixmap() );

}

void
SidePanel::setRatio(int eigen_ratio, int fisher_ratio, int lbph_ratio){
    if (m_type == "view"){
        qDebug() << "setRatio";
        tauxRecLbl->setText( "<h2>Taux de Reconnaissance / Méthode</h2>" );
        if ( !eigen_ratio && !fisher_ratio && !lbph_ratio ){
            tauxRecLbl->setText( "<h2>Personne inconnue</h2>" );
        }

        eigenRatioLbl->setText( QString("Eigenfaces ( <b>%1%</b> )").arg( eigen_ratio ) );
        fisherRatioLbl->setText( QString("Fisherfaces ( <b>%1%</b> )").arg( fisher_ratio ) );
        lbphRatioLbl->setText( QString("LBPH ( <b>%1%</b> )").arg( lbph_ratio ) );
    }
}

void
SidePanel::populate(QJsonObject info, QByteArray array){

    qDebug() << "<POPULATE>";
    qDebug() << "info" << info;

    clear();

    fullNameEdt->setText( info["firstName"].toString() + " " + info["lastName"].toString() ) ;
    personIdEdt->setText( info["id"].toString() );
    addressTxt->setPlainText( info["address"].toString() );
    emailEdt->setText( info["email"].toString() );
    sexeCbx->setCurrentIndex( info["sexe"].toString().toInt() );
    // nssEdt->setText( info["NSS"].toString() );
    daySpn->setValue( info["day"].toString().toInt() );
    monthCbx->setCurrentIndex( info["month"].toString().toInt() - 1 ); // comboboxes starts from 0
    yearSpn->setValue( info["year"].toString().toInt() );
    birthCityEdt->setText( info["birthCity"].toString() );
    phoneEdt->setText( info["phone"].toString() );
    professionEdt->setText( info["profession"].toString() );

    // vector<uchar*> inputArray = (uchar*) raw.data();
    // cv::Mat mat = cv::imdecode( inputArray, CV_LOAD_IMAGE_COLOR );
    // QImage qImage = Mat2QImage( mat );
    // QImage *qImage = new QImage();
    // qImage->loadFromData( raw );
    // 
    QImage image = QByteArrayToQImage(array);

    
    faceLbl->setPixmap( QPixmap::fromImage( image.scaled(QSize(96, 96), Qt::KeepAspectRatio, Qt::SmoothTransformation) ) );

/*
QLabel *faceLbl;
*/

}

//void
//SidePanel::load(int personId){

//    // loadCompleterList();
//    if (personId <= 0){
//        clear();
//    } else {
//        populate(personId);
//    }
//}

void
SidePanel::confirmChangesBtnClicked(){
    if ( valuesOk() /* && faceLbl->text().toInt() >= 10*/ ){
        QJsonObject data;
        data["personId"]   = get("personId");
        data["firstName"]  = get("firstName");
        data["lastName"]   = get("lastName");
        data["address"]    = get("address");
        data["email"]      = get("email");
        data["sexe"]       = get("sexe");
        // data["nss"]     = get("nss");
        data["birthday"]   = get("date");
        data["birthCity"]  = get("birthCity");
        data["phone"]      = get("phone");
        data["profession"] = get("profession");

        emit confirmChanges( data );
    }


}
// /^\S+@\S+\.\S+$/
// 
QString
SidePanel::regexCheck(QString str, QString regex){
    QRegExp rx ( regex );
    rx.indexIn( str );
    QStringList list = rx.capturedTexts();
    QString r = (list.size() > 0)? list.at(0): "";
    return r.trimmed();
}

QString
SidePanel::get(QString field){
    QString r;
    if (field == "fullName") r = fullNameEdt->text();

    if (field == "firstName") {
        r = regexCheck( fullNameEdt->text(), "(([A-Z]? *)*[a-z]+)*" );
    }

    if (field == "lastName") {
        r = regexCheck( fullNameEdt->text(), "[A-Z]{2}[A-Z ]*" );
    }

    if (field == "personId") {
        r = personIdEdt->text();
    }

    if (field == "address") {
        r = addressTxt->toPlainText();
    }

    if (field == "email") {
        r = emailEdt->text();
    }

    if (field == "sexe") {
        r = QString::number( sexeCbx->currentIndex() );
    }

    if (field == "nss") {
        r = nssEdt->text();
    }

    if (field == "date"){
        r = QString( "%1-%2-%3" ).arg( yearSpn->value() ).arg( monthCbx->currentIndex() + 1 ).arg( daySpn->value() );
    }

    if (field == "birthCity"){
        r = birthCityEdt->text();
    }

    if (field == "phone"){
        r = phoneEdt->text();
    }

    if (field == "profession"){
        r = professionEdt->text();
    }    
    return r;

}

bool
SidePanel::valuesOk(){
    return true;

    bool isOk = false;

    if (
        get("firstName").size() > 1 &&
        get("lastName").size() > 1 &&
        get("address").size() > 5 &&
        regexCheck( get("email"), "^([a-z0-9_\.-]+)@([\da-z\.-]+)\.([a-z\.]{2,6})$" ).size() > 5 &&
        ( get("sexe") == "0" || get("sexe") == "1" ) &&
        get("date").size() > 2 &&
        regexCheck( get("date"), "/^\S+@\S+\.\S+$/" ).size() > 5 &&
        get("birthCity").size() > 2 &&
        regexCheck( get("phone"), "((00|\+)213|0)?[567][0-9]{8}" ).size() > 2 &&
        get("profession").size() > 2
        ) {
        isOk = true;
    }

    return isOk;

    // vector<uchar*> inputArray = (uchar*) raw.data();
    // cv::Mat mat = cv::imdecode( inputArray, CV_LOAD_IMAGE_COLOR );
    //QImage qImage = Mat2QImage( mat );
    // QImage *qImage = new QImage();
}

