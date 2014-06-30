#ifndef SOCKET_H
#define SOCKET_H

#include "../core.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QQueue>

#include <QThread>

static QString host = "";
static uint port = 3886;


class Socket: public QTcpSocket{
    Q_OBJECT

public:
    Socket(const Socket&);                 // Prevenir la construction par copy
    Socket& operator=(const Socket&);      // Prevenir l'assignation
    static Socket& sock(){
        static Socket _instance = Socket();
        return _instance;
    }


    void
    login(QString serverIp, QString username, QString password, cv::Mat face = cv::Mat() ){

        disconnectFromHost();
        host = serverIp;

        //SERIALIZE
        QJsonObject data;
        data["username"] = username;
        data["password"] = password;

        cv::Mat gray;
        if( face.rows != 0 && face.cols != 0 ){
            cvtColor(face, gray, CV_BGR2GRAY);
            equalizeHist(gray, gray);
        }

        transmit("AUTH", data, MatToQByteArray( gray ) );
    }

   

    void
    addPerson(QJsonObject data, QList<QImage> images){
        qDebug() << "Socket::addPerson";
        QByteArray imgsArray;
        foreach( QImage img, images){
            imgsArray.append( MatToQByteArray( QImage2Mat(img) ) );
            imgsArray.append( "<smoky-image>" );
        }
        transmit("ADDPERSON", data, imgsArray );
    }

    void
    is(int faceId, int personId){
        QJsonObject data;
        data["faceId"] = faceId;
        data["personId"] = personId;

        transmit("IS", data);
    }

    void
    get(int personId){
        QJsonObject data;
        data["personId"] = personId;

        transmit("GET", data);
    }


    void
    getInfo(int personId){
        QJsonObject data;
        data["personId"] = personId;

        transmit("GETINFO", data);
    }


    void
    recognize( cv::Mat img, int imageId, int faceId ){
        // (int rows, int cols, int type, void* data, size_t step=AUTO_STEP)
        QJsonObject data;

   
        data["imageId"] = imageId;
        data["faceId"] = faceId;
        
        transmit("REC", data, MatToQByteArray( img ) );
    }

    void
    deleteUser(QString userId){
        QJsonObject data;
        data["userId"] = userId;
        
        transmit("DELETEUSER", data );
    }

    void
    getParameters(){
        QJsonObject data;
        transmit("GETSETTINGS", data );
    }

    void
    sendUser(QString userId, QString username, QString password, int personId, QString role){
        QJsonObject data;
        data["userId"] = userId;
        data["username"] = username;
        data["password"] = password;
        data["personId"] = personId;
        data["role"] = role;
        
        transmit("SENDUSER", data );
    }


    void
    saveOptions(QJsonObject data ){
        transmit("SAVEOPTIONS", data);
    }

    void
    getStats(){
       QJsonObject data;

       transmit("STATS", data);
    }


    void
    getHistory(QString username = "", int op_id = 0, QString op_date = "", int page = 1){
        QJsonObject data;
        data["username"] = username;
        data["op_id"] = op_id;
        data["op_date"] = op_date;
        data["page"] = page;

        transmit("HIST", data );
    }

    void
    launchSearch(QJsonObject data){

        transmit( "SEARCH", data );
    }

public slots:

    void
    refute(QList<int> ratio){
        QJsonObject data;
        data["eigen_ratio"]  = ratio[0];
        data["fisher_ratio"] = ratio[1];
        data["lbph_ratio"]   = ratio[2];
        transmit( "REFUTE", data );
    }

    void
    confirm(int personId, QByteArray newFace, QList<int> ratio){
        QJsonObject data;
        data["personId"]     = personId;
        data["eigen_ratio"]  = ratio[0];
        data["fisher_ratio"] = ratio[1];
        data["lbph_ratio"]   = ratio[2];
        
        qDebug() << "CONFIRM" << personId << "facesize" << newFace.size();
        transmit( "CONFIRM", data, newFace );
    }

    void
    logout(){
        qDebug() << "DISC SENT";
        transmit("DISC");
        
    }

    void
    log(int op_id, QString op_data = ""){
        qDebug() << "log event";
        QJsonObject data;
        data["op_id"] = op_id;
        data["op_data"] = op_data;
        
        transmit("LOG", data);
    }

    void
    update(QJsonObject data){

       transmit("UPDATE", data);
    }

private slots:


    void
    onReadyRead()
    {
        static int n = 0;
        QByteArray bytes = readAll();

        // qDebug() << "received: " << bytes.size() << "octets";

        /// SAVE FOR DEBUG
        QFile file( QString( "paquets/bytes%1.dat" ).arg(n++) );
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);   // we will serialize the data into the file
        out << bytes;

        // qDebug() << QString( "saved in paquets/bytes%1.dat" ).arg(n);


        QList<QByteArray> parts = baSplit(bytes, "<smoky-paquet>", true);
        // qDebug() << "number of parts received: " << parts.size();


        QList<QByteArray> paquets;
        foreach( QByteArray part, parts){

            // qDebug() << "part size: " << part.size();

            /// SAVE FOR DEBUG
            QFile file( QString( "paquets/part%1.dat" ).arg(n++) );
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);   // we will serialize the data into the file
            out << part;

            // qDebug() << QString( "saved in paquets/part%1.dat" ).arg(n);

            if ( part.endsWith("<smoky-paquet>") ){

                // m_currentPaquet.append( part.mid( 0, part.size() - 14 ) ); // size of "<smoky-paquet>"
                m_currentPaquet.append( part ); // size of "<smoky-paquet>"
                paquets << m_currentPaquet;
                m_currentPaquet = QByteArray();

            } else {
                m_currentPaquet.append(part);
            }
        }

        // qDebug() << "number of paquets reconstructed: " << paquets.size();

        
        foreach (QByteArray paquet, paquets){

            // qDebug() << "paquet size: " << paquet.size();

            /// SAVE FOR DEBUG
            QFile file( QString( "paquets/paquet%1.dat" ).arg(n++) );
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);   // we will serialize the data into the file
            out << paquet;

            // qDebug() << QString( "saved in paquets/paquet%1.dat" ).arg(n);


            try {
                QList<QByteArray> lines = baSplit( paquet, "<smoky-break>" );
                if ( lines.size() < 3 )
                    return;

                QByteArray query = lines[0].trimmed().toLower();

                QJsonDocument data;
                data = QJsonDocument::fromJson( lines[1].trimmed() );

                QByteArray raw = lines[2];

                qDebug() << "\nQuery: " << query;
                qDebug() << "json_str: " << lines[1];
                qDebug() << "raw size: " << raw.size() << "\n";

                QList<QByteArray> images = baSplit( raw, "<smoky-image>" );
                // qDebug() << images.size() << "images sent !";

                /**
                *  AUTH answer
                */
                if ( query == "auth" ){
                    qDebug() << data;
                    int user_id = data.object()["user_id"].toInt();
                    _token = data.object()["token"].toString();
                    _firstName = data.object()["firstName"].toString();
                    _role = data.object()["role"].toString();

                    if ( _token != "0"){
                        emit userLoggedIn(_firstName);
                        emit clientEvent(1);
                        if (_role == "admin"){
                            qDebug() << user_id;
                            emit loggedAsAdmin();
                            if (user_id == -2) {// temp account
                                emit loggedAsTempAdmin();
                            }
                        } else {
                            emit loggedAsUser();
                        }

                    } else {
                        qDebug() << "DISC received!";
                        emit userLoggedOut();
                        disconnectFromHost();
                        _token = "0";
                    }
                }


                /**
                *  REC answer
                */
                if ( query == "rec" ){
                    int faceId = data.object()["faceId"].toInt();
                    int imageId = data.object()["imageId"].toInt();
                    int personId = data.object()["personId"].toInt();

                    int eigen_ratio = data.object()["eigen_ratio"].toInt();
                    int fisher_ratio = data.object()["fisher_ratio"].toInt();
                    int lbph_ratio = data.object()["lbph_ratio"].toInt();

                    emit faceRecognized(imageId, faceId, personId, eigen_ratio, fisher_ratio, lbph_ratio);
                }


                /**
                *  IS answer
                */
                if ( query == "is" ){
                    int faceId = data.object()["faceId"].toInt();
                    QString firstName = data.object()["firstName"].toString();
                    QString lastName = data.object()["lastName"].toString();
                    emit faceIs(faceId, firstName, lastName);
                }


                /**
                *  SETTINGS answer
                */
                if ( query == "getsettings" ){
                    QJsonObject parameters = data.object()["parameters"].toObject();
                    QJsonArray users = data.object()["users"].toArray();

                    QList<QStandardItem*> items;
                    foreach( QJsonValue row, users ){
                        QJsonObject line = row.toObject();
                        items << new QStandardItem( line["id"].toString() );
                        items << new QStandardItem( line["username"].toString() );
                        items << new QStandardItem( line["person_id"].toString() );
                        items << new QStandardItem( line["role"].toString() );
                    }

                    emit settingsLoaded( items, parameters );
                }


                /**
                 * STATS answer
                 */
                if ( query == "stats" ){
                    qDebug() << "stats received";
                    emit statsLoaded( data.object() );
                }

                /**
                 * STATS PERSONADDED
                 */
                if ( query == "personadded" ){
                    qDebug() << "personadded";
                    emit personAdded();
                }

                if (query == "dberror"){
                    emit connexionError(5);
                }


                /**
                *  GET answer
                */
                if ( query == "get" ){
                    QJsonObject infos = data.object();
                    if (images.size() > 0)
                    emit got( infos, images[0] );
                    qDebug() << "GOT received" << infos;
                }


                if ( query == "getinfo" ){
                    QJsonObject infos = data.object();
                    if (images.size() > 0){
                        qDebug() << infos;
                        emit gotPersonInfo( infos, images[0] );
                    }
                }

                /**
                *  HIST answer
                */
                if ( query == "hist" ){

                    QMap<QString, QString> op;
                    op["1"] = "Connection";
                    op["2"] = "Deconnection";

                    op["3"] = "Ajout de l'utilisateur";
                    op["4"] = "Mise a jour de l'utilisateur";
                    op["5"] = "Suppression de l'utilisateur";

                    op["6"] = "Ajout de la personne #";
                    op["7"] = "Suppression de la personne";
                    op["8"] = "Modification des information de #";
                    op["9"] = "Recherche";
                    op["10"] = "Recherche par image";
                    op["11"] = "Consulation de la fiche de l'individu #";


                    QList<QStandardItem*> items;
                    foreach( QJsonValue row, data.array() ){
                        QJsonObject line = row.toObject();
                        items << new QStandardItem( line["username"].toString() );
                        items << new QStandardItem( QString( "%1 %2" ).arg( op[ line["op_id"].toString() ] ).arg( line["op_data"].toString() ) );
                        items << new QStandardItem( line["op_date"].toString() );
                    }

                    emit historyLoaded( items );

                }

                /**
                *  SEARCH answer
                */
                if ( query == "search" ){

                    QList<QJsonObject> items;

                    foreach( QJsonValue row, data.array() ){
                        items << row.toObject();
                    }

                    emit search( items, images );
                }

                // disconnectFromHost();
            } catch(  std::exception& e ){
                qDebug() << "exception !!!";
            }
        }
    }

    

    void transmit(QString query){
        transmit(query, QJsonObject(), QByteArray() );
    }

    void transmit(QString query, QJsonArray jsonArray, QByteArray rawBytes= QByteArray() ){
        transmit(query, QJsonDocument( jsonArray ), rawBytes);
    }

    void transmit(QString query, QJsonObject jsonObject, QByteArray rawBytes= QByteArray() ){
        transmit(query, QJsonDocument( jsonObject ), rawBytes);
    }

    void transmit(QString query, QJsonDocument jsonDoc, QByteArray rawBytes= QByteArray() ){

        TcpConnect(); // if not connected

        QByteArray paquet;
        if ( query.toLower() == "auth")
            _token = "0";
        paquet.append( _token );
        paquet.append( "<smoky-break>" );
        paquet.append( query );
        paquet.append( "<smoky-break>" );
        paquet.append( jsonDoc.toJson( QJsonDocument::Compact ) );
        paquet.append( "<smoky-break>" );
        write( paquet );


        if (rawBytes.size() > 0){
            int i = 0;
            while( 1 ){
                QByteArray bytes = getPart(rawBytes, i++);
                if( bytes.size() )
                    write( bytes );
                else
                    break; 
            }
        }

        write("<smoky-paquet>");
    }

    void 
    onConnected() {
        attempt("connect", true);
        // login( host, "riless", "pass", cv::Mat() );
    }

    void
    onUserLoggedIn(){
        // qDebug() << "Logged In:" << _token;
    }
    
    void
    onUserLoggedOut(){
        _token = "0";
        qDebug() << "Logged out:";
    }

    QList<QByteArray>
    baSplit( QByteArray bytes, QByteArray sep, bool keepSep = false){
        int begin = 0;
        int len = -1;
        QList<QByteArray> parts;
        // stringulac
        // 0     678     456
        while( 1 ){
            int end = bytes.indexOf(sep, begin); // -1
            if (end - begin <= -1){
                len = -1;
            } else {
                int supp = ( keepSep ) ? sep.size(): 0;
                len = end - begin + supp; // -1
            }

            parts << bytes.mid( begin, len );
            if( end > -1 ){
                begin = end + sep.size(); //16
            } else {
                return parts;
            }
        }
    }

    bool
    attempt(QString action, bool reset = false){
        if ( !actions.contains(action) || reset ){
            // qDebug() << action << ": now is 10 ";
            actions[action] = 3;
        } else {
            actions[action]--;
            // qDebug() << action << ": now is" << actions[action];
        }

        return ( actions[action] > 0 );
    }

    void 
    onDisconnected()
    {
      /*  if ( attempt("connect") )
            TcpConnect()*/;
    }


    void
    onError( QAbstractSocket::SocketError error )
    {
        switch (error)
        {
            case SocketTimeoutError:
                // TODO look, really we should store at least one start message forever
                // then if last time we didn't connect and this time it's a pause we 
                // send the start first
                qWarning() << "TimeOut";
                emit connexionError(1);
                m_msgQueue.clear();
                break;
            
            case RemoteHostClosedError:
                qWarning() << "Remote Host Closed";
                emit connexionError(2);
                // expected
                break;

            case ConnectionRefusedError: // happens if client isn't running
                qWarning() << "connection refused";
                emit connexionError(3);
                break;
            default: // may as well
                emit connexionError(4);
                qWarning() << "unknown connexion error";
        }
    }

signals:
    void connexionError(int);
    void personAdded();
    void clientEvent(int); 
    void userLoggedIn(QString firstName);
    void userLoggedOut();
    void invalidCred();
    void serverError();
    void settingsLoaded( QList<QStandardItem*>, QJsonObject);
    void historyLoaded(const QList<QStandardItem *>);
    void faceRecognized(int, int, int, int, int, int);
    void faceIs(int, QString, QString);
    void got( QJsonObject, QByteArray );
    void gotPersonInfo( QJsonObject, QByteArray );
    void loggedAsUser();
    void loggedAsAdmin();
    void loggedAsTempAdmin();
    void search(QList<QJsonObject> items, QList<QByteArray>);
    void statsLoaded(QJsonObject);

private:
    Socket(QObject *parent = 0): QTcpSocket( parent ), _token("0"){

        connect( this, SIGNAL(readyRead()), SLOT(onReadyRead()) );
        connect( this, SIGNAL(error( QAbstractSocket::SocketError )), SLOT(onError( QAbstractSocket::SocketError )) );
        connect( this, SIGNAL(connected()), SLOT(onConnected()) );
        connect( this, SIGNAL(disconnected()), SLOT(onDisconnected()) );
        connect( this, SIGNAL(userLoggedIn(QString)), SLOT(onUserLoggedIn()) );
        connect( this, SIGNAL(userLoggedOut()), SLOT(onUserLoggedOut()) );
        // TcpConnect();

    }

    QByteArray getPart(QByteArray array, int partNum, int partSize = 1024){
        return array.mid( partNum * partSize, partSize);
    }

   
    void
    TcpConnect(){
        if ( state() == QAbstractSocket::UnconnectedState)
            connectToHost( host, port );

        else
            onConnected();
    }

    QHash<QString, int> actions;
    QString _token;
    QString _userId;
    QString _firstName;
    QString _role;
    QByteArray m_currentPaquet;
    QQueue<QString> m_msgQueue;


};


#endif // SOCKET_H
