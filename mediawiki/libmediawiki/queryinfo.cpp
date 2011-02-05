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

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QRegExp>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "job_p.h"
#include "queryinfo.h"

namespace mediawiki
{

class QueryInfoPrivate : public JobPrivate
{

public:

    QueryInfoPrivate(MediaWiki & mediawiki)
        : JobPrivate(mediawiki)
    {}

    QVector<Protection> protections;
    QMap<QString, QString> requestParameter;
    Page page;
    QString warning; //FIXME: Usefull ?

};

}

using namespace mediawiki;

QueryInfo::QueryInfo(MediaWiki & mediawiki, QObject * parent)
    : Job(*new QueryInfoPrivate(mediawiki), parent)
{
    setCapabilities(KJob::NoCapabilities);
}


QueryInfo::~QueryInfo() {}

void QueryInfo::setPageName(const QString & title)
{
    Q_D(QueryInfo);
    d->requestParameter["titles"] = title;
}

void QueryInfo::setToken(const QString & token)
{
    Q_D(QueryInfo);
    d->requestParameter["intoken"] = token;
}

void QueryInfo::setPageId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter["pageids"] = QString::number(id);
}

void QueryInfo::setRevisionId(unsigned int id)
{
    Q_D(QueryInfo);
    d->requestParameter["revids"] = QString::number(id);
}

QString QueryInfo::warning() const {
    Q_D(const QueryInfo);
    return d->warning;
}

void QueryInfo::setWarning(const QString & warning) {
    Q_D(QueryInfo);
    d->warning = warning;
}

void QueryInfo::start()
{
    Q_D(QueryInfo);
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryInfo::doWorkSendRequest()
{
    Q_D(QueryInfo);
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
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
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void QueryInfo::doWorkProcessReply(QNetworkReply * reply)
{
    Q_D(QueryInfo);
    disconnect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    if (reply->error() == QNetworkReply::NoError) {

        // Replace & in &amp;
        QString content(reply->readAll());
        QRegExp regex("&(?!\\w+;)");
        content.replace(regex, "&amp;");
        QXmlStreamReader reader(content);
        QVector<Protection> protect;

        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            QXmlStreamAttributes attrs = reader.attributes();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "page") {
                    d->page.setPageId(attrs.value( QString( "pageid" ) ).toString().toUInt());
                    d->page.setTitle(attrs.value( QString( "title" ) ).toString());
                    d->page.setNs(attrs.value( QString( "ns" ) ).toString().toUInt());
                    d->page.setTouched(QDateTime::fromString(attrs.value( QString( "touched" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
                    d->page.setLastRevId(attrs.value( QString( "lastrevid" ) ).toString().toUInt());
                    d->page.setCounter(attrs.value( QString( "counter" ) ).toString().toUInt());
                    d->page.setLength(attrs.value( QString( "length" ) ).toString().toUInt());
                    d->page.setStarttimestamp(QDateTime::fromString(attrs.value( QString( "starttimestamp" ) ).toString(), "yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"));
                    d->page.setEditToken(attrs.value( QString( "edittoken" ) ).toString());
                    d->page.setTalkid(attrs.value( QString( "talkid" ) ).toString().toUInt());
                    d->page.setFullurl(QUrl(attrs.value( QString( "fullurl" ) ).toString()));
                    d->page.setEditurl(QUrl(attrs.value( QString( "editurl" ) ).toString()));
                    d->page.setReadable(attrs.value( QString( "readable" ) ).toString());
                    d->page.setPreload(attrs.value( QString( "preload" ) ).toString());
                }
                else if (reader.name() == "protection") {
                    protect.clear();
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
                    Protection p;
                    p.setExpiry(expiry);
                    p.setLevel(level);
                    p.setType(type);
                    p.setSource(source);
                    protect.push_back(p);
                }
                else if (reader.name() == "info") {
                    reader.readNext();
                    d->warning = reader.text().toString();
                }
            } else if (token == QXmlStreamReader::EndElement) {
                if (reader.name() == "page") {
                    d->protections = protect;
                }
            }
        }
        if (!reader.hasError()) {
            setError(KJob::NoError);
            emit protection(protect);
            emit page(d->page);
        } else {
            setError(Job::XmlError);
        }
    }
    else {
        setError(Job::NetworkError);
    }
    emitResult();
}
