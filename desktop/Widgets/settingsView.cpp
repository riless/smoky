#include "settingsView.h"

SettingsView::SettingsView(QWidget *parent) :
    QFrame(parent)
{
	QHBoxLayout *layout = new QHBoxLayout( this );

		QFrame *leftFrame = new QFrame();
		leftFrame->setObjectName( "leftFrame" );
		QVBoxLayout *leftLayout = new QVBoxLayout( leftFrame );
		layout->addWidget( leftFrame );

			leftLayout->addWidget( optionsLbl = new QLabel( "Options" ) );
			optionsLbl->setObjectName( "optionsLbl" );

                QGridLayout *methodLayout = new QGridLayout();
				leftLayout->addLayout(methodLayout);

                methodBG = new QButtonGroup();
				methodBG->setExclusive( true );

				methodLayout->addWidget( methodLbl = new QLabel("Méthodes"), 0, 0);
				methodLbl->setObjectName( "methodLbl" );

				methodLayout->addWidget( eigenFaceRadio = new QRadioButton("EigenFace"), 1, 0 );
                methodBG->addButton( eigenFaceRadio, 0 );
				eigenFaceRadio->setObjectName( "eigenFaceRadio" );

				methodLayout->addWidget( fisherFaceRadio = new QRadioButton("FisherFace"), 2, 0 );
                methodBG->addButton( fisherFaceRadio, 1 );
				fisherFaceRadio->setObjectName( "fisherFaceRadio" );

				methodLayout->addWidget( LBPHFaceRadio = new QRadioButton("LBPH"), 3, 0);
                methodBG->addButton( LBPHFaceRadio, 2 );
				LBPHFaceRadio->setObjectName( "LBPHFaceRadio" );


				methodLayout->addWidget( thresholdLbl = new QLabel("Seuils"), 0, 1);
				thresholdLbl->setObjectName( "thresholdLbl" );

                methodLayout->addWidget( eigenFaceSbx = new QSpinBox(), 1, 1);
                eigenFaceSbx->setMaximum( 999999 );
                eigenFaceSbx->setObjectName( "eigenFaceSbx" );
                
                methodLayout->addWidget( fisherFaceSbx = new QSpinBox(), 2, 1);
                fisherFaceSbx->setMaximum( 999999 );
                fisherFaceSbx->setObjectName( "fisherFaceSbx" );
                
                methodLayout->addWidget( LBPHFaceSbx = new QSpinBox(), 3, 1);
                LBPHFaceSbx->setMaximum( 999999 );
                LBPHFaceSbx->setObjectName( "LBPHFaceSbx" );

            QHBoxLayout *saveOptionsLayout = new QHBoxLayout();
            saveOptionsLayout->addStretch(0);
            
            saveOptionsLayout->addWidget( saveOptions = new QPushButton("Sauveguarder les changement") );
            saveOptions->setObjectName( "saveOptions" );
            connect( saveOptions, SIGNAL(clicked()), SLOT(onSaveOptions()) );
            
           
           /*	QHBoxLayout *saveUsersLayout = new QHBoxLayout();
	            saveUsersLayout->addStretch(0);
	            saveUsersLayout->addWidget( saveUsers = new QPushButton("Sauveguarder les changement") );
	            saveUsers->setObjectName( "saveUsers" );
	            connect( saveUsers, SIGNAL(clicked()), SLOT(onSaveOptions()) );
	            rightLayout->addLayout( saveUsersLayout );
			*/
            leftLayout->addLayout( saveOptionsLayout );

			leftLayout->addStretch(0);

		layout->addSpacing(50);

		QVBoxLayout *rightLayout = new QVBoxLayout();
		layout->addLayout( rightLayout );

				rightLayout->addLayout( topUsersLayout = new QHBoxLayout() );
				topUsersLayout->addWidget( usersLbl = new QLabel( "Utilisateurs" ) );
				usersLbl->setObjectName( "usersLbl" );

				topUsersLayout->addWidget( addUserBtn = new QPushButton("Ajouter") );
				addUserBtn->setObjectName( "addUserBtn" );
				connect( addUserBtn, SIGNAL(clicked()), SLOT(onAddUser()) );

/*				topUsersLayout->addWidget( editUserBtn = new QPushButton("Editer") );
				editUserBtn->setObjectName( "editUserBtn" );
				connect( editUserBtn, SIGNAL(clicked()), SLOT(onEditUser()) );
*/

				topUsersLayout->addWidget( deleteUserBtn = new QPushButton("Supprimer") );
				deleteUserBtn->setObjectName( "deleteUserBtn" );
				deleteUserBtn->setDisabled( true );
                connect( deleteUserBtn, SIGNAL(clicked()), SLOT(onDeleteUser()) );

				topUsersLayout->addStretch(0);

            userFormFrame = new QFrame();
			userFormFrame->setObjectName( "userFormFrame" );
			QHBoxLayout *userFormLayout = new QHBoxLayout( userFormFrame );
			rightLayout->addWidget( userFormFrame );

				userFormLayout->addWidget( userIdLbl = new QLabel("*") );
				userIdLbl->setObjectName( "userIdLbl" );

				userFormLayout->addWidget( usernameEdt = new QLineEdit() );
				usernameEdt->setObjectName( "usernameEdt" );
                usernameEdt->setPlaceholderText( "Nom D'utilisateur" );

				userFormLayout->addWidget( passwordEdt = new QLineEdit() );
				passwordEdt->setObjectName( "passwordEdt" );
				passwordEdt->setEchoMode( QLineEdit::Password );
                passwordEdt->setPlaceholderText( "Mot de passe" );

				userFormLayout->addWidget( new QLabel( "Role" ) );
				userFormLayout->addWidget( roleCbx = new QComboBox() );
				roleCbx->setObjectName( "roleCbx" );
				roleCbx->addItem( "user" );
				roleCbx->addItem( "admin" );

				userFormLayout->addWidget( new QLabel( "ID Personne" ) );
				userFormLayout->addWidget( personIdSbx = new QSpinBox() );
				personIdSbx->setObjectName( "personIdSbx" );

				userFormLayout->addWidget( userConfirmBtn = new QPushButton("OK") );
				userConfirmBtn->setObjectName( "userConfirmBtn" );
                connect(userConfirmBtn, SIGNAL(clicked()), SLOT(onSendUser()) );

				userFormLayout->addStretch(0);

			userFormFrame->hide();



            rightLayout->addWidget( usersTable = new QTableView() );
			usersTable->setObjectName( "usersTable" );

            model = new QStandardItemModel(this);
		    model->setHorizontalHeaderItem(0, new QStandardItem("#") );
		    model->setHorizontalHeaderItem(1, new QStandardItem("NOM D'UTILISATEUR") );
		    model->setHorizontalHeaderItem(2, new QStandardItem("ID PERSONNE") );
		    model->setHorizontalHeaderItem(3, new QStandardItem("ROLE") );
            usersTable->setModel(model);

            // usersTable->setIndexWidget(model->index(2,1), new QPushButton("X") );
            
            connect (usersTable, SIGNAL(clicked(QModelIndex)), SLOT(onUserSelected(QModelIndex)) );

			usersTable->verticalHeader()->hide();
			usersTable->verticalHeader()->setDefaultSectionSize( 30 );
			usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
			usersTable->setFocusPolicy(Qt::NoFocus);

		    usersTable->setColumnWidth(0, 40);
		    usersTable->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch);



    connect( &Socket::sock(), SIGNAL(settingsLoaded( QList<QStandardItem*>, QJsonObject )), SLOT(onSettingsLoaded( QList<QStandardItem*>, QJsonObject )) );
}


void
SettingsView::onAddUser(){

	userConfirmBtn->setText( "Ajouter" );

    userIdLbl->setText( "*" );
	usernameEdt->setText( "" );
    passwordEdt->setText( "" );
    personIdSbx->setValue( 0 );
    roleCbx->setCurrentText( "user" );

	userFormFrame->show();
}



void
SettingsView::onDeleteUser(){
	int row = usersTable->selectionModel()->currentIndex().row();
	QString userId = model->item(row, 0)->text();
	Socket::sock().deleteUser(userId);
	userFormFrame->hide();
	Socket::sock().getParameters();
}

void
SettingsView::onSendUser(){
    QString userId   = userIdLbl->text();
	QString username = usernameEdt->text();
	QString password = passwordEdt->text();
	int personId = personIdSbx->value();
	QString role     = roleCbx->currentText();

    if (userId.size() > 0 && username.size() > 2 && ( password.size() == 0 || password.size() > 3) && personId > 0 && ( role == "admin" || role == "user") ){
		Socket::sock().sendUser( userId, username, password, personId, role );
		Socket::sock().getParameters();
		userFormFrame->hide();
	} else {
		qDebug() << "champs incorrecte";
	}

}

void
SettingsView::onUserSelected(QModelIndex item){

	// disconnect(userConfirmBtn, 0,0,0); // disconnecte koullech
	userConfirmBtn->setText( "Sauveguarder" );
	deleteUserBtn->setDisabled( false );
	
	int row = item.row();
    userIdLbl->setText( model->item(row, 0)->text() );
	usernameEdt->setText(model->item(row, 1)->text() );
    passwordEdt->setText( "" );
    personIdSbx->setValue( model->item(row, 2)->text().toInt() );
    roleCbx->setCurrentText( model->item(row, 3)->text() );

	userFormFrame->show();
}

void
SettingsView::onSettingsLoaded( QList<QStandardItem*> items, QJsonObject settings ){
	model->removeRows(0,model->rowCount());


    const int COLS = 4;
    for (int i=0; i<items.size(); ++i){
    	model->setItem(i / COLS, int(i % COLS), items.at(i) );
    }

    methodBG->button( settings["rec_method"].toString().toInt() )->setChecked( true );
    eigenFaceSbx->setValue( settings["eigenFace_threshold"].toString().toInt() );
    fisherFaceSbx->setValue( settings["fisherFace_threshold"].toString().toInt() );
    LBPHFaceSbx->setValue( settings["LBPHFace_threshold"].toString().toInt() );

}


void
SettingsView::onSaveOptions(){

/*	QJsonArray users_json;

    for (int i=0; i < model->rowCount(); ++i){
        QJsonObject line;

		line["id"]  = model->data( model->index(i, 0) ).toString();
		line["username"]  = model->data( model->index(i, 1) ).toString();
		line["person_id"] = model->data( model->index(i, 2) ).toString();
		line["role"]      = model->data( model->index(i, 3) ).toString();
    }
*/
    qDebug() << "onSaveOptions";
    QJsonObject options_json;
    options_json["rec_method"] = methodBG->checkedId();
    options_json["eigenFace_threshold"] = eigenFaceSbx->value();
    options_json["fisherFace_threshold"] = fisherFaceSbx->value();
    options_json["LBPHFace_threshold"] = LBPHFaceSbx->value();


    Socket::sock().saveOptions(options_json);
}

/*
void
SettingsView::onSaveOptions(){
	qDebug() << "Save Options";

}
*/
