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

#include "job.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class LogoutPrivate;

/**
 * @brief Logout job.
 *
 * Uses for log out a user.
 */
class MEDIAWIKI_EXPORT Logout : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(Logout)

public:

    /**
     * @brief Constructs a Logout job.
     * @param parent the QObject parent
     */
    explicit Logout(MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the Logout job.
     */
    virtual ~Logout();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

private slots:

    void doWorkSendRequest();
    void doWorkProcessReply();

};

}
#endif // LOGOUT_H
