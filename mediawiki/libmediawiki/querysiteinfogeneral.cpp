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

#include <QtCore/QTimer>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "job_p.h"
#include "querysiteinfogeneral.h"

namespace mediawiki {

class QuerySiteInfoGeneralPrivate : public JobPrivate
{

public:

    QuerySiteInfoGeneralPrivate(MediaWiki & mediawiki)
        : JobPrivate(mediawiki)
    {}

    Generalinfo generalinfo;

};

}

using namespace mediawiki;

QuerySiteInfoGeneral::QuerySiteInfoGeneral(MediaWiki & mediawiki, QObject *parent)
    : Job(*new QuerySiteInfoGeneralPrivate(mediawiki))
{}

QuerySiteInfoGeneral::~QuerySiteInfoGeneral() {}

void QuerySiteInfoGeneral::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}
void QuerySiteInfoGeneral::doWorkSendRequest()
{
    Q_D(QuerySiteInfoGeneral);
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta", "siteinfo");
    url.addQueryItem("siprop", "general");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->reply = d->manager->get(request);
    connect(d->reply, SIGNAL(finished()), this, SLOT(doWorkProcessReply()));
}

void QuerySiteInfoGeneral::doWorkProcessReply()
{
    Q_D(QuerySiteInfoGeneral);
    disconnect(d->reply, SIGNAL(finished()), this, SLOT(doWorkProcessReply()));
    if (d->reply->error() != QNetworkReply::NoError)
    {
        this->setError(Job::NetworkError);
        d->reply->close();
        d->reply->deleteLater();
        emitResult();
        return;
    }

    QXmlStreamReader reader(d->reply);
    while(!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if(reader.name() == "general") {
                d->generalinfo.setMainPage(reader.attributes().value("mainpage").toString());
                d->generalinfo.setUrl(reader.attributes().value("base").toString());
                d->generalinfo.setSiteName(reader.attributes().value("sitename").toString());
                d->generalinfo.setGenerator(reader.attributes().value("generator").toString());
                d->generalinfo.setPhpVersion(reader.attributes().value("phpversion").toString());
                d->generalinfo.setPhpApi(reader.attributes().value("phpsapi").toString());
                d->generalinfo.setDataBaseType(reader.attributes().value("dbtype").toString());
                d->generalinfo.setDataBaseVersion(reader.attributes().value("dbversion").toString());
                d->generalinfo.setRev(reader.attributes().value("rev").toString());
                d->generalinfo.setCas(reader.attributes().value("case").toString());
                d->generalinfo.setRights(reader.attributes().value("rights").toString());
                d->generalinfo.setLanguage(reader.attributes().value("lang").toString());
                d->generalinfo.setFallBack8bitEncoding(reader.attributes().value("fallback8bitEncoding").toString());
                d->generalinfo.setWriteApi(reader.attributes().value("writeapi").toString());
                d->generalinfo.setTimeZone(reader.attributes().value("timezone").toString());
                d->generalinfo.setTimeOffset(reader.attributes().value("timeoffset").toString());
                d->generalinfo.setArticlePath(reader.attributes().value("articlepath").toString());
                d->generalinfo.setScriptPath(reader.attributes().value("scriptpath").toString());
                d->generalinfo.setScript(reader.attributes().value("script").toString());
                d->generalinfo.setVariantArticlePath(reader.attributes().value("variantarticlepath").toString());
                d->generalinfo.setServerUrl(reader.attributes().value("server").toString());
                d->generalinfo.setWikiId(reader.attributes().value("wikiid").toString());
                d->generalinfo.setTime(QDateTime::fromString(reader.attributes().value("time").toString(), "yyyy-MM-dd'T'hh:mm:ss'Z'"));
            }
            else if(reader.name() == "error")
            {
                this->setError(QuerySiteInfoGeneral::IncludeAllDenied);
                d->reply->close();
                d->reply->deleteLater();
                emitResult();
                return;
            }
        }
    }
    if(reader.hasError())this->setError(Job::XmlError);
    d->reply->close();
    d->reply->deleteLater();
    emitResult();
}

Generalinfo QuerySiteInfoGeneral::getResult()
{
    Q_D(QuerySiteInfoGeneral);
    return d->generalinfo;
}
