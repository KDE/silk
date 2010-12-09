/*
 *   Copyright 2010 by Hormiere Guillaume <hormiere.guillaume@gmail.com>
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
#include <QDebug>

#include "mediawiki.h"
#include "queryrevision.h"

namespace mediawiki
{

struct QueryRevisionPrivate {

    QueryRevisionPrivate(QNetworkAccessManager * const manager,
                         MediaWiki const & mediawiki,
                         int id,
                         QueryRevision::IdType type
                         )
            : manager(manager)
            , mediawiki(mediawiki)
            , id(id)
            , type(type)
    {
        if(type == QueryRevision::pageids)
            requestParameter["pageids"] = QString::number(id);
        else if(type == QueryRevision::revids)
            requestParameter["revids"] = QString::number(id);
    }
    QueryRevisionPrivate(QNetworkAccessManager * const manager,
                         MediaWiki const & mediawiki,
                         QString title
                         )
            : manager(manager)
            , mediawiki(mediawiki)
            , id(0)
            , type(QueryRevision::noid)
    {
        requestParameter["titles"] = title;
    }

    QNetworkAccessManager * const manager;

    MediaWiki const & mediawiki;

    QMap<QString, QString> requestParameter;

    int id;

    QueryRevision::IdType type;

};

}

using namespace mediawiki;

QueryRevision::QueryRevision(MediaWiki const & mediawiki, QString title, QObject * parent)
        : KJob(parent)
        , d(new QueryRevisionPrivate(new QNetworkAccessManager(this), mediawiki, title))
{
    setCapabilities(KJob::NoCapabilities);
}
QueryRevision::QueryRevision(MediaWiki const & mediawiki, int id, QueryRevision::IdType idType, QObject * parent)
    : KJob(parent)
    , d(new QueryRevisionPrivate(new QNetworkAccessManager(this), mediawiki, id, idType))
{
    setCapabilities(KJob::NoCapabilities);
}

QueryRevision::~QueryRevision()
{
    delete d;
}

void QueryRevision::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}


void QueryRevision::setRvProp(int param)
{
    QString buff;
    if(param & IDS) buff.append("ids");
    if(param & FLAGS)
    {
        if(buff.length())
            buff.append('|');
        buff.append("flags");
    }
    if(param & TIMESTAMP)
    {
        if(buff.length())
            buff.append('|');
        buff.append("timestamp");
    }
    if(param & USER)
    {
        if(buff.length())
            buff.append('|');
        buff.append("user");
    }
    if(param & COMMENT)
    {
        if(buff.length())
            buff.append('|');
        buff.append("comment");
    }
    if(param & SIZE)
    {
        if(buff.length())
            buff.append('|');
        buff.append("size");
    }
    if(param & CONTENT)
    {
        if(buff.length())
            buff.append('|');
        buff.append("content");
    }
    d->requestParameter["rvprop"] = buff;
}

void QueryRevision::setRvContinue(int param)
{
    d->requestParameter["rvcontinue"] = QString::number(param);
}

void QueryRevision::setRvLimit(int param)
{
    d->requestParameter["rvlimit"] = QString::number(param);
}

void QueryRevision::setRvStartId(int param)
{
    d->requestParameter["rvstartid"] = QString::number(param);
}
void QueryRevision::setRvEndId(int param)
{
    d->requestParameter["rvendid"] = QString::number(param);
}

void QueryRevision::setRvStart(QDateTime param)
{
    d->requestParameter["rvstart"] = param.toString("yyyy-MM-ddThh:mm:ssZ");
}

void QueryRevision::setRvEnd(QDateTime param)
{
    d->requestParameter["rvend"] = param.toString("yyyy-MM-ddThh:mm:ssZ");
}

void QueryRevision::setRvUser(QString param)
{
    d->requestParameter["rvuser"] = param;
}

void QueryRevision::setRvExcludeUser(QString param)
{
    d->requestParameter["rvexcludeuser"] = param;
}

void QueryRevision::setRvDir(QueryRevision::Dir param)
{
    if( param == QueryRevision::older)
        d->requestParameter["rvdir"] = QString("older");

    else if( param == QueryRevision::newer)
        d->requestParameter["rvdir"] = QString("newer");
}

void QueryRevision::setRvGenerateXML(bool param)
{
    if( param )
        d->requestParameter["rvgeneratexml"] = QString("on");
}

void QueryRevision::setRvSection(int param)
{
    d->requestParameter["rvsection"] = QString::number(param);
}

void QueryRevision::setRvToken(QueryRevision::Token t)
{
    if(QueryRevision::rollback == t)
        d->requestParameter["rvtoken"] = QString("rollback");
}

void QueryRevision::setRvExpandTemplates(bool param)
{
    if( param )
        d->requestParameter["rvexpandtemplates"] = QString("on");
}
void QueryRevision::doWorkSendRequest()
{
    // Set the url
    QUrl    url = d->mediawiki.url();
            url.addQueryItem("format", "xml");
            url.addQueryItem("action", "query");
            url.addQueryItem("prop", "revisions");

    QMapIterator<QString, QString> i(d->requestParameter);
    while (i.hasNext()) {
        i.next();
        url.addEncodedQueryItem(QByteArray(i.key().toStdString().c_str()),QByteArray(i.value().toStdString().c_str()));
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void QueryRevision::doWorkProcessReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QList<QueryRevision::Result> results;
        QueryRevision::Result tempR;
        QString replytmp = reply->readAll();
        if(d->requestParameter.contains("rvgeneratexml"))
        {
            for(int i = replytmp.indexOf("parsetree"); i != -1; i = replytmp.indexOf("parsetree", i+1))
            {
                int count = 0;
                while(count < 2)
                {
                    if(replytmp[i] == '"' && replytmp[i-1] != '\\')count++;
                    if(replytmp[i] == '<')replytmp[i]=char(255);
                    if(replytmp[i] == '>')replytmp[i]=char(254);
                    i++;
                }
            }
        }
        QXmlStreamReader reader(replytmp);
        while (!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartElement) {
                if (reader.name() == "page" && d->requestParameter.contains("rvtoken")) {
                    tempR.rollback = reader.attributes().value("rollbacktoken").toString();
                }
                if (reader.name() == "rev") {
                    if(d->requestParameter.contains("rvprop"))
                    {
                        QString rvprop = d->requestParameter["rvprop"];
                        if(rvprop.contains("ids")){
                            tempR.revid = reader.attributes().value("revid").toString().toInt();
                            tempR.parentId = reader.attributes().value("parentid").toString().toInt();}
                        if(rvprop.contains("size"))
                            tempR.size = reader.attributes().value("size").toString().toInt();
                        if(rvprop.contains("minor"))
                            tempR.minor = reader.attributes().value("minor").toString();
                        if(rvprop.contains("user"))
                            tempR.user = reader.attributes().value("user").toString();
                        if(rvprop.contains("timestamp"))
                            tempR.timeStamp = QDateTime::fromString(reader.attributes().value("timestamp").toString(),"yyyy-MM-ddThh:mm:ssZ");
                        if(rvprop.contains("comment"))
                            tempR.comment = reader.attributes().value("comment").toString();
                        if(d->requestParameter.contains("rvgeneratexml"))
                            tempR.parseTree = reader.attributes().value("parsetree").toString();
                        if(rvprop.contains("content"))
                            tempR.content = reader.readElementText();
                    }
            results << tempR;
            }
            else if(reader.name() == "error")
            {
                if(reader.attributes().value("code").toString() == QString("rvrevids"))
                    this->setError(this->RevIds);
                else if(reader.attributes().value("code").toString() == QString("rvmultpages"))
                    this->setError(this->MultPages);
                else if(reader.attributes().value("code").toString() == QString("rvaccessdenied"))
                    this->setError(this->AccessDenied);
                else if(reader.attributes().value("code").toString() == QString("rvbadparams"))
                    this->setError(this->AddParams);
                else if(reader.attributes().value("code").toString() == QString("rvnosuchsection"))
                    this->setError(this->NoSuchSection);

                reply->close();
                reply->deleteLater();
                emit revision(QList<QueryRevision::Result>());
                emitResult();
                return;
            }
        }
      }
        if (!reader.hasError()) {
            setError(KJob::NoError);
            for(int i = 0; i < results.length(); i++)
            {
                results[i].parseTree.replace(char(254),'>');
                results[i].parseTree.replace(char(255),'<');
            }

            emit revision(results);
        } else {
            setError(QueryRevision::XmlError);
            reply->close();
            reply->deleteLater();
            emit revision(QList<QueryRevision::Result>());
        }
    }
    else {
        setError(QueryRevision::NetworkError);
        reply->close();
        reply->deleteLater();
        emit revision(QList<QueryRevision::Result>());
    }
    emitResult();
}
