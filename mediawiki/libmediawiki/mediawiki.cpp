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

#include "mediawiki.h"

namespace mediawiki
{

struct MediaWikiPrivate {

    MediaWikiPrivate(QNetworkAccessManager *  manager, const QUrl & url, const QString & userAgent) : manager(manager), url(url), userAgent(userAgent) {}

    const QUrl url;

    const QString userAgent;
    QNetworkAccessManager * manager;
};

}

using namespace mediawiki;

MediaWiki::MediaWiki(const QUrl & url, const QString & customUserAgent)
    : d_ptr(new MediaWikiPrivate(url,
                                 (customUserAgent.isEmpty() ? "" : customUserAgent + "-") + MediaWikiPrivate::POSTFIX_USER_AGENT,
                                 new QNetworkAccessManager()))

{}

MediaWiki::~MediaWiki()
{
    delete d_ptr;
}

QUrl MediaWiki::url() const
{
    return d_ptr->url;
}

QString MediaWiki::userAgent() const
{
    return d_ptr->userAgent;
}

const QString MediaWikiPrivate::POSTFIX_USER_AGENT = "mediawiki-silk";
