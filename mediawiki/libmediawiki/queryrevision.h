/*
 *   Copyright 2010 by Hormiere Guillaume <hormiere.guillaume@gmail.com>
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
#include <QObject>
#include <QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>

#include <KDE/KJob>

#define IDS         1
#define FLAGS       2
#define TIMESTAMP   4
#define USER        8
#define COMMENT     16
#define SIZE        32
#define CONTENT     64

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief QueryRevision job.
 *
 * Uses for fetch a revision information about one pages of the wiki.
 */
class QueryRevision : public KJob
{
    Q_OBJECT

public:
    enum IdType{ pageids , revids , noid };

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum {

        /**
         * @brief A network error has occured.
         */
        NetworkError = KJob::UserDefinedError + 1,
        /**
         * @brief A XML error has occured.
         */
        XmlError,
        /**
         * @brief The revids= parameter may not be used with the list options (limit, startid, endid, dirNewer, start, end).
         */
        RevIds,
        /**
         * @brief titles, pageids or a generator was used to supply multiple pages, but the limit, startid, endid, dirNewer, user, excludeuser, start and end parameters may only be used on a single page.
         */
        MultPages,
        /**
         * @brief The current user is not allowed to read title.
         */
        AccessDenied,
        /**
         * @brief start and startid or end and endid or user and excludeuser cannot be used together
         */
        AddParams,
        /**
         * @brief There is no section section in rrevid
         */
        NoSuchSection

    };
    /**
     * @brief A user group result.
     */
    struct Result {
        Result(){}
        Result(int r, int p, int s, QString m, QString u, QDateTime t, QString cm, QString ct){
            revid=r;
            parentId=p;
            size=s;
            minor=m;
            user=u;
            timeStamp=t;
            comment=cm;
            content=ct;
        }
        int revid;
        int parentId;
        int size;
        QString minor;
        QString user;
        QDateTime timeStamp;
        QString comment;
        QString content;
    };
    /**
     * @brief Constructs a Revision job.
     * @param mediawiki the mediawiki concerned by the job
     * @param title the pages title
     * @param requestParameter map the parameter name and values
     * @param parent the QObject parent
     */
    explicit QueryRevision(MediaWiki const & mediawiki, QString title, QObject * parent = 0);

    /**
     * @brief Constructs a Revision job.
     * @param mediawiki the mediawiki concerned by the job
     * @param title the pages title
     * @param requestParameter map the parameter name and values
     * @param parent the QObject parent
     */
    explicit QueryRevision(MediaWiki const & mediawiki, int id, QueryRevision::IdType, QObject * parent = 0);

    /**
     * @brief Destroys the QueryRevision job.
     */
    virtual ~QueryRevision();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Which properties to get for each revision.
     * @param int
     */
    void setRvProp(int);

signals:

    /**
     * @brief Provides a list of all user groups.
     * @param revision list of all user groups
     * @see QueryRevision::Result
     */
    void revision(QList<QueryRevision::Result> const & revision);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct QueryRevisionPrivate * const d;

private:

    QueryRevision(QueryRevision const &);

    QueryRevision & operator=(QueryRevision const &);

};
}
#endif //QUERYREVISION_H
