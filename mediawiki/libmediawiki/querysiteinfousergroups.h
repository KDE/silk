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

#ifndef MEDIAWIKI_QUERYSITEINFOUSERGROUPS_H
#define MEDIAWIKI_QUERYSITEINFOUSERGROUPS_H

#include <job.h>
#include "mediawiki_export.h"
#include "usergroup.h"

class QNetworkReply;

namespace mediawiki
{

class MediaWiki;
class QuerySiteinfoUsergroupsPrivate;

/**
 * @brief UserGroups job.
 *
 * Uses for fetch a list of all user groups and their permissions.
 */
class MEDIAWIKI_EXPORT QuerySiteinfoUsergroups : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(QuerySiteinfoUsergroups)

public:

    /**
     * @brief Constructs a UserGroups job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QuerySiteinfoUsergroups(MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the UserGroups job.
     */
    virtual ~QuerySiteinfoUsergroups();

    /**
     * @brief If true number of users of each user group is included
     * @param includeNumber if true number of users of each user group is included
     */
    void setIncludeNumber(bool includeNumber);

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

signals:

    /**
     * @brief Provides a list of all user groups.
     * @param usergroups list of all user groups
     * @see QuerySiteinfoUsergroups::Result
     */
    void usergroups(const QList<UserGroup> & usergroups);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    QuerySiteinfoUsergroups(const QuerySiteinfoUsergroups &);

    QuerySiteinfoUsergroups & operator=(const QuerySiteinfoUsergroups &);

};

}

#endif
