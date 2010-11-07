/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *   Copyright 2010 by Guillaume Hormiere <hormiere.guillaume@gmail.com>
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

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "general.h"

namespace mediawiki {

    struct GeneralPrivate
    {
        GeneralPrivate(QNetworkAccessManager * const manager,General::Result* result, MediaWiki const & mediawiki)
                : manager(manager)
                , result(result)
                , mediawiki(mediawiki) {}
        QNetworkAccessManager * manager;
        General::Result* result;
        MediaWiki const & mediawiki;
    };

}

using namespace mediawiki;

General::General(MediaWiki const & mediawiki, QObject * parent)
    : KJob(parent),d(new GeneralPrivate(new QNetworkAccessManager(this),new Result,mediawiki))
{
    setCapabilities(KJob::NoCapabilities);
}

General::~General()
{
    delete this->d->result;
    delete d;
}
void General::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}
void General::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta", "siteinfo");
    url.addQueryItem("siprop", "general");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "mediawiki-silk");
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}
void General::abort()
{
    this->setError(this->connectionAbort);
    emitResult();
}
void General::doWorkProcessReply(QNetworkReply * reply)
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->connectionAbort);
        reply->close();
        reply->deleteLater();
        emitResult();
        return;
    }

    QXmlStreamReader reader(reply);
    while(!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if(reader.name() == "general") {
                d->result->mainpage = reader.attributes().value("mainpage").toString();
                d->result->base = reader.attributes().value("base").toString();
                d->result->sitename = reader.attributes().value("sitename").toString();
                d->result->generator = reader.attributes().value("generator").toString();
                d->result->phpversion = reader.attributes().value("phpversion").toString();
                d->result->phpsapi = reader.attributes().value("phpsapi").toString();
                d->result->dbtype = reader.attributes().value("dbtype").toString();
                d->result->dbversion = reader.attributes().value("dbversion").toString();
                d->result->rev = reader.attributes().value("rev").toString();
                d->result->cas = reader.attributes().value("case").toString();
                d->result->rights = reader.attributes().value("rights").toString();
                d->result->lang = reader.attributes().value("lang").toString();
                d->result->fallback8bitencoding = reader.attributes().value("fallback8bitEncoding").toString();
                d->result->writeapi = reader.attributes().value("writeapi").toString();
                d->result->timezone = reader.attributes().value("timezone").toString();
                d->result->timeoffset = reader.attributes().value("timeoffset").toString();
                d->result->articlepath = reader.attributes().value("articlepath").toString();
                d->result->scriptpath = reader.attributes().value("scriptpath").toString();
                d->result->script = reader.attributes().value("script").toString();
                d->result->variantarticlepath = reader.attributes().value("variantarticlepath").toString();
                d->result->server = reader.attributes().value("server").toString();
                d->result->wikiid = reader.attributes().value("wikiid").toString();
                d->result->time = reader.attributes().value("time").toString();
            }
            else if(reader.name() == "error")
            {
                this->setError(this->includeAllDenied);
                reply->close();
                reply->deleteLater();
                emitResult();
                return;
            }
        }
    }
    if(reader.hasError())this->setError(this->falsexml);


    reply->close();
    reply->deleteLater();
    emitResult();
}
mediawiki::General::Result General::getResult(){ return *d->result; }
