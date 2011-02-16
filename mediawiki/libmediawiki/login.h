/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef MEDIAWIKI_LOGIN_H
#define MEDIAWIKI_LOGIN_H

#include <QtCore/QString>
#include <QtNetwork/QNetworkCookieJar>

#include "job.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {
    
class MediaWiki;
class LoginPrivate;

/**
 * @brief Login job.
 *
 * Uses for log in a user.
 */
class MEDIAWIKI_EXPORT Login : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(Login)

public:

    enum
    {
        /**
         * @brief
         */
        BadXml = Job::UserDefinedError + 1,

        /**
         * @brief You didn't set the login parameter
         */
        LoginMissing,

        /**
         * @brief You provided an illegal username
         */
        IllegalUsername,

        /**
         * @brief The username you provided doesn't exist
         */
        UsernameNotExists,

        /**
        * @brief You didn't set the lgpassword parameter or you left it empty
        */
        PasswordMissing,

        /**
        * @brief The password you provided is incorrect
        */
        WrongPassword,

        /**
        * @brief Same as WrongPass, returned when an authentication plugin rather than MediaWiki itself rejected the password
        */
        WrongPluginPassword,

        /**
        * @brief The wiki tried to automatically create a new account for you, but your IP address has been blocked from account creation
        */
        IPAddressBlocked,

        /**
        * @brief You've logged in too many times in a short time.
        */
        TooManyConnections,

        /**
        * @brief User is blocked
        */
        UserBlocked,

        /**
        * @brief Either you did not provide the login token or the sessionid cookie. Request again with the token and cookie given in this response
        */
        TokenNeeded,

    };    

    /**
     * @brief Constructs a Login job.
     * @param mediawiki the mediawiki concerned by the job
     * @param login the QObject parent
     * @param password the QObject parent
     * @param parent the QObject parent
     */    
    explicit Login(MediaWiki & mediawiki, const QString & login, const QString & password, QObject * parent = 0);

    /**
     * @brief Destroys the Login job.
     */
    virtual ~Login();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();    

private slots:

    /**
     * @brief Send a request to get the token and the cookie.
     */
    void doWorkSendRequest();

    /**
     * @brief Reads the xml
     * if the attribute value is equal to "NeedToken", try to log in the user
     * else if the attribute value is equal to "Success", the user is logged in
     * @param success true if the connection was completed successfully.
     */
    void doWorkProcessReply();

private:

    /**
     * @brief Get the error number.
     * @param error the error sent by the API.
     */
    //FIXME: getError -> error
    int getError(const QString & error);
};

}

#endif // LOGIN_H
