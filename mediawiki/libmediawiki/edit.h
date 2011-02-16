/*
 *   Copyright 2010 by Alexandre Mendes <alex.mendes1988@gmail.com>
 *   Copyright 2011 by Manuel Campomanes <campomanes.manuel@gmail.com>
 *   Copyright 2011 by Hormiere Guillaume <hormiere.guillaume@gmail.com>
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

#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkCookieJar>

#include "job.h"
#include "queryinfo.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class EditPrivate;

/**
 * @brief Edit job.
 *
 * Uses for create or edit a wiki.
 */
class MEDIAWIKI_EXPORT Edit : public Job
{

    Q_OBJECT
    Q_DECLARE_PRIVATE(Edit)

public:

    enum
    {
        /**
         * @brief
         */
        TextMissing = Job::UserDefinedError+1,

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
     * @brief Constructs an Edit job.
     * @param parent the QObject parent
     */
    explicit Edit( MediaWiki & media, QObject * parent = 0);
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
    //FIXME: virer les espaces
    /**
     * @brief Specify how the watchlist is affected by this edit.
     * @param watchlist Specify how the watchlist is affected by this edit
     */
    void setWatchList(Edit::Watchlist watchlist);

    /**
     * @brief If set, suppress errors about the page having been deleted in the meantime and recreate it.
     */
    void setRecreate(bool);

    /**
     * @brief If set, throw an error if the page already exists.
     */
    void setCreateonly(bool);

    /**
     * @brief If set, throw a missingtitle error if the page doesn't exist.
     */
    void setNocreate(bool);

    /**
     * @brief If set to true, mark the edit as minor
     * @param minor If set to true, mark the edit as minor
     */
    void setMinor(bool);

    /**
     * @brief Set the section.
     * @param section Set the section. New or integer
     */
    void setSection(const QString&);

    /**
     * @brief Set the summary.
     * @param summary Set the summary
     */
    void setSummary(const QString&);

    /**
     * @brief Which properties .
     * @param undoafter Undo all revisions from undo to this one. If not set, just undo one revision.
     */
    void setUndoAfter( int );
    /**
     * @brief Which properties .
     * @param undo Undo this revision. Overrides text, prependtext and appendtext.
     */
    void setUndo( int );
    /**
     * @brief Which properties .
     * @param prependtext the text added to the beginning of the page. Overrides text.
     */
    void setPrependText( const QString& );
    /**
     * @brief Which properties .
     * @param appendtext the text added to the end of the page. Overrides text.
     */
    void setAppendText( const QString& );
    /**
     * @brief Which properties .
     * @param title the page title
     */
    void setPageName( const QString&);
    /**
     * @brief Which properties .
     * @param token the edit token. Gotten through prop=info.
     */
    void setToken( const QString&);
    /**
     * @brief Which properties .
     * @param basetimestamp the timestamp of the base revision. Leave unset to ignore conflit.
     */
    void setBaseTimesStamp( const QDateTime&);
    /**
     * @brief Which properties .
     * @param  starttimestamp the timestamp when you obtained the edit token.
     */
    void setStartTimesStamp( const QDateTime&);
    /**
     * @brief Which properties .
     * @param text the page content.
     */

    void setText( const QString&);

signals:

    /**
     * @brief Emitted when a connection has been completed.
     * @param success true if the connection was completed successfully.
     */
    void resultCaptcha(const QVariant & captcha);

private slots:

    /**
     * @brief Send a request.
     */
    void doWorkSendRequest(Page page);

    /**
     * @brief Reads the xml
     * @param success true if the connection was completed successfully.
     */
    void finishedEdit();

public slots:
    /**
     * @brief Reads the xml
     * @param success true if the connection was completed successfully.
     */
    void finishedCaptcha( const QString & captcha );

};

}

#endif // EDIT_H
