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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QTimer>
#include <QDebug>
#include <QNetworkCookieJar>

#include "login.h"
#include "mediawiki.h"

struct LoginPrivate
{
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl apiUrl;
    QUrl baseUrl;
    QString lgname;
    QString lgpassword;
    QString lgtoken;
    QString lgsessionid;
};

Login::Login( MediaWiki const & media, const QString &login, const QString &password, QObject * parent )
        : QObject( parent )
        , d( new LoginPrivate )
{

    d->apiUrl = media.url();
    QUrl url = d->apiUrl;
    QString data = "action=login&lgname=" + login + "&lgpassword=" + password + "&format=xml";
    d->baseUrl = data;

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", "mediawiki-silk" );
    // Send the request
    d->manager = new QNetworkAccessManager( this );

    qDebug() << "connect log in";
    connect( d->manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedLogin( QNetworkReply * ) ) );
    d->reply = d->manager->post( request, data.toUtf8() );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}

Login::~Login()
{
    delete d;
}

void Login::abort()
{
    qDebug() << "abort";
    if ( !d->reply )
        return;

    d->reply->abort();
    d->reply = 0;
}

void Login::finishedLogin( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qDebug() << "Request failed, " << reply->errorString();
        emit finishedLogin( false );
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
                    emit finishedLogin( true );
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "NeedToken" )
                {
                    d->lgtoken = attrs.value( QString( "result" ) ).toString() ;

                    d->lgtoken = attrs.value( QString( "token" ) ).toString() ;

                    d->lgsessionid = attrs.value( QString( "sessionid" ) ).toString() ;
                    emit finishedLogin( true );
                }
                else
                {
                    emit finishedLogin( false );
                    return;
                }
            }
        }
        else if ( token == QXmlStreamReader::Invalid )
            emit finishedLogin( false );
    }

    QUrl url = d->apiUrl;
    QString data = d->baseUrl.toString();
    data += "&lgtoken=" + d->lgtoken;


    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader( "User-Agent", "mediawiki-silk" );
    request.setRawHeader( "Cookie", d->manager->cookieJar()->cookiesForUrl( d->apiUrl ).at( 0 ).toRawForm() );


    // Send the request
    d->manager = new QNetworkAccessManager( this );
    qDebug() << "connect token";
    connect( d->manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedToken( QNetworkReply * ) ) );
    d->reply = d->manager->post( request, data.toUtf8() );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );

}


void Login::finishedToken( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qDebug() << "Request failed, " << reply->errorString();
        emit finishedToken( false );
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
                    d->lgtoken = attrs.value( QString( "result" ) ).toString() ;
                    emit finishedToken( true );
                }
                else
                {
                    emit finishedToken( false );
                    return;
                }
            }
        }
        else if ( token == QXmlStreamReader::Invalid )
            emit finishedToken( false );
    }
}
