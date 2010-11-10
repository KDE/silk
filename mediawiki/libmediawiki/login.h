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

#ifndef MEDIAWIKI_LOGIN_H
#define MEDIAWIKI_LOGIN_H

#include <QtCore/QString>
#include <KDE/KJob>
#include <kdemacros.h>

class QNetworkReply;

namespace mediawiki {
    
class MediaWiki;

/**
 * @brief Login job.
 *
 * Uses for log in a user.
 */
class KDE_EXPORT Login : public KJob
{

    Q_OBJECT

public:

    enum
    {
        /**
         * @brief
         */
        Falsexml = KJob::UserDefinedError+1,

        /**
         * @brief
         */
        ConnectionAbort,

        /**
         * @brief You didn't set the lgname parameter
         */
        NoName,

        /**
         * @brief You provided an illegal username
         */
        Illegal,

        /**
         * @brief The username you provided doesn't exist
         */
        NotExists,

        /**
        * @brief You didn't set the lgpassword parameter or you left it empty
        */
        EmptyPass,

        /**
        * @brief The password you provided is incorrect
        */
        WrongPass,

        /**
        * @brief Same as WrongPass, returned when an authentication plugin rather than MediaWiki itself rejected the password
        */
        WrongPluginPass,

        /**
        * @brief The wiki tried to automatically create a new account for you, but your IP address has been blocked from account creation
        */
        CreateBlocked,

        /**
        * @brief You've logged in too many times in a short time.
        */
        Throttled,

        /**
        * @brief User is blocked
        */
        Blocked,

        /**
        * @brief Either you did not provide the login token or the sessionid cookie. Request again with the token and cookie given in this response
        */
        NeedToken,

    };

    /**
     * @brief A login result.
     */
    struct Result
    {

        /**
         * @brief The username.
         */
        QString lgname;

        /**
         * @brief The password.
         */
        QString lgpassword;

        /**
         * @brief The session id.
         */
        QString lgsessionid;

        /**
         * @brief The token.
         */
        QString lgtoken;

    };

    /**
     * @brief Return all parameters defined in the result struct.
     */
    Login::Result getResults();

    /**
     * @brief Constructs a Login job.
     * @param mediawiki the mediawiki concerned by the job
     * @param lgname the QObject parent
     * @param lgpassword the QObject parent
     * @param parent the QObject parent
     */
    explicit Login( MediaWiki const & media, const QString & lgname, const QString & lgpassword, QObject * parent = 0 );

    /**
     * @brief Destroys the Login job.
     */
    virtual ~Login();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Get the error number.
     * @param error the error sent by the API.
     */
    int getError(const QString & error);

signals:

    /**
     * @brief Emitted when a connection request has been completed.
     * @param success true if the request was completed successfully.
     */
    void resultLogin( KJob * job );

    /**
     * @brief Emitted when a connection has been completed.
     * @param success true if the connection was completed successfully.
     */
    void resultToken( KJob * job );

private slots:

    /**
     * @brief Destroy the connection.
     */
    void abort();

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
    void finishedLogin( QNetworkReply *reply );

    /**
     * @brief Reads the xml
     * if the attribute value is equal to "Success", the user is logged in
     * @param success true if the connection was completed successfully.
     */
    void finishedToken( QNetworkReply *reply );

private:

    /**
     * @brief Contains the class attributes.
     */
    struct LoginPrivate * const d;
};

}

#endif // LOGIN_H
