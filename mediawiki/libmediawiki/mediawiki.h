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

#ifndef MEDIAWIKI_H
#define MEDIAWIKI_H

#include <QtCore/QUrl>

/**
 * @brief Provides access to wiki powered by MediaWiki.
 */
class MediaWiki
{

public:

    /**
     * @brief Constructs a MediaWiki by its url api.
     * @param url the url api of the wiki
     */
    explicit MediaWiki(QUrl const & url);

    /**
     * @brief Returns the url api of the wiki
     * @returns the url api of the wiki
     */
    QUrl url() const;

private:

    struct MediaWikiPrivate * const d;

private:

    MediaWiki(MediaWiki const &);

    MediaWiki & operator=(MediaWiki const &);

};

#endif // MEDIAWIKI_H
