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

#include <QtCore/QList>
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
                          const MediaWiki & mediawiki,
                          const QString & title,
                          QueryImageinfo::property_type properties,
                          const QString & limit,
                          bool stop,
                          const QString & start,
                          const QString & end,
                          QString width,
                          QString height)
        : manager(manager)
        , mediawiki(mediawiki)
        , title(title)
        , properties(properties)
        , limit(limit)
        , stop(stop)
        , begin(begin)
        , end(end)
        , width(width)
        , height(height)
    {}

    QNetworkAccessManager * const manager;
    const MediaWiki & mediawiki;
    const QString title;
    QueryImageinfo::property_type properties;
    QString limit;
    bool stop;
    QString begin;
    QString end;
    QString width;
    QString height;

};

}

using namespace mediawiki;

QueryImageinfo::QueryImageinfo(const MediaWiki & mediawiki, const QString & title, QObject * parent)
    : KJob(parent)
    , d(new QueryImageinfoPrivate(new QNetworkAccessManager(this),
                                  mediawiki,
                                  title,
                                  QueryImageinfo::NO_PROPERTY,
                                  QString("1"),
                                  false,
                                  QString(),
                                  QString(),
                                  QString("0"),
                                  QString("0")))
{
    setCapabilities(KJob::NoCapabilities);
}

QueryImageinfo::~QueryImageinfo() {
    delete d;
}

void QueryImageinfo::paramProperties(property_type properties) {
    d->properties = properties;
}

void QueryImageinfo::paramLimit(unsigned int limit, bool stop) {
    Q_ASSERT(limit >= 1u);
    d->limit = QString::number(limit);
    d->stop = stop;
}

void QueryImageinfo::paramStart(const QDateTime & start) {
    d->start = start.toString("yyyy-MM-dd'T'hh:mm:ss'Z'");
}

void QueryImageinfo::paramEnd(const QDateTime & end) {
    d->end = end.toString("yyyy-MM-dd'T'hh:mm:ss'Z'");
}

void QueryImageinfo::paramScale(unsigned int width) {
    Q_ASSERT(width >= 1u);
    d->width = QString::number(width);
    d->height = QString::number(0u);
}

void QueryImageinfo::paramScale(unsigned int width, unsigned int height) {
    Q_ASSERT(width >= 1u);
    Q_ASSERT(height >= 1u);
    d->width = QString::number(width);
    d->height = QString::number(height);
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
    url.addQueryItem("iilimit", d->limit);
    if (!d->begin.isNull()) {
        url.addQueryItem("iistart", d->begin);
    }
    if (!d->end.isNull()) {
        url.addQueryItem("iiend", d->end);
    }
    if (d->width != QString("0")) {
        url.addQueryItem("iiurlwidth", d->width);
        if (d->height != QString("0")) {
            url.addQueryItem("iiurlheight", d->height);
        }
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

qint64 toQInt64(const QString & qstring)
{
    bool ok;
    qint64 result = qstring.toLongLong(&ok);
    return ok ? result : -1;
}

void QueryImageinfo::doWorkProcessReply(QNetworkReply * reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QXmlStreamReader reader(reply);
        QList<QueryImageinfo::Image> imagesReceived;
        unsigned int namespaceId;
        QString title;
        QString imageRepository;
        QList<Imageinfo> imageinfos;
        Imageinfo imageinfo;
        QMap<QString, QString> normalized;
        d->begin = QString();
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "n") {
                    normalized[reader.attributes().value("to").toString()] = reader.attributes().value("from").toString();
                } else if (reader.name() == "page") {
                    namespaceId = reader.attributes().value("ns").toString().toUInt();
                    title = reader.attributes().value("title").toString();
                    imageRepository = reader.attributes().value("imagerepository").toString();
                } else if (reader.name() == "imageinfo") {
                    if (reader.attributes().value("iistart").isNull()) {
                        imageinfos.clear();
                    } else {
                        d->begin = reader.attributes().value("iistart").toString();
                    }
                } else if (reader.name() == "ii") {
                    imageinfo.setTimestamp(QDateTime::fromString(reader.attributes().value("timestamp").toString(), "yyyy-MM-dd'T'hh:mm:ss'Z'"));
                    imageinfo.setUser(reader.attributes().value("user").toString());
                    imageinfo.setComment(reader.attributes().value("comment").toString());
                    imageinfo.setUrl(QUrl(reader.attributes().value("url").toString()));
                    imageinfo.setDescriptionUrl(QUrl(reader.attributes().value("descriptionurl").toString()));
                    imageinfo.setThumbUrl(QUrl(reader.attributes().value("thumburl").toString()));
                    imageinfo.setThumbWidth(toQInt64(reader.attributes().value("thumbwidth").toString()));
                    imageinfo.setThumbHeight(toQInt64(reader.attributes().value("thumbheight").toString()));
                    imageinfo.setSize(toQInt64(reader.attributes().value("size").toString()));
                    imageinfo.setWidth(toQInt64(reader.attributes().value("width").toString()));
                    imageinfo.setHeight(toQInt64(reader.attributes().value("height").toString()));
                    imageinfo.setSha1(reader.attributes().value("sha1").toString());
                    imageinfo.setMime(reader.attributes().value("mime").toString());
                } else if (reader.name() == "metadata") {
                    if (reader.attributes().isEmpty()) {
                        imageinfo.metadata().clear();
                    } else {
                        imageinfo.metadata()[reader.attributes().value("name").toString()] = reader.attributes().value("value").toString();
                    }
                }
            } else if (token == QXmlStreamReader::EndElement) {
                if (reader.name() == "page") {
                    imagesReceived.push_back(QueryImageinfo::Image(namespaceId,
                                                           title,
                                                           normalized.contains(title) ? normalized[title] : title,
                                                           imageRepository,
                                                           QVector<Imageinfo>::fromList(imageinfos)));
                } else if (reader.name() == "ii") {
                    imageinfos.push_back(imageinfo);
                }
            }
        }
        if (!reader.hasError()) {
            disconnect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
            emit images(imagesReceived);
            if (d->begin.isNull() || d->stop) {
                setError(KJob::NoError);
            }
            else {
                QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
                return;
            }
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
    if (d->properties & QueryImageinfo::COMMENT) {
        iiprop.append("comment|");
    }
    if (d->properties & QueryImageinfo::URL) {
        iiprop.append("url|");
    }
    if (d->properties & QueryImageinfo::SIZE) {
        iiprop.append("size|");
    }
    if (d->properties & QueryImageinfo::SHA1) {
        iiprop.append("sha1|");
    }
    if (d->properties & QueryImageinfo::MIME) {
        iiprop.append("mime|");
    }
    if (d->properties & QueryImageinfo::METADATA) {
        iiprop.append("metadata|");
    }
    iiprop.chop(1);
    return iiprop;
}
