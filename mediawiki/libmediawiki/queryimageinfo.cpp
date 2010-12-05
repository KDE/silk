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

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "queryimageinfo.h"

namespace mediawiki {

struct QueryImageinfoPrivate {

    QueryImageinfoPrivate(QNetworkAccessManager * const manager,
                          MediaWiki const & mediawiki,
                          QString const & title,
                          QueryImageinfo::property_type properties)
        : manager(manager)
        , mediawiki(mediawiki)
        , title(title)
        , properties(properties)
    {}

    QNetworkAccessManager * const manager;

    MediaWiki const & mediawiki;

    QString const title;

    QueryImageinfo::property_type properties;

};

}

using namespace mediawiki;

QueryImageinfo::QueryImageinfo(MediaWiki const & mediawiki, QString const & title, QObject * parent)
    : KJob(parent)
    , d(new QueryImageinfoPrivate(new QNetworkAccessManager(this),
                                  mediawiki,
                                  title,
                                  QueryImageinfo::NO_PROPERTY))
{
    setCapabilities(KJob::NoCapabilities);
}

QueryImageinfo::~QueryImageinfo() {
    delete d;
}

void QueryImageinfo::paramProperties(property_type properties) {
    d->properties = properties;
}

void QueryImageinfo::start() {
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryImageinfo::doWorkSendRequest() {
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("titles", d->title);
    url.addQueryItem("prop", "imageinfo");
    url.addQueryItem("iiprop", iiprop());
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void QueryImageinfo::doWorkProcessReply(QNetworkReply * reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QList<QueryImageinfo::Imageinfo> imageinfosReceived;
        QXmlStreamReader reader(reply);
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "ii") {
                    imageinfosReceived.push_back(QueryImageinfo::Imageinfo(
                        QDateTime::fromString(reader.attributes().value("timestamp").toString(), "yyyy-MM-dd'T'hh:mm:ss'Z'"),
                        reader.attributes().value("user").toString(),
                        QUrl(reader.attributes().value("url").toString()),
                        QUrl(reader.attributes().value("descriptionurl").toString()),
                        d->properties
                    ));
                }
            }
        }
        if (!reader.hasError()) {
            emit imageinfos(imageinfosReceived);
            setError(KJob::NoError);
        } else {
            setError(QueryImageinfo::XmlError);
        }
    }
    else {
        setError(QueryImageinfo::NetworkError);
    }
    emitResult();
}

QString QueryImageinfo::iiprop() const {
    QString iiprop;
    if (d->properties & QueryImageinfo::TIMESTAMP) {
        iiprop.append("timestamp|");
    }
    if (d->properties & QueryImageinfo::USER) {
        iiprop.append("user|");
    }
    if (d->properties & QueryImageinfo::URL) {
        iiprop.append("url|");
    }
    iiprop.chop(1);
    return iiprop;
}
