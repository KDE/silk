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

#include "job_p.h"
#include "logout.h"
#include "mediawiki.h"

namespace mediawiki {

    class LogoutPrivate : public JobPrivate {

    public:

        LogoutPrivate(MediaWiki & mediawiki)
            : JobPrivate(mediawiki)
        {}

    };

}

using namespace mediawiki;

Logout::Logout(MediaWiki & mediawiki, QObject *parent)
    : Job(*new LogoutPrivate(mediawiki), parent)
{
    setCapabilities(KJob::NoCapabilities);
}

Logout::~Logout() {}

void Logout::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void Logout::doWorkSendRequest()
{
    Q_D(Logout);
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "logout");
    QByteArray cookie = "";
    for(int i = 0 ; i<d->mediawiki.cookies().size();i++){
        cookie += d->mediawiki.cookies().at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader( "Cookie", cookie );
    // Delete cookies
    d->mediawiki.manager()->setCookieJar(new QNetworkCookieJar);
    // Send the request
    d->mediawiki.manager()->get(request);
    connect(d->mediawiki.manager(), SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void Logout::doWorkProcessReply(QNetworkReply * reply)
{
    this->setError(KJob::NoError);
    reply->close();
    reply->deleteLater();
    emitResult();
}
