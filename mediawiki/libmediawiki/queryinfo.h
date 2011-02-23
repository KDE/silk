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

#ifndef QUERYINFO_H
#define QUERYINFO_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>

#include "page.h"
#include "protection.h"
#include "mediawiki_export.h"
#include "job.h"

class QNetworkReply;

namespace mediawiki
{

class MediaWiki;
class QueryInfoPrivate;

/**
 * @brief QueryInfo job.
 *
 * Uses to send a request to get basic page information.
 */
class MEDIAWIKI_EXPORT QueryInfo : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(QueryInfo)

public:

    /**
     * @brief Constructs a QueryInfo job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryInfo(MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QueryInfo();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Set the page name.
     * @param title the page name
     */
    void setPageName(const QString & title);

    /**
     * @brief Set the token to perform a data-modifying action on a page
     * @param token the token
     */
    void setToken(const QString & token);

    /**
     * @brief Set the page id.
     * @param id the page id
     */
    void setPageId(unsigned int id);

    /**
     * @brief Set the page revision
     * @param id the page revision
     */
    void setRevisionId(unsigned int id);

signals:

    /**
     * @brief Provides a page
     * @param
     */
    void page(const Page & p);
    void protection(const QVector <Protection> & protect);

private slots:

    /**
     * @brief Send a request.
     */
    void doWorkSendRequest();

    void doWorkProcessReply();

};

}
#endif // QUERYINFO_H
