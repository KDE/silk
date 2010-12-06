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
#include <QtCore/QCryptographicHash>
#include <QStringList>
#include <QDebug>
#include <Q3Url>

#include "edit.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct EditPrivate
    {
        EditPrivate(QNetworkAccessManager * const manager, QString const & title, QString const & token, QString const & basetimestamp, QString const & starttimestamp, QString const & text, QString const & section, QString const & summary, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {
            requestParameter["title"] = title;
            requestParameter["token"] = token;
            requestParameter["starttimestamp"] = starttimestamp;
            requestParameter["basetimestamp"] = basetimestamp;
            requestParameter["text"] = text;
            requestParameter["section"] = section;
            requestParameter["summary"] = summary;
            QByteArray hash = QCryptographicHash::hash(text.toUtf8(),QCryptographicHash::Md5);
            requestParameter["md5"] = hash.toHex();
        }

        EditPrivate(QNetworkAccessManager * const manager, QString const & title, QString const & token, QString const & basetimestamp, QString const & starttimestamp, QString const & appendtext, QString const & prependtext, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {
            requestParameter["title"] = title;
            requestParameter["token"] = token;
            requestParameter["starttimestamp"] = starttimestamp;
            requestParameter["basetimestamp"] = basetimestamp;
            requestParameter["appendtext"] = appendtext;
            requestParameter["prependtext"] = prependtext;
            QString text = prependtext+appendtext;
            QByteArray hash = QCryptographicHash::hash(text.toUtf8(),QCryptographicHash::Md5);
            requestParameter["md5"] = hash.toHex();
        }

        EditPrivate(QNetworkAccessManager * const manager, QString const & title, QString const & token, QString const & basetimestamp, QString const & starttimestamp, unsigned int undo, unsigned int undoafter, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {
            requestParameter["title"] = title;
            requestParameter["token"] = token;
            requestParameter["starttimestamp"] = starttimestamp;
            requestParameter["basetimestamp"] = basetimestamp;
            requestParameter["undo"] = QString::number(undo);
            requestParameter["undoafter"] = QString::number(undoafter);
        }

        QNetworkAccessManager *manager;
        QUrl baseUrl;
        MediaWiki const & mediawiki;
        QMap<QString, QString> requestParameter;
        Edit::Result result;
    };
}

using namespace mediawiki;

Edit::Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString const & text, QString const & section, QString const & summary, QObject *parent)
    : KJob(parent)
    , d(new EditPrivate(new QNetworkAccessManager(this), title, token, basetimestamp, starttimestamp, text, section, summary, media))
{
    setCapabilities(KJob::NoCapabilities);
}

Edit::Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString const & appendtext, QString const & prependtext, QObject *parent)
    : KJob(parent)
    , d(new EditPrivate(new QNetworkAccessManager(this), title, token, basetimestamp, starttimestamp, appendtext, prependtext, media))
{
    setCapabilities(KJob::NoCapabilities);
}

Edit::Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString, unsigned int undo, unsigned int undoafter, QObject *parent)
    : KJob(parent)
    , d(new EditPrivate(new QNetworkAccessManager(this), title, token, basetimestamp, starttimestamp, undo, undoafter, media))
{
    setCapabilities(KJob::NoCapabilities);
}

void Edit::setParams(Edit::Watchlist watchlist, bool recreate, bool createonly,bool nocreate, bool minor, bool notminor)
{
    switch(watchlist) {
    case Edit::watch:
        d->requestParameter["watchlist"] = QString("watch");
        break;
    case Edit::unwatch:
        d->requestParameter["watchlist"] = QString("unwatch");
        break;
    case Edit::nochange:
        d->requestParameter["watchlist"] = QString("nochange");
        break;
    case Edit::preferences:
        d->requestParameter["watchlist"] = QString("preferences");
        break;
    }
    if(recreate)
        d->requestParameter["recreate"] = QString();
    if(createonly)
        d->requestParameter["createonly"] = QString();
    if(nocreate)
        d->requestParameter["nocreate"] = QString();
    if(minor)
        d->requestParameter["minor"] = QString();
    if(notminor)
        d->requestParameter["notminor"] = QString();
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
            if(i.key() != "recreate" && i.key() != "createonly" && i.key() != "recreate" && i.key() != "nocreate" && i.key() != "notminor") {
                param += "=";
                param += i.value();
            }
            url += param;
        }
    }
    QUrl urlEncoded(url, QUrl::TolerantMode);
    // Add the token
    QString token = d->requestParameter["token"];
    token.replace("+", "%2B");
    token.replace("\\", "%5C");
    urlEncoded.addEncodedQueryItem(QString("token").toUtf8(), token.toUtf8());
    d->baseUrl = urlEncoded;
    // Set the request
    QNetworkRequest request( urlEncoded );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // Send the request
    d->manager->post( request, urlEncoded.toString().toUtf8() );
    connect( d->manager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( finishedEdit( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

void Edit::abort()
{
    this->setError(this->ConnectionAbort);
    emitResult();
}

void Edit::finishedEdit( QNetworkReply *reply )
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
            if ( reader.name() == QString( "edit" ) ) {
                if ( attrs.value( QString( "result" ) ).toString() == "Success" ) {
                    //qDebug()<<"Success Login";
                    this->setError(KJob::NoError);
                    reply->close();
                    reply->deleteLater();
                    emit resultEdit( this );
                    emitResult();
                    return;
                }
                else if ( attrs.value( QString( "result" ) ).toString() == "Failure" ) {
                    this->setError(KJob::NoError);
                    reader.readNext();
                    attrs = reader.attributes();
                    d->result.captchaid = attrs.value( QString( "id" ) ).toString().toUInt() ;
                    if (!attrs.value( QString( "question" ) ).isEmpty())
                        d->result.captchaquestionorurl = QVariant(attrs.value( QString( "question" ) ).toString()) ;
                    else if (!attrs.value( QString( "url" ) ).isEmpty())
                        d->result.captchaquestionorurl = QVariant(attrs.value( QString( "url" ) ).toString()) ;
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
    emit resultCaptcha( d->result.captchaquestionorurl );
}

void Edit::finishedCaptcha( QString  const & captcha )
{
    d->result.captchaword = captcha;
    QUrl url = d->baseUrl;
    url.addQueryItem("captchaid", QString::number(d->result.captchaid));
    url.addQueryItem("captchaword", d->result.captchaword);
    QString data = url.toString();

    // Set the request
    QNetworkRequest request( url );
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager = new QNetworkAccessManager();
    d->manager->post( request, data.toUtf8() );
    connect( d->manager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedEdit( QNetworkReply * ) ) );
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

Edit::Result Edit::getResults()
{
    return d->result;
}

int Edit::getError(const QString & error)
{
    QString temp = error;
    int ret = 0;
    QStringList list;
    list << "Falsexml"
            << "ConnectionAbort"
            << "notitle"
            << "notext"
            << "notoken"
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
    return  ret + (int)Edit::Falsexml ;
}
