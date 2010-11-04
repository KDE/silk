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

#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "mediawiki.h"

#include "usergroups.h"

namespace mediawiki
{

struct UserGroupsPrivate {

    UserGroupsPrivate(QNetworkAccessManager * const manager, MediaWiki const & mediawiki)
            : manager(manager)
            , mediawiki(mediawiki) {}

    QNetworkAccessManager * const manager;

    MediaWiki const & mediawiki;

};

}

using namespace mediawiki;

UserGroups::UserGroups(MediaWiki const & mediawiki, QObject * parent)
        : KJob(parent)
        , d(new UserGroupsPrivate(new QNetworkAccessManager(this), mediawiki))
{
    setCapabilities(KJob::NoCapabilities);
}

UserGroups::~UserGroups()
{
    delete d;
}

void UserGroups::start()
{
    QTimer::singleShot(0, this, SLOT(doWorkSendRequest()));
}

void UserGroups::doWorkSendRequest()
{
    // Set the url
    QUrl url = d->mediawiki.url();
    url.addQueryItem("format", "xml");
    url.addQueryItem("action", "query");
    url.addQueryItem("meta",   "siteinfo");
    url.addQueryItem("siprop", "usergroups");
    // Set the request
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "mediawiki-silk");
    // Send the request
    d->manager->get(request);
    connect(d->manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(doWorkProcessReply(QNetworkReply *)));
}

void UserGroups::doWorkProcessReply(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        setError(KJob::NoError);
        emitResult();
        emit result(this, QList<UserGroups::Result>());
    }
}
