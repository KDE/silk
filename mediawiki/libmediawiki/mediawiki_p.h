/*
 *   Copyright 2009 by Rémi Benoit <r3m1.benoit@gmail.com>
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

#ifndef MEDIAWIKI_P_H
#define MEDIAWIKI_P_H

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

namespace mediawiki {

class MediaWikiPrivate {

public:

    MediaWikiPrivate(const QUrl & url, const QString & userAgent, QNetworkAccessManager * const manager)
        : url(url)
        , userAgent(userAgent)
        , manager(manager)
    {}

    ~MediaWikiPrivate()
    {
        delete manager;
    }

    static const QString POSTFIX_USER_AGENT;

    const QUrl url;
    const QString userAgent;
    QNetworkAccessManager * const manager;

};

}

#endif // MEDIAWIKI_P_H
