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
class UserGroups : public KJob
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
    explicit UserGroups(MediaWiki const & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the UserGroups job.
     */
    virtual ~UserGroups();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

signals:

    /**
     * @brief Emitted when the job is finished.
     * @param job the job that emitted this signal
     * @param results a list of all user groups and their permissions
     */
    void result(UserGroups * job, QList<UserGroups::Result> const & results);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct UserGroupsPrivate * const d;

private:

    UserGroups(UserGroups const &);

    UserGroups & operator=(UserGroups const &);

};

}

#endif
