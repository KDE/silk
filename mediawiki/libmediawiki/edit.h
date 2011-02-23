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

    //FIXME: Comments ?
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
    explicit Edit(MediaWiki & media, QObject * parent = 0);
    /**
     * @brief Destroys the Edit job.
     */
    virtual ~Edit();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

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
    void setWatchList(Edit::Watchlist watchlist);

    /**
     * @brief If set, suppress errors about the page having been deleted in the meantime and recreate it.
     * @param recreate if set, suppress errors about the page having been deleted in the meantime and recreate it
     */
    void setRecreate(bool recreate);

    /**
     * @brief If set, throw an error if the page already exists.
     * @param if set, throw an error if the page already exists
     */
    void setCreateonly(bool createonly);

    /**
     * @brief If set, throw a missingtitle error if the page doesn't exist.
     * @param norecreate if set, throw a missingtitle error if the page doesn't exist
     */
    void setNocreate(bool norecreate);

    /**
     * @brief If set to true, mark the edit as minor
     * @param minor if set to true, mark the edit as minor
     */
    void setMinor(bool minor);

    /**
     * @brief Set the section.
     * @param section new section or integer
     */
    void setSection(const QString & section);

    /**
     * @brief Set the summary.
     * @param summary the summary
     */
    void setSummary(const QString & summary);

    /**
     * @brief Undo all revisions from undo to this one. If not set, just undo one revision.
     * @param undoafter if true set the undo after property
     */
    void setUndoAfter(int undoafter);

    /**
     * @brief Undo this revision. Overrides text, prependtext and appendtext.
     * @param undo if true set the undo
     */
    void setUndo(int undo);

    /**
     * @brief Set the text added to the beginning of the page. Overrides text.
     * @param prependText the text added to the beginning of the page
     */
    void setPrependText(const QString & prependText);

    /**
     * @brief Set the text added to the end of the page. Overrides text.
     * @param appendText the text added to the end of the page
     */
    void setAppendText(const QString & appendText);

    /**
     * @brief Set the page title.
     * @param pageName the page title
     */
    void setPageName(const QString & pageName);

    /**
     * @brief Set the edit token. Retrieve from QueryInfo.
     * @param token the edit token
     */
    void setToken(const QString & token);

    /**
     * @brief Set the timestamp of the base revision. Leave unset to ignore conflit.
     * @param baseTimestamp the timestamp of the base revision
     */
    void setBaseTimestamp(const QDateTime & baseTimestamp);

    /**
     * @brief Set the timestamp when you obtained the edit token.
     * @param startTimestamp the timestamp when you obtained the edit token
     */
    void setStartTimestamp(const QDateTime & startTimestamp);

    /**
     * @brief Set the page content.
     * @param text the page content.
     */
    void setText(const QString & text);

signals:

    /**
     * @brief Emit the captcha question.
     * @param captcha the captcha question
     */
    void resultCaptcha(const QVariant & captcha);

private slots:

    void doWorkSendRequest(Page page);

    void finishedEdit();

public slots:

    /**
     * @brief Put the captcha answer.
     * @param captcha the captcha answer
     */
    void finishedCaptcha(const QString & captcha);

};

}

#endif // EDIT_H
