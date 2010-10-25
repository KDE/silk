#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QTimer>
#include <QDebug>
#include <QNetworkCookieJar>


#include "login.h"

struct LoginPrivate {
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl apiUrl;
    QUrl baseUrl;
    QString lgname;
    QString lgpassword;
    QString lgtoken;
    QString lgsessionid;
};

Login::Login(MediaWiki const & media, const QString &login, const QString &password, QObject * parent)
    : QObject(parent)
    , d(new LoginPrivate)
{
//    qDebug()<< "Login" ;
    d->apiUrl = media.url();
    QUrl url = d->apiUrl;
    QString data = "action=login&lgname="+login+"&lgpassword="+password+"&format=xml";
    d->baseUrl=data;

//    qDebug() << "Constructed login URL" << url.toString();
//    qDebug() << "Constructed login postData" << data;

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "mediawiki-silk");
    // Send the request
    d->manager = new QNetworkAccessManager(this);

//    qDebug()<< "d->reply avant";
    connect( d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedLogin(QNetworkReply *)) );
    d->reply = d->manager->post( request, data.toUtf8() );

//    qDebug()<< "d->reply apres connect";
//    qDebug()<< "Fin Login\n" ;
}

Login::~Login()
{
    delete d;
}

void Login::finishedLogin( QNetworkReply *reply )
{
//    qDebug()<< "finishedLogin" << reply->error();

    if (reply->error() == QNetworkReply::NoError) {
//        qDebug()<< "pas d erreur";
        QXmlStreamReader reader(reply);

        while(!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartElement) {
                if(reader.name() == QString("login")) {
//                    qDebug()<<reader.name().toString();
                    QXmlStreamAttributes attrs = reader.attributes();

                    //TODO : tester l'attribut result si success alors pas de traitement sinon traitement
                    if(attrs.value( QString("result") ).toString() == "Success")
                    {
                        emit finishedLogin( true );
                        return;
                    }
                    else if(attrs.value( QString("result") ).toString() == "NeedToken")
                    {
                        d->lgtoken= attrs.value( QString("result") ).toString() ;
//                        qDebug() << "tagattribute result"<< d->lgtoken;

                        d->lgtoken= attrs.value( QString("token") ).toString() ;
//                        qDebug() << "tagattribute token"<< d->lgtoken;

                        d->lgsessionid = attrs.value( QString("sessionid") ).toString() ;
//                        qDebug() << "tagattribute sessionid"<< d->lgsessionid;
                        emit finishedLogin( true );
                    }
                }
            }
            else if ( token == QXmlStreamReader::Invalid )
                emit finishedLogin( false );
        }

//        qDebug()<< "hors de la boucle";

        QUrl url = d->apiUrl;
        QString data = d->baseUrl.toString();
//        data+="&lgtoken="+d->lgtoken;

//        qDebug() << "Constructed login URL" << url.toString();
//        qDebug() << "Constructed login postData" << data;

        // Set the request
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", "mediawiki-silk");
        request.setRawHeader("Cookie", d->manager->cookieJar()->cookiesForUrl(d->apiUrl).at(0).toRawForm());

//        qDebug()<<"cookie "<<d->manager->cookieJar()->cookiesForUrl(d->apiUrl).at(0).toRawForm();

        // Send the request
        d->manager = new QNetworkAccessManager(this);

//        qDebug()<< "d->reply avant";
        connect( d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedToken(QNetworkReply *)) );
        d->reply = d->manager->post( request, data.toUtf8() );

//        qDebug()<< "d->reply apres connect";
//        qDebug()<< "Fin finishedLogin\n" ;
    }
}


void Login::finishedToken( QNetworkReply *reply )
{
//    qDebug()<< "finishedToken" << reply->error();

    if (reply->error() == QNetworkReply::NoError) {
//        qDebug()<< "pas d erreur";
        QXmlStreamReader reader(reply);

        while(!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartElement) {
                if(reader.name() == QString("login")) {
//                    qDebug()<<reader.name().toString();
                    QXmlStreamAttributes attrs = reader.attributes();
                    if(attrs.value( QString("result") ).toString() == "Success")
                    {

                        d->lgtoken= attrs.value( QString("result") ).toString() ;
//                        qDebug() << "tagattribute result"<< d->lgtoken;
                        emit finishedToken( true );
                    }
                }
            }
            else if ( token == QXmlStreamReader::Invalid )
                emit finishedToken( false );
        }
//        qDebug()<< "hors de la boucle";
//        qDebug()<< "Fin finishedToken\n" ;
    }
}
