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

#ifndef MEDIAWIKI_USERGROUPS_H
#define MEDIAWIKI_USERGROUPS_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <KDE/KJob>

class QNetworkReply;

namespace mediawiki
{

class MediaWiki;

/**
 * @brief UserGroups job.
 *
 * Uses for fetch a list of all user groups and their permissions.
 */
class QuerySiteinfoUsergroups : public KJob
{

    Q_OBJECT

public:

    /**
     * @brief A user group result.
     */
    struct Result {

        /**
         * @brief The name of the user group.
         */
        QString name;

        /**
         * @brief Rights of the user group.
         */
        QList<QString> rights;

    };

    /**
     * @brief Constructs a UserGroups job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QuerySiteinfoUsergroups(MediaWiki const & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the UserGroups job.
     */
    virtual ~QuerySiteinfoUsergroups();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

signals:

    /**
     * @brief Provides a list of all user groups.
     * @param usergroups list of all user groups
     */
    void usergroups(QList<QuerySiteinfoUsergroups::Result> const & usergroups);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct QuerySiteinfoUsergroupsPrivate * const d;

private:

    QuerySiteinfoUsergroups(QuerySiteinfoUsergroups const &);

    QuerySiteinfoUsergroups & operator=(QuerySiteinfoUsergroups const &);

};

}

#endif
