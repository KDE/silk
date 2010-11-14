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

#ifndef MEDIAWIKI_LOGOUT_H
#define MEDIAWIKI_LOGOUT_H

#include <QtCore/QString>
#include <KDE/KJob>
#include <kdemacros.h>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Logout job.
 *
 * Uses for log out a user.
 */
class KDE_EXPORT Logout : public KJob
{

    Q_OBJECT

public:
    enum
    {
        ConnectionAbort = KJob::UserDefinedError+1
    };

    /**
     * @brief Constructs a Logout job.
     * @param parent the QObject parent
     */
    explicit Logout(MediaWiki const & mediawiki, QObject *parent = 0);

    /**
     * @brief Destroys the Logout job.
     */
    virtual ~Logout();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

private slots:

    /**
     * @brief Destroy the connection.
     */
    void abort();

    /**
     * @brief Send a request to destroy the cookie.
     */
    void doWorkSendRequest();
    void doWorkProcessReply(QNetworkReply * reply);

private:

    /**
     * @brief Contains the class attributes.
     */
    struct LogoutPrivate * const d;

};
}
#endif // LOGOUT_H
