/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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

#include "mediawiki.h"
#include "querysiteinfousergroups.h"

namespace mediawiki
{

struct QuerySiteinfoUsergroupsPrivate {

    QuerySiteinfoUsergroupsPrivate(QNetworkAccessManager * const manager, const MediaWiki & mediawiki, bool includeNumber)
            : manager(manager)
            , mediawiki(mediawiki)
            , includeNumber(includeNumber)
    {}

    QNetworkAccessManager * const manager;

    const MediaWiki & mediawiki;

    bool includeNumber;

};

}

using namespace mediawiki;

QuerySiteinfoUsergroups::QuerySiteinfoUsergroups(const MediaWiki & mediawiki, QObject * parent)
        : Job(mediawiki, parent)
        , d(new QuerySiteinfoUsergroupsPrivate(new QNetworkAccessManager(this), mediawiki, false))
{
    setCapabilities(Job::NoCapabilities);
}

QuerySiteinfoUsergroups::~QuerySiteinfoUsergroups()
{
    delete d;
}

void QuerySiteinfoUsergroups::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QuerySiteinfoUsergroups::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta",   "siteinfo");
    url.addQueryItem("siprop", "usergroups");
    if (d->includeNumber) {
        url.addQueryItem("sinumberingroup", QString());
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void QuerySiteinfoUsergroups::doWorkProcessReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QList<UserGroup> results;
        QString name;
        QList<QString> rights;
        unsigned int number;
        QXmlStreamReader reader(reply);
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "group") {
                    name = reader.attributes().value("name").toString();
                    if (d->includeNumber) {
                        number = reader.attributes().value("number").toString().toUInt();
                    }
                } else if (reader.name() == "rights") {
                    rights.clear();
                } else if (reader.name() == "permission") {
                    reader.readNext();
                    rights.push_back(reader.text().toString());
                }
            } else if (token == QXmlStreamReader::EndElement) {
                if (reader.name() == "group") {
                    UserGroup res;
                    res.setName(name);
                    res.setRights(rights);
                    if(d->includeNumber){
                        res.setNumber(number);
                    }
                    results.push_back(res);
                }
            }
        }
        if (!reader.hasError()) {
            setError(Job::NoError);
            emit usergroups(results);
        } else {
            setError(QuerySiteinfoUsergroups::XmlError);
        }
    }
    else {
        setError(QuerySiteinfoUsergroups::NetworkError);
    }
    emitResult();
}

void QuerySiteinfoUsergroups::setIncludeNumber(bool includeNumber)
{
    d->includeNumber = includeNumber;
}
