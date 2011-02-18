/*
 *   Copyright 2010 by Hormiere Guillaume <hormiere.guillaume@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
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

#ifndef QUERYREVISION_H
#define QUERYREVISION_H

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>

#include "job.h"
#include "revision.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class QueryRevisionPrivate;

/**
 * @brief QueryRevision job.
 *
 * Uses for fetch a revision information about one pages of the wiki.
 */
class MEDIAWIKI_EXPORT QueryRevision : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(QueryRevision)

public:

    enum Dir {Older, Newer};

    enum Token{Rollback};

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum {

        /**
         * @brief The revids= parameter may not be used with the list options (limit, startid, endid, dirNewer, start, end).
         */
        WrongRevisionId = Job::UserDefinedError + 1,

        /**
         * @brief titles, pageids or a generator was used to supply multiple pages, but the limit, startid, endid, dirNewer, user, excludeuser, start and end parameters may only be used on a single page.
         */
        MultiPagesNotAllowed,

        /**
         * @brief The current user is not allowed to read title.
         */
        TitleAccessDenied,

        /**
         * @brief start and startid or end and endid or user and excludeuser cannot be used together
         */
        TooManyParams,

        /**
         * @brief There is no section section in rrevid
         */
        SectionNotFound

    };

    /**
     * @brief Property.
     */
    enum Prop {
        Ids         = 0x01,
        Flags       = 0x02,
        Timestamp   = 0x04,
        User        = 0x08,
        Comment     = 0x10,
        Size        = 0x20,
        Content     = 0x40
    };
    Q_DECLARE_FLAGS(Props, Prop)

    /**
     * @brief Constructs a Revision job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryRevision(MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the QueryRevision job.
     */
    virtual ~QueryRevision();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @param Id
     * @param QueryRevision::IdType
     */
    void setPageId(unsigned int);

    /**
     * @param Id
     * @param QueryRevision::IdType
     */
    void setRevisionId(unsigned int);

    /**
     *
     * @param Qstring title
     */
    void setPageName(const QString &);

    /**
     * @brief Which properties to get for each revision.
     * @param int
     */
    void setProp(Props properties); //FIXME: No abreviation

    /**
     * @brief When more results are available, use this to continue.
     * @param int
     */
    void setContinue(int);

    /**
     * @brief The maximum number of revisions to return.
     * @param int
     */
    void setLimit(int);

    /**
     * @brief Revision ID to stop listing at.
     * @param int
     */
    void setStartId(int);

    /**
     * @brief Revision ID to start listing from.
     * @param int
     */
    void setEndId(int);

    /**
     * @brief Timestamp to start listing from.
     * @param QDateTime
     */
    void setStart(const QDateTime &);

    /**
     * @brief Timestamp to stop listing at.
     * @param QDateTime
     */
    void setEnd(const QDateTime &);

    /**
     * @brief Only list revisions made by this user.
     * @param QString
     */
    void setUser(const QString &);

    /**
     * @brief Do not list revisions made by this user.
     * @param QString
     */
    void setExcludeUser(const QString &);

    /**
     * @brief Direction to list in.
     * older: List newest revisions first.
     * newer: List oldest revisions first.
     * @param QueryRevision::Dir
     */
    void setDir(QueryRevision::Dir);

    /**
     * @brief Set XML generation to parse tree for revision content.
     * @param bool
     */
    void setGenerateXML(bool);

    /**
     * @brief If rvprop=content is set, only retrieve the contents of this section. NOTE: Only implemented in MediaWiki versions 1.13 and above.
     * @param int
     */
    void setSection(int);

    /**
     * @brief Tokens to get for each revision
     * @param QueryRevision::Token
     */
    void setToken(QueryRevision::Token);

    /**
     * @brief Expand templates in rvprop=content output
     * @param bool
     */
    void setExpandTemplates(bool);

signals:

    /**
     * @brief Provides a list of all user groups.
     * @param revision list of all user groups
     * @see QueryRevision::Result
     */
    void revision(const QList<Revision> & revision);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply();

private:

    QueryRevision(const QueryRevision &);

    QueryRevision & operator=(const QueryRevision &);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QueryRevision::Props)

}
#endif //QUERYREVISION_H
