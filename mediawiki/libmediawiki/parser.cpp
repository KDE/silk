/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *   Copyright 2010 by Vincent Garcia <xavier.vincent.garcia@gmail.com>
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
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "job_p.h"
#include "mediawiki.h"
#include "parser.h"

namespace mediawiki
{

class ParserPrivate : public JobPrivate {

public:

    ParserPrivate(MediaWiki & mediawiki)
        : JobPrivate(mediawiki)
    {}

    QString text;

};

}

using namespace mediawiki ;

Parser::Parser(MediaWiki & mediawiki, QObject * parent)
    : Job(*new ParserPrivate(mediawiki), parent)
{}

Parser::~Parser() {}

QString Parser::text() const
{
    Q_D(const Parser);
    return d->text;
}

void Parser::setText(const QString & text)
{
    Q_D(Parser);
    d->text = text;
}

void Parser::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Parser::doWorkSendRequest()
{
    Q_D(Parser);
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "parse");
    url.addQueryItem("text", d->text);
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void Parser::doWorkProcessReply(QNetworkReply * reply)
{
    Q_D(Parser);
    disconnect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    if (reply->error() == QNetworkReply::NoError) {
        QXmlStreamReader reader(reply);
        QString text;
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "text") {
                    text = reader.text().toString();
                }
            }
        }
        if (!reader.hasError()) {
            emit result(text);

        } else {
            setError(Parser::XmlError);
        }
    }
    else {
        setError(Parser::NetworkError);
    }
    emitResult();
}
