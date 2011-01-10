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

#ifndef MEDIAWIKI_MEDIAWIKI_H
#define MEDIAWIKI_MEDIAWIKI_H

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include "mediawiki_export.h"

namespace mediawiki
{

/**
 * @brief Provides access to wiki powered by MediaWiki.
 */
    class MEDIAWIKI_EXPORT MediaWiki:QObject
{
Q_OBJECT
public:

    /**
     * @brief Constructs a MediaWiki by its url api.
     * @param url the url api of the wiki
     * @param customUserAgent you can specify the user agent to use
                              wich will be concatenated with the default user agent
     *                        else the default user agent is used only
     */
    explicit MediaWiki(QUrl const & url, QString const & customUserAgent = QString());

    /**
     * @brief Returns the url api of the wiki.
     * @returns the url api of the wiki
     */
    QUrl url() const;

    /**
     * @brief Returns the user agent of the wiki.
     * @return the user agent of the wiki
     */
    QString userAgent() const;

    /**
     * @brief The default user agent.
     */
    static QString const DEFAULT_USER_AGENT;

    /**
     * @brief Returns the connection manager.
     * @return the connection manager
     */
    QNetworkAccessManager* manager();

    /**
     * @brief Returns the cookies created by the login.
     * @return the cookies created by the login
     */
    QList<QNetworkCookie>  cookies() const;

private:

    struct MediaWikiPrivate * const d;

private:

    MediaWiki(MediaWiki const &);

    MediaWiki & operator=(MediaWiki const &);

};

}

#endif
