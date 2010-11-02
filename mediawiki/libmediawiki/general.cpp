/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
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
#include <QtCore/QXmlStreamReader>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"

#include "general.h"

struct GeneralPrivate
{
    QNetworkAccessManager * manager;
    QString mainpage;
    QString base;
    QString sitename;
    QString generator;
    QString phpversion;
    QString phpsapi;
    QString dbtype;
    QString dbversion;
    QString rev;
    QString cas;
    QString rights;
    QString lang;
    QString fallback8bitencoding;
    QString writeapi;
    QString timezone;
    QString timeoffset;
    QString articlepath;
    QString scriptpath;
    QString script;
    QString variantarticlepath;
    QString server;
    QString wikiid;
    QString time;
};

General::General(MediaWiki const & mediawiki, QObject * parent) : QObject(parent), d(new GeneralPrivate)
{
    // Set the url
    QUrl url = mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta", "siteinfo");
    url.addQueryItem("siprop", "general");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "mediawiki-silk");
    // Send the request
    d->manager = new QNetworkAccessManager(this);
    connect(d->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply *)));
    d->manager->get(request);
}

General::~General()
{
    delete d;
}

void General::onFinished(QNetworkReply * reply)
{
    bool xmlReturn = true;
    if (reply->error() == QNetworkReply::NoError) {
        QXmlStreamReader reader(reply);
        while(!reader.atEnd() && !reader.hasError()) {
            QXmlStreamReader::TokenType token = reader.readNext();
            if(token == QXmlStreamReader::StartElement) {
                if(reader.name() == "general") {
                    d->mainpage = reader.attributes().value("mainpage").toString();
                    d->base = reader.attributes().value("base").toString();
                    d->sitename = reader.attributes().value("sitename").toString();
                    d->generator = reader.attributes().value("generator").toString();
                    d->phpversion = reader.attributes().value("phpversion").toString();
                    d->phpsapi = reader.attributes().value("phpsapi").toString();
                    d->dbtype = reader.attributes().value("dbtype").toString();
                    d->dbversion = reader.attributes().value("dbversion").toString();
                    d->rev = reader.attributes().value("rev").toString();
                    d->cas = reader.attributes().value("case").toString();
                    d->rights = reader.attributes().value("rights").toString();
                    d->lang = reader.attributes().value("lang").toString();
                    d->fallback8bitencoding = reader.attributes().value("fallback8bitEncoding").toString();
                    d->writeapi = reader.attributes().value("writeapi").toString();
                    d->timezone = reader.attributes().value("timezone").toString();
                    d->timeoffset = reader.attributes().value("timeoffset").toString();
                    d->articlepath = reader.attributes().value("articlepath").toString();
                    d->scriptpath = reader.attributes().value("scriptpath").toString();
                    d->script = reader.attributes().value("script").toString();
                    d->variantarticlepath = reader.attributes().value("variantarticlepath").toString();
                    d->server = reader.attributes().value("server").toString();
                    d->wikiid = reader.attributes().value("wikiid").toString();
                    d->time = reader.attributes().value("time").toString();
                }
            }
        }
        xmlReturn = reader.hasError();

    }
    reply->close();
    reply->deleteLater();
    if(!xmlReturn)
        emit finished(true);
    else
        emit finished(false);


    //TESTS VALEURS GET
//    qDebug() << "getMainpage : " << this->getMainpage();
//    qDebug() << "getArticlepath : " << this->getArticlepath();
//    qDebug() << "getBase : " << this->getBase();
//    qDebug() << "getCase : " << this->getCase();
//    qDebug() << "getDbtype : " << this->getDbtype();
//    qDebug() << "getDbversion : " << this->getDbversion();
//    qDebug() << "getFallback8bitencoding : " << this->getFallback8bitencoding();
//    qDebug() << "getGenerator : " << this->getGenerator();
//    qDebug() << "getLang : " << this->getLang();
//    qDebug() << "getPhpsapi : " << this->getPhpsapi();
//    qDebug() << "getPhpversion : " << this->getPhpversion();
//    qDebug() << "getRev : " << this->getRev();
//    qDebug() << "getRights : " << this->getRights();
//    qDebug() << "getScript : " << this->getScript();
//    qDebug() << "getScriptpath : " << this->getScriptpath();
//    qDebug() << "getServer : " << this->getServer();
//    qDebug() << "getSitename : " << this->getSitename();
//    qDebug() << "getTime : " << this->getTime();
//    qDebug() << "getTimeoffset : " << this->getTimeoffset();
//    qDebug() << "getTimezone : " << this->getTimezone();
//    qDebug() << "getVariantarticlepath : " << this->getVariantarticlepath();
//    qDebug() << "getWikiid : " << this->getWikiid();
//    qDebug() << "getWriteapi : " << this->getWriteapi();
}

QString General::getMainpage(){ return d->mainpage; }
QString General::getBase(){ return d->base; }
QString General::getSitename(){ return d->sitename; }
QString General::getGenerator(){ return d->generator; }
QString General::getPhpversion(){ return d->phpversion; }
QString General::getPhpsapi(){ return d->phpsapi; }
QString General::getDbtype(){ return d->dbtype; }
QString General::getDbversion(){ return d->dbversion; }
QString General::getRev(){ return d->rev; }
QString General::getCase(){ return d->cas; }
QString General::getRights(){ return d->rights; }
QString General::getLang(){ return d->lang; }
QString General::getFallback8bitencoding(){ return d->fallback8bitencoding; }
QString General::getWriteapi(){ return d->writeapi; }
QString General::getTimezone(){ return d->timezone; }
QString General::getTimeoffset(){ return d->timeoffset; }
QString General::getArticlepath(){ return d->articlepath; }
QString General::getScriptpath(){ return d->scriptpath; }
QString General::getScript(){ return d->script; }
QString General::getVariantarticlepath(){ return d->variantarticlepath; }
QString General::getServer(){ return d->server; }
QString General::getWikiid(){ return d->wikiid; }
QString General::getTime(){ return d->time; }
