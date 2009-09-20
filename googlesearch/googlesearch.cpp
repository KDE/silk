/*
 *   Copyright 2009 by Richard Moore <rich@kde.org>
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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QDebug>

#include <qjson/parser.h>

#include "googlesearch.h"

struct GoogleSearchPrivate
{
    QList<GoogleSearch::Result> results;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    int timeout;
};

GoogleSearch::GoogleSearch( QObject *parent )
    : QObject( parent )
{
    d = new GoogleSearchPrivate;
    d->manager = new QNetworkAccessManager( this );
    d->timeout = 30 * 1000; // 30 second
    d->reply = 0;

    connect( d->manager, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply *)) );
}

GoogleSearch::~GoogleSearch()
{
    delete d;
}

QList<GoogleSearch::Result> GoogleSearch::results() const
{
    return d->results;
}

void GoogleSearch::search( const QString &terms )
{
    QUrl url("http://ajax.googleapis.com/ajax/services/search/web");
    url.addQueryItem( QString("v"), QString("1.0") );
    url.addQueryItem( QString("q"), terms );

    d->reply = d->manager->get( QNetworkRequest(url) );
    QTimer::singleShot( d->timeout, this, SLOT( abort() ) );
}

void GoogleSearch::finished( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Request failed, " << reply->errorString();
        emit finished(false);
        return;
    }

    qDebug() << "Request succeeded";

    bool ok = processSearchResult( reply );

    emit finished( ok );
    d->reply->deleteLater();
    d->reply = 0;
}

bool GoogleSearch::processSearchResult( QIODevice *source )
{
    d->results.clear();

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse( source, &ok ).toMap();
    QVariantMap responseData = result[QString("responseData")].toMap();

    foreach (QVariant hit, responseData[QString("results")].toList()) {
	QVariantMap hitMap = hit.toMap();
	//	qDebug() << hitMap[QString("url")].toString();
	Result r;
	r.url = hitMap[QString("url")].toString();
	r.title = hitMap[QString("titleNoFormatting")].toString();

	d->results.append( r );
    }

    return ok;
}

void GoogleSearch::abort()
{
    if ( !d->reply )
	return;

    d->reply->abort();
    d->reply = 0;
}
