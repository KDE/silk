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

#ifndef QUERYINFO_H
#define QUERYINFO_H

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include <KDE/KJob>

#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki
{

class MediaWiki;

/**
 * @brief QueryInfo job.
 *
 * Uses to send a request to get basic page information.
 */
class MEDIAWIKI_EXPORT QueryInfo : public KJob
{

    Q_OBJECT

public:

    /**
     * @brief type of the id
     */
    enum IdType
    {
        RevisionId,
        PageId,
        NoId
    };

    enum
    {
        /**
         * @brief XML error
         */
        BadXml = KJob::UserDefinedError+1,

        /**
         * @brief Connection aborted
         */
        ConnectionAborted,

    };

    /**
     * @brief Constructs a QueryInfo job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryInfo(MediaWiki & mediawiki, QObject *parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QueryInfo();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    /**
     * @brief Set the page name.
     * @param title the page name
     */
    void setPageName(const QString & title);

    /**
     * @brief Set the token to perform a data-modifying action on a page
     * @param token the token
     */
    void setToken(const QString & token);

    /**
     * @brief Set the page id.
     * @param id the page id
     */
    void setPageId(unsigned int id);

    /**
     * @brief Set the page revision
     * @param id the page revision
     */
    void setRevisionId(unsigned int id);

    struct Protection {

    public:

        /**
         * Constructs a protection.
         * @param type the protection type
         * @param level the protection level
         * @param expiry the protection expiry
         * @param source the protection source
         */
        Protection(const QString & type, const QString & level, const QString & expiry, const QString & source)
                : m_type(type)
                , m_level(level)
                , m_expiry(expiry)
                , m_source(source)
        {}


        /**
         * @brief Get the protection type.
         */
        inline const QString & type() const { return m_type; }

        /**
         * @brief Get the protection level.
         */
        inline const QString & level() const { return m_level; }

        /**
         * @brief Get the protection expiry.
         */
        inline const QString & expiry() const { return m_expiry; }

        /**
         * @brief Get the protection source.
         */
        inline const QString & source() const { return m_source; }

    private:

        QString m_type;
        QString m_level;
        QString m_expiry;
        QString m_source;
    };

    /**
     * @brief A QueryInfo result.
     */
    struct Result {

    public:

        /**
         * Constructs an info.
         */
        Result()
        {}

        /**
         * Constructs an info.
         * @param protections the page protections
         */
        Result(const QList<QueryInfo::Protection> & protections)
                : m_protections(protections)
        {}

        /**
         * @brief Get the last touched timestamp.
         */
        inline QString touched() const { return m_touched.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"); }

        /**
         * @brief Get the timestamp when you obtained the token.
         */
        inline QString starttimestamp() const { return m_starttimestamp.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"); }

        /**
         * @brief Put a protections list.
         * @param protections the page protections
         */
        inline void setProtections(QList<QueryInfo::Protection> protection) { m_protections = protection; }

        /**
         * @brief Get the protections list.
         */
        inline const QList<QueryInfo::Protection> & protections() const { return m_protections; }

        unsigned int m_pageid;
        QString m_title;
        unsigned int m_ns;
        unsigned int m_lastrevid;
        unsigned int m_counter;
        unsigned int m_length;
        QString m_edittoken ;
        unsigned int m_talkid ;
        QUrl m_fullurl;
        QUrl m_editurl;
        QString m_readable;
        QString m_preload;
        QDateTime m_touched;
        QDateTime m_starttimestamp;

    private:

        QList<QueryInfo::Protection> m_protections;
    };

    /**
     * @brief Get the warning.
     */
    const QString & warning () const { return m_warning; }

    /**
     * @brief Put a warning.
     * @param warning the warning
     */
    void setWarning (const QString & warning) {  m_warning = warning; }

signals:

    /**
     * @brief Provides a pages list
     * @param info list of pages
     * @see QueryInfo::Result
     */
    void infos(const QList<QueryInfo::Result> & info);

private slots:

    /**
     * @brief Destroy the connection.
     */
    void abort();

    /**
     * @brief Send a request.
     */
    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    /**
     * @brief Contains the class attributes.
     */
    struct QueryInfoPrivate * const d;

    /**
     * @brief Contains the warning.
     */
    QString m_warning;

};

}
#endif // QUERYINFO_H
