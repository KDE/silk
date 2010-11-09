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

    MediaWikiPrivate(QUrl const & url, QString const & userAgent) : url(url), userAgent(userAgent) {}

    QUrl const url;

    QString const userAgent;

};

}

using namespace mediawiki;

MediaWiki::MediaWiki(QUrl const & url, QString const & customUserAgent)
    : d(new MediaWikiPrivate(url, customUserAgent.isEmpty() ? MediaWiki::DEFAULT_USER_AGENT : customUserAgent + "-" + MediaWiki::DEFAULT_USER_AGENT))
{}

QUrl MediaWiki::url() const
{
    return d->url;
}

QString MediaWiki::userAgent() const
{
    return d->userAgent;
}

QString const MediaWiki::DEFAULT_USER_AGENT = "mediawiki-silk";
