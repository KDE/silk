/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QStringList>
#include <QDebug>

#include "login.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct LoginPrivate
    {
        QUrl baseUrl;
        MediaWiki & mediawiki;
        QString login;
        QString password;
        QString lgsessionid;
        QString lgtoken;

        LoginPrivate(const QString &log, const QString &pass, MediaWiki & mediawiki)
            : mediawiki(mediawiki)
        {
            login = log;
            password = pass;
        }        
    };

}

using namespace mediawiki;

Login::Login( MediaWiki & media, const QString &login, const QString &password, QObject * parent )
    : Job(media, parent)
    , d(new LoginPrivate(login, password, media))
{
    setCapabilities(Job::NoCapabilities);
}



Login::~Login()
{
    delete d;
}

void Login::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Login::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "login");
    url.addQueryItem("lgname", d->login);
    url.addQueryItem("lgpassword", d->password);
    d->baseUrl = url;
    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->mediawiki.manager()->post( request, url.toString().toUtf8() );
    connect( d->mediawiki.manager(), SIGNAL( finished( QNetworkReply * ) ), this, SLOT( doWorkProcessReply( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

void Login::abort()
{
    this->setError(this->ConnectionAborted);
    emitResult();
}

void Login::doWorkProcessReply( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->ConnectionAborted);
        reply->close();
        reply->deleteLater();
        emitResult();
        return;
    }

    QXmlStreamReader reader( reply );
    while ( !reader.atEnd() && !reader.hasError() ) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement ) {
            QXmlStreamAttributes attrs = reader.attributes();
            if ( reader.name() == QString( "login" ) ) {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" ) {
                    //qDebug()<<"Success";
                    this->setError(Job::NoError);
                    d->lgtoken = attrs.value( QString( "lgtoken" ) ).toString() ;
                    d->lgsessionid = attrs.value( QString( "sessionid" ) ).toString() ;
                    if(d->mediawiki.manager()->cookieJar()->cookiesForUrl( d->mediawiki.url() ).isEmpty()) {
                        QList<QNetworkCookie> cookies;
                        QString prefix = attrs.value( QString( "cookieprefix" ) ).toString();

                        QString prefixSession = prefix;
                        prefixSession.append("_session");
                        QNetworkCookie cookie1(prefixSession.toUtf8(),attrs.value( QString( "sessionid" ) ).toString().toUtf8());
                        cookies.append(cookie1);

                        QString prefixUserName = prefix;
                        prefixUserName.append("UserName");
                        QNetworkCookie cookie2(prefixUserName.toUtf8(),attrs.value( QString( "lgusername" ) ).toString().toUtf8());
                        cookies.append(cookie2);

                        QString prefixUserID = prefix;
                        prefixUserID.append("UserID");
                        QNetworkCookie cookie3(prefixUserID.toUtf8(),attrs.value( QString( "lguserid" ) ).toString().toUtf8());
                        cookies.append(cookie3);

                        QString prefixToken = prefix;
                        prefixToken.append("Token");
                        QNetworkCookie cookie4(prefixToken.toUtf8(),attrs.value( QString( "lgtoken" ) ).toString().toUtf8());
                        cookies.append(cookie4);

                        d->mediawiki.manager()->cookieJar()->setCookiesFromUrl(cookies, d->mediawiki.url());
                    }
                    reply->close();
                    reply->deleteLater();
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "NeedToken" ) {
                    this->setError(Job::NoError);
                    //qDebug()<<"Need Token";
                    d->lgtoken = attrs.value( QString( "token" ) ).toString() ;
                    d->lgsessionid = attrs.value( QString( "sessionid" ) ).toString() ;
                    if(d->mediawiki.manager()->cookieJar()->cookiesForUrl( d->mediawiki.url() ).isEmpty()) {
                        QString prefix = attrs.value( QString( "cookieprefix" ) ).toString();
                        prefix.append("_session");
                        QNetworkCookie cookie(prefix.toUtf8(),QString(d->lgsessionid).toUtf8());
                        QList<QNetworkCookie> cookies;
                        cookies.append(cookie);
                        d->mediawiki.manager()->cookieJar()->setCookiesFromUrl(cookies, d->mediawiki.url());
                    }
                }
                else{
                    this->setError(this->getError(attrs.value( QString( "result" ) ).toString()));
                    reply->close();
                    reply->deleteLater();
                    emitResult();
                    return;
                }
            }
            else if ( reader.name() == QString( "error" ) ) {
                this->setError(this->getError(attrs.value( QString( "code" ) ).toString()));
                reply->close();
                reply->deleteLater();
                emitResult();
                return;
            }
        }
        else if ( token == QXmlStreamReader::Invalid && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError){
            this->setError(this->BadXml);
            reply->close();
            reply->deleteLater();
            emitResult();
            return;
        }
    }
    reply->close();
    reply->deleteLater();

    QUrl url = d->baseUrl;
    url.addQueryItem("lgtoken", d->lgtoken);
    QString data = url.toString();

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader( "Cookie", d->mediawiki.manager()->cookieJar()->cookiesForUrl( d->mediawiki.url() ).at( 0 ).toRawForm() );
    // Send the request
    d->mediawiki.manager()->post( request, data.toUtf8() );
    connect( d->mediawiki.manager(), SIGNAL( finished( QNetworkReply* ) ), this, SLOT( doWorkProcessReply( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

int Login::getError(const QString & error)
{
    int ret = 0;
    QStringList list;
    list << "Falsexml"
            << "ConnectionAbort"
            << "NoName"
            << "Illegal"
            << "NotExists"
            << "EmptyPass"
            << "WrongPass"
            << "WrongPluginPass"
            << "CreateBlocked"
            << "Throttled"
            << "Blocked"
            << "NeedToken";
    ret = list.indexOf(error);
    if(ret == -1){
        ret = 0;
    }
    return  ret + (int)Login::BadXml ;
}
