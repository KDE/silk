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
    request.setRawHeader( "User-Agent", "mediawiki-silk" );
    // Send the request
    d->manager->post( request, url.toString().toUtf8() );
    qDebug() << "connect log in";
    connect( d->manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( finishedLogin( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

void Login::abort()
{
    qDebug() << "abort";
    this->setError(this->ConnectionAbort);
    delete d->manager;
    emit result(this);
}

void Login::finishedLogin( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qDebug() << "Request failed, " << reply->errorString();
        this->setError(this->ConnectionAbort);
        emit resultLogin( this );
        return;
    }

    QXmlStreamReader reader( reply );

    while ( !reader.atEnd() && !reader.hasError() )
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement )
        {
            if ( reader.name() == QString( "login" ) )
            {
                QXmlStreamAttributes attrs = reader.attributes();
                if ( attrs.value( QString( "result" ) ).toString() == "Success" )
                {
                    setError(KJob::NoError);
                    emitResult();
                    emit resultToken( this );
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "NeedToken" )
                {
                    d->result.lgtoken = attrs.value( QString( "token" ) ).toString() ;
                    d->result.lgsessionid = attrs.value( QString( "sessionid" ) ).toString() ;
                    emit resultLogin( this );
                }
                else
                {
                    this->setError(this->getError(attrs.value( QString( "result" ) ).toString()));
                    emit resultLogin( this );
                    return;
                }
            }
        }
        else if ( token == QXmlStreamReader::Invalid ){
            this->setError(this->Falsexml);
            emit resultLogin( this );
        }
    }

    QUrl url = d->baseUrl;
    url.addQueryItem("lgtoken", d->result.lgtoken);
    QString data = url.toString();

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", "mediawiki-silk" );
    request.setRawHeader( "Cookie", d->manager->cookieJar()->cookiesForUrl( d->mediawiki.url() ).at( 0 ).toRawForm() );

    // Send the request
    d->manager = new QNetworkAccessManager( this );
    qDebug() << "connect token";
    connect( d->manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedToken( QNetworkReply * ) ) );
    d->manager->post( request, data.toUtf8() );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}


void Login::finishedToken( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qDebug() << "Request failed, " << reply->errorString();
        this->setError(this->ConnectionAbort);
        emit resultToken( this );
        return;
    }

    QXmlStreamReader reader( reply );

    while ( !reader.atEnd() && !reader.hasError() )
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        if ( token == QXmlStreamReader::StartElement )
        {
            if ( reader.name() == QString( "login" ) )
            {
                QXmlStreamAttributes attrs = reader.attributes();
                if ( attrs.value( QString( "result" ) ).toString() == "Success" )
                {
                    qDebug() << "Logged in";
                    setError(KJob::NoError);
                    emitResult();
                    emit resultToken( this );
                }
                else
                {
                    this->setError(this->getError(attrs.value( QString( "result" ) ).toString()));
                    emit resultToken( this );
                    return;
                }
            }
        }
        else if ( token == QXmlStreamReader::Invalid ){
            this->setError(this->Falsexml);
            emit resultToken( this );
        }
    }
}


Login::Result Login::getResults()
{
    return d->result;
}


int Login::getError(const QString & error)
{
    QStringList list;
    list << "NoName"
            <<"Illegal"
            <<"NotExists"
            <<"EmptyPass"
            <<"WrongPass"
            <<"WrongPluginPass"
            <<"CreateBlocked"
            <<"Throttled"
            <<"Blocked"
            <<"mustbeposted"
            <<"NeedToken";
    return list.indexOf(error) + Login::NoName;
}
