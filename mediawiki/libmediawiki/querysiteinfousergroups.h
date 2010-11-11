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
     * @biref Indicates all possible error conditions found during the processing of the job.
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

    };

    /**
     * @brief A user group result.
     */
    struct Result {

    public:

        /**
         * Constructs a user group.
         * @param name the name of the user group
         * @param rights rights of the user group
         */
        Result(QString const & name, QList<QString> const & rights)
                : m_name(name)
                , m_rights(rights)
                , m_hasNumber(false)
        {}

        /**
         * Constructs a user group.
         * @param name the name of the user group
         * @param rights rights of the user group
         * @param number the numbers of users in the user group
         */
        Result(QString const & name, QList<QString> const & rights, unsigned int const number)
                : m_name(name)
                , m_rights(rights)
                , m_number(number)
                , m_hasNumber(true)
        {}

        /**
         * @brief Returns the name of the user group.
         * @return the name of the user group
         */
        inline QString name() const { return m_name; }

        /**
         * @brief Returns rights of the user group.
         * @return rights of the user group
         */
        inline QList<QString> const & rights() const { return m_rights; }

        /**
         * @brief Returns the numbers of users in the user group.
         * @pre hasNumber() == true
         * @return the numbers of users in the user group
         * @see QuerySiteinfoUsergroups::Result::hasNumber()
         */
        inline unsigned int number() const { Q_ASSERT(hasNumber()); return m_number; }

        /**
         * @brief Returns true if number has defined else false.
         * @return true if number has defined else false
         * @see QuerySiteinfoUsergroups::Result::number()
         */
        inline bool hasNumber() const { return m_hasNumber; }

    private:

        QString m_name;

        QList<QString> m_rights;

        unsigned int m_number;

        bool m_hasNumber;

    };

    /**
     * @brief Constructs a UserGroups job.
     * @param mediawiki the mediawiki concerned by the job
     * @param includeNumber if true number of users of each user group is included
     * @param parent the QObject parent
     */
    explicit QuerySiteinfoUsergroups(MediaWiki const & mediawiki, bool includeNumber = false, QObject * parent = 0);

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
     * @see QuerySiteinfoUsergroups::Result
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
