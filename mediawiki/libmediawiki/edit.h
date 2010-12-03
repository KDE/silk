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

#ifndef MEDIAWIKI_EDIT_H
#define MEDIAWIKI_EDIT_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <KDE/KJob>
#include <kdemacros.h>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Edit job.
 *
 * Uses for edit a wiki.
 */
class KDE_EXPORT Edit : public KJob
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
         * @brief
         */
        notitle,

        /**
         * @brief
         */
        notext,

        /**
         * @brief
         */
        notoken,

        /**
         * @brief
         */
        invalidsection,

        /**
         * @brief
         */
        protectedtitle,

        /**
         * @brief
         */
        cantcreate,

        /**
         * @brief
         */
        cantcreateAnon,

        /**
         * @brief
         */
        articleexists,

        /**
         * @brief
         */
        noimageredirectAnon,

        /**
         * @brief
         */
        noimageredirect,

        /**
         * @brief
         */
        spamdetected,

        /**
         * @brief
         */
        filtered,

        /**
         * @brief
         */
        contenttoobig,

        /**
         * @brief
         */
        noeditAnon,

        /**
         * @brief
         */
        noedit,

        /**
         * @brief
         */
        pagedeleted,

        /**
         * @brief
         */
        emptypage,

        /**
         * @brief
         */
        emptynewsection,

        /**
         * @brief
         */
        editconflict,

        /**
         * @brief
         */
        revwrongpage,

        /**
         * @brief
         */
        undoFailure,

    };

    /**
     * @brief A login result.
     */
    struct Result
    {

        /**
         * @brief Page title.
         */
        QString title;
        /**
         * @brief Section number. 0 for the top section, 'new' for a new section.
         */
        QString section;
        /**
         * @brief Page content.
         */
        QString text;
        /**
         * @brief Edit token. You can get one of these through prop=info.
         */
        QString token;
        /**
         * @brief Edit summary. Also section title when section=new.
         */
        QString summary;
        /**
         * @brief Minor edit.
         */
        bool minor;
        /**
         * @brief Non-minor edit.
         */
        bool notminor;
        /**
         * @brief Timestamp of the base revision (gotten through prop=revisions&rvprop=timestamp).
         */
        QDateTime basetimestamp;
        /**
         * @brief Timestamp when you obtained the edit token.
         */
        QDateTime starttimestamp;
        /**
         * @brief Override any errors about the article having been deleted in the meantime.
         */
        bool recreate;
        /**
         * @brief Don't edit the page if it exists already.
         */
        bool createonly;
        /**
         * @brief Throw an error if the page doesn't exist.
         */
        bool nocreate;
        /**
         * @brief Unconditionally add or remove the page from your watchlist, use preferences or do not change watch.
         */
        QString watchlist;
        /**
         * @brief CAPTCHA ID from previous request.
         */
        unsigned int captchaid;
        /**
         * @brief Answer to the CAPTCHA.
         */
        QString captchaword;
        /**
         * @brief The MD5 hash of the text parameter, or the prependtext and appendtext parameters concatenated. If set, the edit won't be done unless the hash is correct.
         */
        QString md5;
        /**
         * @brief Add this text to the beginning of the page. Overrides text.
         */
        QString prependtext;
        /**
         * @brief Add this text to the end of the page. Overrides text.
         */
        QString appendtext;
        /**
         * @brief Undo this revision. Overrides text, prependtext and appendtext.
         */
        unsigned int undo;
        /**
         * @brief Undo all revisions from undo to this one. If not set, just undo one revision.
         */
        unsigned int undoafter;
    };

    /**
     * @brief Return all parameters defined in the result struct.
     */
    Edit::Result getResults();

    /**
     * @brief Constructs an Edit job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit Edit( MediaWiki const & media, QString const & title, QString const & token, QString const & params, QObject *parent = 0);

    /**
     * @brief Destroys the Edit job.
     */
    virtual ~Edit();

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
    void resultEdit( KJob * job );

    /**
     * @brief Emitted when a connection has been completed.
     * @param success true if the connection was completed successfully.
     */
    void resultCaptcha( KJob * job );

private slots:

    /**
     * @brief Destroy the connection.
     */
    void abort();

    /**
     * @brief Send a request.
     */
    void doWorkSendRequest();

    /**
     * @brief Reads the xml
     * if the attribute value is equal to "NeedToken", try to log in the user
     * else if the attribute value is equal to "Success", the user is logged in
     * @param success true if the connection was completed successfully.
     */
    void finishedEdit( QNetworkReply *reply );

    /**
     * @brief Reads the xml
     * if the attribute value is equal to "Success", the user is logged in
     * @param success true if the connection was completed successfully.
     */
    void finishedCaptcha( QNetworkReply *reply );

private:

    /**
     * @brief Contains the class attributes.
     */
    struct EditPrivate * const d;
};
}
#endif // EDIT_H
