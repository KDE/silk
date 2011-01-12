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

#ifndef MEDIAWIKI_EDIT_H
#define MEDIAWIKI_EDIT_H

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/QUrl>
#include <KDE/KJob>
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Edit job.
 *
 * Uses for create or edit a wiki.
 */
class MEDIAWIKI_EXPORT Edit : public KJob
{

Q_OBJECT

public:

    enum
    {
        /**
         * @brief
         */
        BadXml = KJob::UserDefinedError+1,

        /**
         * @brief
         */
        ConnectionAborted,

        /**
         * @brief
         */
        TextMissing,

        /**
         * @brief
         */
        InvalidSection,

        /**
         * @brief
         */
        TitleProtected,

        /**
         * @brief
         */
        CreatePagePermissionMissing,

        /**
         * @brief
         */
        AnonymousCreatePagePermissionMissing,

        /**
         * @brief
         */
        ArticleDuplication,

        /**
         * @brief
         */
        AnonymousCreateImagePermissionMissing,

        /**
         * @brief
         */
        CreateImagePermissionMissing,

        /**
         * @brief
         */
        SpamDetected,

        /**
         * @brief
         */
        Filtered,

        /**
         * @brief
         */
        ArticleSizeExceed,

        /**
         * @brief
         */
        AnonymousEditPagePermissionMissing,

        /**
         * @brief
         */
        EditPagePermissionMissing,

        /**
         * @brief
         */
        PageDeleted,

        /**
         * @brief
         */
        EmptyPage,

        /**
         * @brief
         */
        EmptySection,

        /**
         * @brief
         */
        EditConflict,

        /**
         * @brief
         */
        RevWrongPage,

        /**
         * @brief
         */
        UndoFailed,

    };

    /**
     * @brief An edit result.
     */
    struct Result
    {
        /**
         * @brief CAPTCHA ID from previous request.
         */
        unsigned int CaptchaId;

        /**
         * @brief Question from the CAPTCHA.
         */
        QVariant CaptchaQuestion;

        /**
         * @brief Answer to the CAPTCHA.
         */
        QString CaptchaAnswer;
    };

    /**
     * @brief Constructs an Edit job.
     * @param media the mediawiki concerned by the job
     * @param title the page title
     * @param token the edit token. Gotten through prop=info.
     * @param basetimestamp the timestamp of the base revision. Gotten through prop=revisions&rvprop=timestamp.
     * @param starttimestamp the timestamp when you obtained the edit token.
     * @param text the page content
     * @param parent the QObject parent
     * @pre !title.isEmpty()
     * @pre !token.isEmpty()
     */
    explicit Edit( MediaWiki  & media, const QString& title, const QString& token, const QString& basetimestamp, const QString& starttimestamp, const QString& text, QObject *parent = 0);

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
     * @pre !title.isEmpty()
     * @pre !token.isEmpty()
     */
    explicit Edit( MediaWiki  & media, const QString& title, const QString& token, const QString& basetimestamp, const QString& starttimestamp, const QString& appendtext, const QString& prependtext, QObject *parent = 0);

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
     * @pre !title.isEmpty()
     * @pre !token.isEmpty()
     */
    explicit Edit( MediaWiki  & media, const QString& title, const QString& token, const QString& basetimestamp, const QString& starttimestamp, unsigned int undo, unsigned int undoafter = 0, QObject *parent = 0);

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
     * @brief Specify how the watchlist is affected by this edit.
     * @param watchlist Specify how the watchlist is affected by this edit
     */
    void setWatchlist(Edit::Watchlist watchlist);

    /**
     * @brief If set, suppress errors about the page having been deleted in the meantime and recreate it.
     */
    void setRecreate();

    /**
     * @brief If set, throw an error if the page already exists.
     */
    void setCreateonly();

    /**
     * @brief If set, throw a missingtitle error if the page doesn't exist.
     */
    void setNocreate();

    /**
     * @brief If set to true, mark the edit as minor
     * @param minor If set to true, mark the edit as minor
     */
    void setMinor(bool minor);

    /**
     * @brief Set the section.
     * @param section Set the section. New or integer
     */
    void setSection(const QString& section);

    /**
     * @brief Set the summary.
     * @param summary Set the summary
     */
    void setSummary(const QString& summary);
signals:

    /**
     * @brief Emitted when a connection has been completed.
     * @param success true if the connection was completed successfully.
     */
    void resultCaptcha(const QVariant & captcha);

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
    void finishedCaptcha( const QString & captcha );

private:

    /**
     * @brief Contains the class attributes.
     */
    struct EditPrivate * const d;
};
}
#endif // EDIT_H
