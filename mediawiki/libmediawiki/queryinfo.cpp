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
#include <QtCore/QRegExp>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QDebug>
#include <QDateTime>

#include "queryinfo.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct QueryInfoPrivate
    {

        QueryInfoPrivate(int id, QueryInfo::IdType type, QString token, MediaWiki & mediawiki)
            : mediawiki(mediawiki)
            , id(id)
            , type(type)
            , token(token)
        {
            if(type == QueryInfo::pageids)
                requestParameter["pageids"] = QString::number(id);
            else if(type == QueryInfo::revids)
                requestParameter["revids"] = QString::number(id);
        }

        QueryInfoPrivate(QString title, QString token, MediaWiki & mediawiki)
            : mediawiki(mediawiki)
            , id(0)
            , type(QueryInfo::noid)
            , token(token)
        {
            requestParameter["titles"] = title;
        }

        MediaWiki & mediawiki;
        QMap<QString, QString> requestParameter;
        int id;
        QueryInfo::IdType type;
        QString token;
    };
}

using namespace mediawiki;
QueryInfo::QueryInfo(MediaWiki & mediawiki, QString const & titre, QString const & token, QObject *parent)
    : KJob(parent)
    , d(new QueryInfoPrivate(titre, token, mediawiki))
{
    setCapabilities(KJob::NoCapabilities);
}


QueryInfo::QueryInfo(MediaWiki & mediawiki, unsigned int id, QueryInfo::IdType type, QString const & token, QObject *parent)
    : KJob(parent)
    , d(new QueryInfoPrivate(id, type, token, mediawiki))
{
    setCapabilities(KJob::NoCapabilities);
}

QueryInfo::~QueryInfo()
{
    delete d;
}

void QueryInfo::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryInfo::abort()
{
    this->setError(this->ConnectionAbort);
    emitResult();
}

void QueryInfo::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addEncodedQueryItem("intoken", d->token.toUtf8());
    url.addQueryItem("prop", "info");
    url.addEncodedQueryItem("inprop", QString("protection|talkid|watched|subjectid|url|readable|preload").toUtf8());

    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext()) {
        i.next();
        url.addEncodedQueryItem(QByteArray(i.key().toStdString().c_str()),QByteArray(i.value().toStdString().c_str()));
    }

    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    QByteArray cookie = "";
    for(int i = 0 ; i<d->mediawiki.cookies().size();i++){
        cookie += d->mediawiki.cookies().at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }
    request.setRawHeader( "Cookie", cookie );
    // Send the request
    d->mediawiki.manager()->get(request);
    connect(d->mediawiki.manager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

void QueryInfo::doWorkProcessReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QList<QueryInfo::Result> results;
        Result page;
        QList<QueryInfo::Protection> protections;

        // Replace & in &amp;
        QString content(reply->readAll());
        QRegExp regex("&(?!\\w+;)");
        content.replace(regex, "&amp;");
        QXmlStreamReader reader(content);

        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            QXmlStreamAttributes attrs = reader.attributes();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "page") {
                    page.m_pageid = attrs.value( QString( "pageid" ) ).toString().toUInt();
                    page.m_title = attrs.value( QString( "title" ) ).toString();
                    page.m_ns = attrs.value( QString( "ns" ) ).toString().toUInt();
                    page.m_touched = QDateTime::fromString(attrs.value( QString( "touched" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
                    page.m_lastrevid = attrs.value( QString( "lastrevid" ) ).toString().toUInt();
                    page.m_counter = attrs.value( QString( "counter" ) ).toString().toUInt();
                    page.m_length = attrs.value( QString( "length" ) ).toString().toUInt();
                    page.m_starttimestamp = QDateTime::fromString(attrs.value( QString( "starttimestamp" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'");
                    page.m_edittoken = attrs.value( QString( "edittoken" ) ).toString();
                    page.m_talkid = attrs.value( QString( "talkid" ) ).toString().toUInt();
                    page.m_fullurl = QUrl(attrs.value( QString( "fullurl" ) ).toString());
                    page.m_editurl = QUrl(attrs.value( QString( "editurl" ) ).toString());
                    page.m_readable = attrs.value( QString( "readable" ) ).toString();
                    page.m_preload = attrs.value( QString( "preload" ) ).toString();
                }
                else if (reader.name() == "protection") {
                    protections.clear();
                }
                else if (reader.name() == "pr") {
                    QString expiry(attrs.value( QString( "expiry" ) ).toString());
                    QString level(attrs.value( QString( "level" ) ).toString());
                    QString type(attrs.value( QString( "type" ) ).toString());
                    QString source = "";
                    if(attrs.value( QString( "source" ) ).toString() != "") {
                        source = attrs.value( QString( "source" ) ).toString();
                    }else if(attrs.value( QString( "cascade" ) ).toString() != "") {
                        source = attrs.value( QString( "cascade" ) ).toString();
                    }
                    protections.push_back(QueryInfo::Protection(type, level, expiry, source));
                }
                else if (reader.name() == "info") {
                    reader.readNext();
                    m_warning = reader.text().toString();
                }
            } else if (token == QXmlStreamReader::EndElement) {
                if (reader.name() == "page") {
                    page.setProtections(protections);
                    results.push_back(QueryInfo::Result(page));
                }
            }
        }
        if (!reader.hasError()) {
            setError(KJob::NoError);
            emit infos(results);
        } else {
            setError(QueryInfo::Falsexml);
        }
    }
    else {
        setError(QueryInfo::ConnectionAbort);
    }
    emitResult();
}
