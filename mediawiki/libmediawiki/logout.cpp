/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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
#include <QtCore/QDebug>
#include <QDateTime>

#include "logout.h"
#include "mediawiki.h"

namespace mediawiki
{
    struct LogoutPrivate
    {

        LogoutPrivate(QNetworkAccessManager * const manager, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki)
        {}

        QNetworkAccessManager *manager;
        MediaWiki const & mediawiki;

    };
}

using namespace mediawiki;

Logout::Logout(MediaWiki const & mediawiki, QObject *parent)
    : KJob(parent)
    , d(new LogoutPrivate(new QNetworkAccessManager(this), mediawiki))
{
    setCapabilities(KJob::NoCapabilities);
}

Logout::~Logout()
{
    delete d;
}

void Logout::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Logout::abort()
{
    this->setError(this->ConnectionAbort);
    emitResult();
}

void Logout::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "logout");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
    QTimer::singleShot( 30 * 1000, this, SLOT( abort() ) );
}

void Logout::doWorkProcessReply(QNetworkReply * reply)
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        this->setError(this->ConnectionAbort);
        return;
    }
    this->setError(KJob::NoError);
    qDebug()<<"Logout ";
    d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).clear();
    reply->close();
    reply->deleteLater();
    emitResult();
}

QList<QNetworkCookie> Logout::cookies()
{
    if (d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url()).isEmpty()) {
        return QList<QNetworkCookie>();
    }
    else {
        return d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());
    }
}
