/*
 *   Copyright 2011 by Alexandre Mendes <alex.mendes1988@gmail.com>
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

#ifndef MEDIAWIKI_UPLOAD_H
#define MEDIAWIKI_UPLOAD_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/QUrl>

#include "mediawiki_export.h"
#include "job.h"
#include "queryinfo.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class UploadPrivate;

/**
 * @brief Upload job.
 *
 * Uses for upload files.
 */
class MEDIAWIKI_EXPORT Upload : public Job {

    Q_OBJECT
    Q_DECLARE_PRIVATE(Upload)

public:

    enum
    {

        /**
         * @brief An internal error occurred.
         */
        InternalError= Job::UserDefinedError+1,
        /**
         * @brief The module is disabled.
         */
        UploadDisabled,

        /**
         * @brief The session key is invalid.
         */
        InvalidSessionKey,

        /**
         * @brief The current user can't upload.
         */
        BadAccess,

        /**
         * @brief A param is missing.
         */
        ParamMissing,

        /**
         * @brief No upload without logged in.
         */
        MustBeLoggedIn,

        /**
         * @brief
         */
        FetchFileError,

        /**
         * @brief No upload module set.
         */
        NoModule,

        /**
         * @brief The file submitted was empty.
         */
        EmptyFile,

        /**
         * @brief The file is missing an extension.
         */
        ExtensionMissing,

        /**
         * @brief The filename is too short.
         */
        TooShortFilename,

        /**
         * @brief Overwriting an existing file is not allowed.
         */
        OverWriting,

        /**
         * @brief Stashing temporary file failed.
         */
        StashFailed,

    };

    /**
     * @brief Constructs an Upload job.
     * @param parent the QObject parent
     */
    Upload( MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys the Upload job.
     */
    virtual ~Upload();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Set the target filename.
     * @param filename the filename
     */
    void setFilename(const QString&);

    /**
     * @brief Set the file.
     * @param file the file
     */
    void setFile(QIODevice*);

    /**
     * @brief Set the upload comment. Also used as the initial page text for new files if text parameter not provided.
     * @param comment the comment
     */
    void setComment(const QString&);

    /**
     * @brief Set the information of the image. Use this template {{Information|Description=|Source=|Date=|Author=|Permission=|other_versions=}}.
     * @param text the text
     */
    void setText(const QString&);

private slots:

    void doWorkSendRequest(Page page);

    void doWorkProcessReply();

private:

    int getError(const QString & error);

};
}
#endif // UPLOAD_H
