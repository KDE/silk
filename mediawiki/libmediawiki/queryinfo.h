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

class QNetworkReply;

namespace mediawiki
{

class MediaWiki;

/**
 * @brief QueryInfo job.
 *
 * Uses to send a request to get basic page information.
 */
class QueryInfo : public KJob
{

    Q_OBJECT

public:

    enum IdType
    {
        revids,
        pageids,
        noid
    };

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
         * @brief Warning
         */
        Warnings,
    };

    /**
     * @brief Constructs a QueryInfo job.
     * @param mediawiki the mediawiki concerned by the job
     * @param name the page title
     * @param token the page token
     * @param parent the QObject parent
     */
    explicit QueryInfo(MediaWiki const & mediawiki, QString const & name, QString const & token, QObject *parent = 0);

    /**
     * @brief Constructs a QueryInfo job.
     * @param mediawiki the mediawiki concerned by the job
     * @param id the page id
     * @param token the page token
     * @param parent the QObject parent
     */
    explicit QueryInfo(MediaWiki const & mediawiki, unsigned int id, IdType type, QString const & token, QObject *parent = 0);

    /**
     * @brief Destroys the QuerySiteInfoGeneral job.
     */
    virtual ~QueryInfo();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

    struct Protection {

    public:

        /**
         * Constructs a protection.
         * @param type the name of the user group
         * @param level rights of the user group
         * @param expiry rights of the user group
         */
        Protection(QString const & type, QString const & level, QString const & expiry, QString const & source)
                : m_type(type)
                , m_level(level)
                , m_expiry(expiry)
                , m_source(source)
        {}

        inline QString const & type() const { return m_type; }
        inline QString const & level() const { return m_level; }
        inline QString const & expiry() const { return m_expiry; }
        inline QString const & source() const { return m_source; }

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
         * Constructs a user group.
         */
        Result()
        {}

        /**
         * Constructs a user group.
         * @param name the name of the user group
         */
        Result(QList<QueryInfo::Protection> const & protections)
                : m_protections(protections)
        {}

        /**
         * @brief Returns rights of the user group.
         * @return rights of the user group
         */
        inline QList<QueryInfo::Protection> const & protections() const { return m_protections; }
        inline QString touched() const { return m_touched.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"); }
        inline QString starttimestamp() const { return m_starttimestamp.toString("yyyy'-'MM'-'dd'T'hh':'mm':'ss'Z'"); }

        inline void setProtections(QList<QueryInfo::Protection> protection) { m_protections = protection; }

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

signals:

    /**
     * @brief
     * @param info list of
     * @see QueryInfo::Result
     */
    void infos(QList<QueryInfo::Result> const & info);

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

};

}
#endif // QUERYINFO_H
