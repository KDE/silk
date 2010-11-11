/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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
#include <QtNetwork/QNetworkCookieJar>
#include <QStringList>
#include <QDebug>

#include "login.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct LoginPrivate
    {

        LoginPrivate(QNetworkAccessManager * const manager, const QString &lgname, const QString &lgpassword, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {
            result.lgname = lgname;
            result.lgpassword = lgpassword;
        }

        QNetworkAccessManager *manager;
        QUrl baseUrl;
        MediaWiki const & mediawiki;
        Login::Result result;

    };

}

using namespace mediawiki;

Login::Login( MediaWiki const & media, const QString &lgname, const QString &lgpassword, QObject * parent )
    : KJob(parent)
    , d(new LoginPrivate(new QNetworkAccessManager(this), lgname, lgpassword, media))
{
    setCapabilities(KJob::NoCapabilities);
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
    url.addQueryItem("lgname", d->result.lgname);
    url.addQueryItem("lgpassword", d->result.lgpassword);
    d->baseUrl = url;
    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->post( request, url.toString().toUtf8() );
    connect( d->manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( finishedLogin( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

void Login::abort()
{
    this->setError(this->ConnectionAbort);
    emitResult();
}

void Login::finishedLogin( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->ConnectionAbort);
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
                    this->setError(KJob::NoError);
                    d->result.lgtoken = attrs.value( QString( "lgtoken" ) ).toString() ;
                    d->result.lgsessionid = attrs.value( QString( "lgsessionid" ) ).toString() ;
                    reply->close();
                    reply->deleteLater();
                    emit resultToken( this );
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "NeedToken" ) {
                    this->setError(KJob::NoError);
                    d->result.lgtoken = attrs.value( QString( "token" ) ).toString() ;
                    d->result.lgsessionid = attrs.value( QString( "sessionid" ) ).toString() ;
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
        else if ( token == QXmlStreamReader::Invalid ){
            this->setError(this->Falsexml);
            reply->close();
            reply->deleteLater();
            emitResult();
            return;
        }
    }
    reply->close();
    reply->deleteLater();
    emit resultLogin( this );

    QUrl url = d->baseUrl;
    url.addQueryItem("lgtoken", d->result.lgtoken);
    QString data = url.toString();

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    if(d->manager->cookieJar()->cookiesForUrl( d->mediawiki.url() ).empty()) {
        QNetworkCookie cookie(QString("enwiki_session").toUtf8(),QString(d->result.lgsessionid).toUtf8());
        cookie.setName(QString("test").toUtf8());
        d->manager->cookieJar()->cookiesForUrl( d->mediawiki.url() ).push_back(cookie);
    }
    request.setRawHeader( "Cookie", d->manager->cookieJar()->cookiesForUrl( d->mediawiki.url() ).at( 0 ).toRawForm() );

    // Send the request
    d->manager = new QNetworkAccessManager( this );
    d->manager->post( request, data.toUtf8() );
    connect( d->manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedToken( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}


void Login::finishedToken( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError ) {
        this->setError(this->ConnectionAbort);
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
                this->setError(KJob::NoError);
                QString cookieName = attrs.value( QString( "cookieprefix" ) ).toString();
                d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).value(0).setName(cookieName.toUtf8());
            }
            else if ( reader.name() == QString( "error" ) ) {
                this->setError(this->getError(attrs.value( QString( "code" ) ).toString()));
                reply->close();
                reply->deleteLater();
                emitResult();
                return;
            }
        }
        else if ( token == QXmlStreamReader::Invalid ){
            this->setError(this->Falsexml);
            reply->close();
            reply->deleteLater();
            emitResult();
            return;
        }
    }
    reply->close();
    reply->deleteLater();
    emit resultToken( this );
    emitResult();
}


Login::Result Login::getResults()
{
    return d->result;
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
    return  ret + (int)Login::Falsexml ;
}
