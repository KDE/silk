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
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "queryimages.h"

namespace mediawiki {

struct QueryImagesPrivate {

    QueryImagesPrivate(QNetworkAccessManager * const manager, const MediaWiki & mediawiki, const QString & limit)
        : manager(manager)
        , mediawiki(mediawiki)
        , limit(limit)
        , imcontinue()
    {}

    QNetworkAccessManager * const manager;
    const MediaWiki & mediawiki;
    QString title;
    QString limit;
    QString imcontinue;

};

}

using namespace mediawiki;

QueryImages::QueryImages(const MediaWiki & mediawiki, QObject * parent)
    : KJob(parent)
    , d(new QueryImagesPrivate(new QNetworkAccessManager(this), mediawiki, "10"))
{
    setCapabilities(KJob::NoCapabilities);
}

void QueryImages::setTitle(const QString & title) {
    d->title = title;
}

void QueryImages::setLimit(unsigned int limit) {
    d->limit = QString::number(limit);
}

QueryImages::~QueryImages() {
    delete d;
}

void QueryImages::start() {
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void QueryImages::doWorkSendRequest() {
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("titles", d->title);
    url.addQueryItem("prop", "images");
    url.addQueryItem("imlimit", d->limit);
    if (!d->imcontinue.isNull()) {
        url.addQueryItem("imcontinue", d->imcontinue);
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    if (d->imcontinue.isNull()) {
        connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    }
}
#include <QDebug>
void QueryImages::doWorkProcessReply(QNetworkReply * reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QList<Image> imagesReceived;
        d->imcontinue = QString();
        QXmlStreamReader reader(reply);
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "images") {
                    if (reader.attributes().value("imcontinue").isNull()) {
                        imagesReceived.clear();
                    } else {
                        d->imcontinue = reader.attributes().value("imcontinue").toString();
                    }
                } else if (reader.name() == "im") {
                    Image image;
                    image.setNamespaceId( reader.attributes().value("ns").toString().toUInt());
                    image.setTitle(reader.attributes().value("title").toString());
                    imagesReceived.push_back(image);
                }
            }
        }
        //FIXME: Bad solution
        if (reader.error() == QXmlStreamReader::NoError || reader.error() == QXmlStreamReader::PrematureEndOfDocumentError) {
            emit images(imagesReceived);
            if (!d->imcontinue.isNull()) {
                QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
                return;
            } else {
                setError(KJob::NoError);
            }
        } else {
            setError(QueryImages::XmlError);
        }
    }
    else {
        setError(QueryImages::NetworkError);
    }
    emitResult();
}
