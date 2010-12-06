/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
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

#ifndef MEDIAWIKI_QUERYIMAGEINFO_H
#define MEDIAWIKI_QUERYIMAGEINFO_H

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QUrl>

#include <KDE/KJob>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Query image info job.
 *
 * Gets image information for an image.
 */
class QueryImageinfo : public KJob {

    Q_OBJECT

public:

    /**
     * @brief Constructs a query image info job.
     *
     * You can set parameters of the query after.
     *
     * @param mediawiki the mediawiki concerned by the job
     * @param title the title of the image
     * @param parent the QObject parent
     */
    QueryImageinfo(MediaWiki const & mediawiki, QString const & title, QObject * parent = 0);

    /**
     * @brief Destructs a query image info job.
     */
    virtual ~QueryImageinfo();

    /**
     * @brief Type used for property.
     */
    typedef unsigned char property_type;

    /**
     * @brief Set which properties to get.
     * @param properties bitmask of properties to get
     * @pre to call just before start()
     * @see NO_PROPERTY
     * @see TIMESTAMP
     * @see USER
     * @see COMMENT
     * @see URL
     * @see SIZE
     * @see ALL_PROPERTIES
     */
    void paramProperties(property_type properties);

    /**
     * @brief No property.
     * @see paramProperties()
     */
    static property_type const NO_PROPERTY = 0;

    /**
     * @brief Timestamp of the image.
     * @see paramProperties()
     */
    static property_type const TIMESTAMP = 1 << 0;

    /**
     * @brief User of the image.
     * @see paramProperties()
     */
    static property_type const USER = 1 << 1;

    /**
     * @brief Comment of the image.
     * @see paramProperties()
     */
    static property_type const COMMENT = 1 << 2;

    /**
     * @brief URL of the image.
     * @see paramProperties()
     */
    static property_type const URL = 1 << 3;

    /**
     * @brief Size of the image.
     * @see paramProperties()
     */
    static property_type const SIZE = 1 << 4;

    /**
     * @brief All properties.
     * @see paramProperties()
     */
    static property_type const ALL_PROPERTIES = TIMESTAMP|USER|COMMENT|URL|SIZE;

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    QString iiprop() const;

    struct QueryImageinfoPrivate * const d;

public:

    /**
     * @brief Indicates all possible error conditions found during the processing of the job.
     */
    enum {

        /**
         * @brief A network error has occured.
         */
        NetworkError = KJob::UserDefinedError + 1,

        /**
         * @brief A XML error has occured.
         */
        XmlError,

    };

    /**
     * @brief An image info.
     */
    struct Imageinfo {

    public:

        /**
         * @brief Constructs an image info not initialized.
         */
        Imageinfo() {}

        /**
         * @brief Constructs an image info.
         * @param timestamp the time and date of the revision
         * @param user the user who made the revision
         * @param comment the edit comment
         * @param url the URL of the image
         * @param descriptionUrl the description URL of the image
         * @param size the image's size in bytes
         * @param width the image's width
         * @param height the image's height
         * @param properties properties has set
         */
        Imageinfo(QDateTime const & timestamp,
                  QString const & user,
                  QString const & comment,
                  QUrl const & url,
                  QUrl const & descriptionUrl,
                  unsigned int size,
                  unsigned int width,
                  unsigned int height,
                  QueryImageinfo::property_type properties)
            : m_timestamp(timestamp)
            , m_user(user)
            , m_comment(comment)
            , m_url(url)
            , m_descriptionUrl(descriptionUrl)
            , m_size(size)
            , m_width(width)
            , m_height(height)
            , m_properties(properties)
        {}

        /**
         * @brief Returns the time and date of the revision.
         * @return the time and date of the revision
         * @pre #hasTimestamp()
         */
        inline QDateTime timestamp() const { Q_ASSERT(hasTimestamp()); return m_timestamp; }

        /**
         * @brief Returns true if timestamp has set, else false.
         * @return true if timestamp has set, else false
         */
        inline bool hasTimestamp() const { return m_properties & QueryImageinfo::TIMESTAMP; }

        /**
         * @brief Returns the user who made the revision.
         * @return the user who made the revision
         * @pre #hasUser()
         */
        inline QString user() const { Q_ASSERT(hasUser()); return m_user; }

        /**
         * @brief Returns true if user has set, else false.
         * @return true if user has set, else false
         */
        inline bool hasUser() const { return m_properties & QueryImageinfo::USER; }

        /**
         * @brief Returns the edit comment.
         * @return the edit comment
         * @pre #hasComment()
         */
        inline QString comment() const { Q_ASSERT(hasComment()); return m_comment; }

        /**
         * @brief Returns true if comment has set, else false.
         * @return true if comment has set, else false
         */
        inline bool hasComment() const { return m_properties & QueryImageinfo::COMMENT; }

        /**
         * @brief Returns the URL of the image.
         * @return the URL of the image
         * @pre #hasUrl()
         */
        inline QUrl url() const { Q_ASSERT(hasUrl()); return m_url; }

        /**
         * @brief Returns the description URL of the image.
         * @return the description URL of the image
         * @pre #hasUrl()
         */
        inline QUrl descriptionUrl() const { Q_ASSERT(hasUrl()); return m_descriptionUrl; }

        /**
         * @brief Returns true if URL has set, else false.
         * @return true if URL has set, else false
         */
        inline bool hasUrl() const { return m_properties & QueryImageinfo::URL; }

        /**
         * @brief Returns the image's size in bytes.
         * @return the image's size in bytes
         * @pre #hasSize()
         */
        inline unsigned int size() const { Q_ASSERT(hasSize()); return m_size; }

        /**
         * @brief Returns the image's width.
         * @return the image's width
         * @pre #hasSize()
         */
        inline unsigned int width() const { Q_ASSERT(hasSize()); return m_width; }

        /**
         * @brief Returns the image's height.
         * @return the image's height
         * @pre #hasSize()
         */
        inline unsigned int height() const { Q_ASSERT(hasSize()); return m_height; }

        /**
         * @brief Returns true if size has set, else false.
         * @return true if size has set, else false
         */
        inline bool hasSize() const { return m_properties & QueryImageinfo::SIZE; }

    private:

        QDateTime m_timestamp;
        QString m_user;
        QString m_comment;
        QUrl m_url;
        QUrl m_descriptionUrl;
        unsigned int m_size;
        unsigned int m_width;
        unsigned int m_height;
        QueryImageinfo::property_type m_properties;

    };

signals:

    /**
     * @brief Provides a list of all imageinfos.
     *
     * This signal can be emited several times.
     *
     * @param imageinfos list of all imageinfos
     */
    void imageinfos(QList<QueryImageinfo::Imageinfo> const & imageinfos);

};

/**
 * @brief Returns true if lhs and rhs are equal, else false.
 * @param lhs left-hand side image info
 * @param rhs right-hand side image info
 * @return true if lhs and rhs are equal, else false
 */
inline bool operator==(QueryImageinfo::Imageinfo const & lhs, QueryImageinfo::Imageinfo const & rhs) {
    return lhs.hasTimestamp() == rhs.hasTimestamp() &&
           (lhs.hasTimestamp() /* && rhs.hasTimestamp() */ ? lhs.timestamp() == rhs.timestamp() : true) &&
           lhs.hasUser() == rhs.hasUser() &&
           (lhs.hasUser() /* && rhs.hasUser() */ ? lhs.user() == rhs.user() : true) &&
           lhs.hasComment() == rhs.hasComment() &&
           (lhs.hasComment() /* && rhs.hasComment() */ ? lhs.comment() == rhs.comment() : true) &&
           lhs.hasUrl() == rhs.hasUrl() &&
           (lhs.hasUrl() /* && rhs.hasUrl() */ ? lhs.url() == rhs.url() && lhs.descriptionUrl() == rhs.descriptionUrl() : true) &&
           lhs.hasSize() == rhs.hasSize() &&
           (lhs.hasSize() /* && rhs.hasSize() */ ? lhs.size() == rhs.size() && lhs.width() == rhs.width() && lhs.height() == rhs.height() : true);
}

}

#endif
