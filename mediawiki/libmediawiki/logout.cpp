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

#include "logout.moc"

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"
#include "job_p.h"

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
{}

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
    QList<QNetworkCookie> mediawikiCookies = d->manager->cookieJar()->cookiesForUrl(d->mediawiki.url());
    for(int i = 0 ; i < mediawikiCookies.size(); ++i){
        cookie += mediawikiCookies.at(i).toRawForm(QNetworkCookie::NameAndValueOnly);
        cookie += ";";
    }
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", d->mediawiki.userAgent().toUtf8());
    request.setRawHeader( "Cookie", cookie );
    // Delete cookies
    d->manager->setCookieJar(new QNetworkCookieJar);
    // Send the request
    d->reply = d->manager->get(request);
    connectReply();
    connect(d->reply, SIGNAL(finished()), this, SLOT(doWorkProcessReply()));
}

void Logout::doWorkProcessReply()
{
    Q_D(Logout);
    disconnect(d->reply, SIGNAL(finished()), this, SLOT(doWorkProcessReply()));
    this->setError(KJob::NoError);
    d->reply->close();
    d->reply->deleteLater();
    emitResult();
}
