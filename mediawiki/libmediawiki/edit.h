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
#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <KDE/KJob>
#include <kdemacros.h>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Edit job.
 *
 * Uses for create or edit a wiki.
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
        cantcreateanon,

        /**
         * @brief
         */
        articleexists,

        /**
         * @brief
         */
        noimageredirectanon,

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
        noeditanon,

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
        undofailure,

    };

    /**
     * @brief An edit result.
     */
    struct Result
    {
        /**
         * @brief CAPTCHA ID from previous request.
         */
        unsigned int captchaid;

        /**
         * @brief Question from the CAPTCHA.
         */
        QVariant captchaquestionorurl;

        /**
         * @brief Answer to the CAPTCHA.
         */
        QString captchaword;
    };

    /**
     * @brief Return all parameters defined in the result struct.
     */
    Edit::Result getResults();

    /**
     * @brief Constructs an Edit job.
     * @param media the mediawiki concerned by the job
     * @param title the page title
     * @param token the edit token. Gotten through prop=info.
     * @param basetimestamp the timestamp of the base revision. Gotten through prop=revisions&rvprop=timestamp.
     * @param starttimestamp the timestamp when you obtained the edit token.
     * @param text the page content
     * @param section the section number. 0 for the top section, 'new' for a new section.
     * @param summary the edit summary. Also section title when section=new.
     * @param parent the QObject parent
     */
    explicit Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString const & text, QString const & section = QString(), QString const & summary = QString(), QObject *parent = 0);

    /**
     * @brief Constructs an Edit job.
     * @param media the mediawiki concerned by the job
     * @param title the page title
     * @param token the edit token. Gotten through prop=info.
     * @param basetimestamp the timestamp of the base revision. Gotten through prop=revisions&rvprop=timestamp.
     * @param starttimestamp the timestamp when you obtained the edit token.
     * @param appendtext the text added to the end of the page. Overrides text.
     * @param prependtext the text added to the beginning of the page. Overrides text.
     * @param parent the QObject parent
     */
    explicit Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString const & appendtext, QString const & prependtext, QObject *parent = 0);

    /**
     * @brief Constructs an Edit job.
     * @param media the mediawiki concerned by the job
     * @param title the page title
     * @param token the edit token. Gotten through prop=info.
     * @param basetimestamp the timestamp of the base revision. Gotten through prop=revisions&rvprop=timestamp.
     * @param starttimestamp the timestamp when you obtained the edit token.
     * @param undo Undo this revision. Overrides text, prependtext and appendtext.
     * @param undoafter Undo all revisions from undo to this one. If not set, just undo one revision.
     * @param parent the QObject parent
     */
    explicit Edit( MediaWiki const & media, QString const & title, QString const & token, QString basetimestamp, QString starttimestamp, QString, unsigned int undo, unsigned int undoafter = 0, QObject *parent = 0);

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


    /**
     * @brief Specify how the watchlist is affected by this edit.
     */
    enum Watchlist {        
        /**
         * @brief Add the page to the watchlist
         */
        watch,

        /**
         * @brief Remove the page from the watchlist
         */
        unwatch,

        /**
         * @brief Use the preference settings
         */
        preferences,

        /**
         * @brief Don't change the watchlist
         */
        nochange
    };

    /**
     * @brief Which properties to get for each revision.
     * @param watchlist Specify how the watchlist is affected by this edit
     * @param recreate If set, suppress errors about the page having been deleted in the meantime and recreate it
     * @param createonly If set, throw an error if the page already exists
     * @param nocreate If set, throw a missingtitle error if the page doesn't exist.
     * @param minor If set to true, mark the edit as minor
     */
    void setParams(Edit::Watchlist watchlist, bool recreate = false, bool createonly = false, bool nocreate = false, bool minor = false, bool notminor = false);

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
    void resultCaptcha(QVariant const & captcha);

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
     * @param success true if the connection was completed successfully.
     */
    void finishedEdit( QNetworkReply *reply );

public slots:
    /**
     * @brief Reads the xml
     * @param success true if the connection was completed successfully.
     */
    void finishedCaptcha( QString  const & captcha );

private:

    /**
     * @brief Contains the class attributes.
     */
    struct EditPrivate * const d;
};
}
#endif // EDIT_H