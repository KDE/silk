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
#include <QtCore/QCryptographicHash>
#include <QtNetwork/QNetworkCookieJar>
#include <QStringList>
#include <QDebug>
#include <Q3Url>

#include "edit.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct EditPrivate
    {
        EditPrivate(QNetworkAccessManager *  manager, MediaWiki  & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki){}

        QNetworkAccessManager *manager;
        QUrl baseUrl;
        MediaWiki  & mediawiki;
        QMap<QString, QString> requestParameter;
        Edit::Result result;
    };
}

using namespace mediawiki;

Edit::Edit( MediaWiki  & media, QObject *parent)
    : MediaWikiJob(media,parent)
    , d(new EditPrivate(new QNetworkAccessManager(this), media))
{
}
void Edit::setUndoAfter( int param )
{
    d->requestParameter["undoafter"] = QString::number(param);
}

void Edit::setUndo( int param )
{
    d->requestParameter["undo"] = QString::number(param);
}

void Edit::setPrependText( const QString& param )
{
    d->requestParameter["prependtext"] = param;
}

void Edit::setAppendText( const QString& param )
{
    d->requestParameter["appendtext"] = param;
}

void Edit::setPageTitle( const QString& param )
{
    d->requestParameter["title"] = param;
}

void Edit::setToken( const QString& param )
{
    d->requestParameter["intoken"] = param;
}
void Edit::setBaseTimesStamp( const QDateTime& param )
{
    d->requestParameter["basetimestamp"] = param.toString("yyyy-MM-ddThh:mm:ssZ");
}

void Edit::setStartTimesStamp( const QDateTime& param )
{
    d->requestParameter["starttimestamp"] = param.toString("yyyy-MM-ddThh:mm:ssZ");
}

void Edit::setText( const QString& param )
{
    d->requestParameter["text"] = param;
}

void Edit::setRecreate(bool param)
{
    if(param)
        d->requestParameter["recreate"] = "on";
}

void Edit::setCreateonly(bool param)
{
    if(param)
        d->requestParameter["createonly"] = "on";
}

void Edit::setNocreate(bool param)
{
    if(param)
        d->requestParameter["nocreate"] = "on";
}

void Edit::setMinor(bool minor)
{
    if(minor)
        d->requestParameter["minor"] = "on";
    else
        d->requestParameter["notminor"] = "on";
}
void Edit::setSection(const QString& section)
{
    d->requestParameter["section"] = section;
}

void Edit::setSummary(const QString & summary)
{
    d->requestParameter["summary"] = summary;
}

Edit::~Edit()
{
    delete d;
}

void Edit::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Edit::doWorkSendRequest()
{
    // Set the url
    QString url = d->mediawiki.url().toString();
    url.append("?format=xml&action=edit");
    // Add params
    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext()) {
        i.next();
        if(i.key() != "token") {
            QString param("&");
            param += i.key();
            if(i.key() != "recreate" && i.key() != "createonly" && i.key() != "recreate" && i.key() != "nocreate" && i.key() != "notminor" && i.key() != "minor") {
                param += "=";
                param += i.value();
            }
            url += param;
        }
    }
    QByteArray cookie = "";
    for(int i = 0 ; i<d->mediawiki.cookies().size();i++){
        cookie += d->mediawiki.cookies().at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }
    QUrl urlEncoded(url, QUrl::TolerantMode);
    // Add the token
    QString token = d->requestParameter["token"];
    token.replace("+", "%2B");
    token.replace("\\", "%5C");
    urlEncoded.addQueryItem(QString("token"), token);
    d->baseUrl = urlEncoded;
    // Set the request
    QNetworkRequest request( urlEncoded );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader( "Cookie", cookie );
    // Send the request
    d->mediawiki.manager()->post( request, urlEncoded.toString().toUtf8() );
    connect( d->mediawiki.manager(), SIGNAL( finished( QNetworkReply * ) ), this, SLOT( finishedEdit( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

void Edit::abort()
{
    this->setError(this->ConnectionAborted);
    emitResult();
}

void Edit::finishedEdit( QNetworkReply *reply )
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
            if ( reader.name() == QString( "edit" ) ) {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" ) {
                    this->setError(KJob::NoError);
                    reply->close();
                    reply->deleteLater();
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "Failure" ) {
                    this->setError(KJob::NoError);
                    reader.readNext();
                    attrs = reader.attributes();
                    d->result.captchaId = attrs.value( QString( "id" ) ).toString().toUInt() ;
                    if (!attrs.value( QString( "question" ) ).isEmpty())
                        d->result.captchaQuestion = QVariant(attrs.value( QString( "question" ) ).toString()) ;
                    else if (!attrs.value( QString( "url" ) ).isEmpty())
                        d->result.captchaQuestion = QVariant(attrs.value( QString( "url" ) ).toString()) ;
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
    emit resultCaptcha( d->result.captchaQuestion );
}

void Edit::finishedCaptcha( const QString & captcha )
{
    d->result.captchaAnswer = captcha;
    QUrl url = d->baseUrl;
    url.addQueryItem("CaptchaId", QString::number(d->result.captchaId));
    url.addQueryItem("CaptchaAnswer", d->result.captchaAnswer);
    QString data = url.toString();
    QByteArray cookie = "";
    for(int i = 0 ; i<d->mediawiki.cookies().size();i++){
        cookie += d->mediawiki.cookies().at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader( "Cookie", cookie );
    // Send the request
    d->mediawiki.manager()->post( request, data.toUtf8() );
    connect( d->mediawiki.manager(), SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedEdit( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

int Edit::getError(const QString & error)
{
    QString temp = error;
    int ret = 0;
    QStringList list;
    list << "Falsexml"
            << "ConnectionAbort"
            << "notext"
            << "invalidsection"
            << "protectedtitle"
            << "cantcreate"
            << "cantcreateanon"
            << "articleexists"
            << "noimageredirectanon"
            << "noimageredirect"
            << "spamdetected"
            << "filtered"
            << "contenttoobig"
            << "noeditanon"
            << "noedit"
            << "pagedeleted"
            << "emptypage"
            << "emptynewsection"
            << "editconflict"
            << "revwrongpage"
            << "undofailure";
    ret = list.indexOf(temp.remove(QChar('-')));
    if(ret == -1){
        ret = 0;
    }
    return  ret + (int)Edit::BadXml ;
}
