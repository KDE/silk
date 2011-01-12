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
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#include <KDE/KJob>

#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Query image info job.
 *
 * Gets image information for an image.
 */
class MEDIAWIKI_EXPORT QueryImageinfo : public KJob {

    Q_OBJECT

public:

    /**
     * @brief Constructs a query image info job.
     *
     * You can set parameters of the query after.
     *
     * By default, a single image info is returned.
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
     * @see SHA1
     * @see MIME
     * @see METADATA
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
     * @brief SHA-1 of the image.
     * @see paramProperties()
     */
    static property_type const SHA1 = 1 << 5;

    /**
     * @brief MIME of the image.
     * @see paramProperties()
     */
    static property_type const MIME = 1 << 6;

    /**
     * @brief Metadata of the image.
     * @see paramProperties()
     */
    static property_type const METADATA = 1 << 7;

    /**
     * @brief All properties.
     * @see paramProperties()
     */
    static property_type const ALL_PROPERTIES = TIMESTAMP|USER|COMMENT|URL|SIZE|SHA1|MIME|METADATA;

    /**
     * @brief Set how many image revisions to return.
     * @param limit how many image revisions to return
     * @param stop stop the job when the limit is reached
     * @pre to call just before start()
     * @pre limit >= 1
     */
    void paramLimit(unsigned int limit, bool stop);

    /**
     * @brief Set timestamp to start listing from.
     * @param start timestamp to start listing from
     * @pre to call just before start()
     */
    void paramStart(QDateTime const & begin);

    /**
     * @brief Set timestamp to stop listing at.
     * @param end timestamp to stop listing at
     * @pre to call just before start()
     */
    void paramEnd(QDateTime const & end);

    /**
     * @brief Set image scale parameters for an URL will be returned.
     *
     * Only for the first image info and the property URL will be set.
     *
     * @param width width parameter
     * @pre to call just before start()
     * @pre width >= 1
     */
    void paramScale(unsigned int width);

    /**
     * @brief Set image scale parameters for an URL will be returned.
     *
     * Only for the first image info and the property URL will be set.
     *
     * @param width width parameter
     * @param height height paramter
     * @pre to call just before start()
     * @pre width >= 1
     * @pre height >= 1
     */
    void paramScale(unsigned int width, unsigned int height);

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
         * @param thumbUrl the thumb URL of the image
         * @param thumbWidth the thumb width of the image
         * @param thumbHeight the thumb height of the image
         * @param size the image's size in bytes
         * @param width the image's width
         * @param height the image's height
         * @param sha1 the image's SHA-1 hash
         * @param mime the image's MIME type
         * @param metadata image metadata
         * @param properties properties has set
         */
        Imageinfo(QDateTime const & timestamp,
                  QString const & user,
                  QString const & comment,
                  QUrl const & url,
                  QUrl const & descriptionUrl,
                  QUrl const & thumbUrl,
                  unsigned int thumbWidth,
                  unsigned int thumbHeight,
                  unsigned int size,
                  unsigned int width,
                  unsigned int height,
                  QString const & sha1,
                  QString const & mime,
                  QHash<QString, QVariant> const & metadata,
                  QueryImageinfo::property_type properties)
            : m_timestamp(timestamp)
            , m_user(user)
            , m_comment(comment)
            , m_url(url)
            , m_descriptionUrl(descriptionUrl)
            , m_thumbUrl(thumbUrl)
            , m_thumbWidth(thumbWidth)
            , m_thumbHeight(thumbHeight)
            , m_size(size)
            , m_width(width)
            , m_height(height)
            , m_sha1(sha1)
            , m_mime(mime)
            , m_metadata(metadata)
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
         * @brief Returns the thumb URL of the image.
         * @return the thumb URL of the image
         * @pre #hasThumbUrl()
         */
        inline QUrl thumbUrl() const { Q_ASSERT(hasThumbUrl()); return m_thumbUrl; }

        /**
         * @brief Returns the thumb width of the image.
         * @return the thumb width of the image
         * @pre #hasThumbUrl()
         */
        inline unsigned int thumbWidth() const { Q_ASSERT(hasThumbUrl()); return m_thumbWidth; }

        /**
         * @brief Returns the thumb height of the image.
         * @return the thumb height of the image
         * @pre #hasThumbUrl()
         */
        inline unsigned int thumbHeight() const { Q_ASSERT(hasThumbUrl()); return m_thumbHeight; }

        /**
         * @brief Returns true if thumb URL has set, else false.
         * @return true if thumb URL has set, else false
         */
        inline bool hasThumbUrl() const { return !m_thumbUrl.isEmpty(); }

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

        /**
         * @brief Returns the image's SHA-1 hash.
         * @return the image's SHA-1 hash
         * @pre #hasSha1()
         */
        inline QString sha1() const { Q_ASSERT(hasSha1()); return m_sha1; }

        /**
         * @brief Returns true if SHA-1 has set, else false.
         * @return true if SHA-1 has set, else false
         */
        inline bool hasSha1() const { return m_properties & QueryImageinfo::SHA1; }

        /**
         * @brief Returns the image's MIME type.
         * @return the image's MIME type
         * @pre #hasMime()
         */
        inline QString mime() const { Q_ASSERT(hasMime()); return m_mime; }

        /**
         * @brief Returns true if MIME has set, else false.
         * @return true if MIME has set, else false
         */
        inline bool hasMime() const { return m_properties & QueryImageinfo::MIME; }

        /**
         * @brief Returns image metadata.
         * @return image metadata
         * @pre #hasMetadata()
         */
        inline QHash<QString, QVariant> const & metadata() const { Q_ASSERT(hasMetadata()); return m_metadata; }

        /**
         * @brief Returns true if metadata has set, else false.
         * @return true if metadata has set, else false
         */
        inline bool hasMetadata() const { return m_properties & QueryImageinfo::METADATA; }

    private:

        QDateTime m_timestamp;
        QString m_user;
        QString m_comment;
        QUrl m_url;
        QUrl m_descriptionUrl;
        QUrl m_thumbUrl;
        unsigned int m_thumbWidth;
        unsigned int m_thumbHeight;
        unsigned int m_size;
        unsigned int m_width;
        unsigned int m_height;
        QString m_sha1;
        QString m_mime;
        QHash<QString, QVariant> m_metadata;
        QueryImageinfo::property_type m_properties;

    };

    /**
     * @brief An image.
     */
    struct Image {

    public:

        /**
         * @brief Construct an image.
         * @param namespaceId the image's namespace id
         * @param title the image's title
         * @param titleNoNormalized he image's title no normalized
         * @param imageRepository the image's repository
         * @param imageinfos the image's info
         */
        Image(unsigned int namespaceId,
              QString const & title,
              QString const & titleNoNormalized,
              QString const & imageRepository,
              QVector<Imageinfo> const & imageinfos)
              : m_namespaceId(namespaceId)
              , m_title(title)
              , m_titleNoNormalized(titleNoNormalized)
              , m_imageRepository(imageRepository)
              , m_imageinfos(imageinfos)
        {}

        /**
         * @brief Returns the image's namespace id.
         * @return the image's namespace id
         */
        inline unsigned int namespaceId() const { return m_namespaceId; }

        /**
         * @brief Returns the image's title.
         * @return the image's title
         */
        inline QString title() const { return m_title; }

        /**
         * @brief Returns the image's title no normalized.
         * @return the image's title no normalized
         * @post #isNormalized() ? return != #title() : return == #title()
         */
        inline QString titleNoNormalized() const {
            Q_ASSERT(isNormalized() ? m_titleNoNormalized != title() : m_titleNoNormalized == title());
            return m_titleNoNormalized;
        }

        /**
         * @brief Returns true if is normalized, else false.
         * @return true if is normalized, else false
         */
        inline bool isNormalized() const { return m_title != m_titleNoNormalized; }

        /**
         * @brief Returns the image's repository.
         * @return the image's repository
         * @pre !isMissing()
         */
        inline QString imageRepository() const { Q_ASSERT(!isMissing()); return m_imageRepository; }

        /**
         * @brief Returns the image's info.
         * @return the image's info
         * @pre !isMissing()
         */
        inline QVector<Imageinfo> const & imageinfos() const { Q_ASSERT(!isMissing()); return m_imageinfos; }

        /**
         * @brief Returns true if is missing, else false.
         * @return true if is missing, else false
         */
        inline bool isMissing() const { return m_imageRepository.isEmpty(); }

    private:

        unsigned int m_namespaceId;
        QString m_title;
        QString m_titleNoNormalized;
        QString m_imageRepository;
        QVector<Imageinfo> m_imageinfos;

    };

signals:

    /**
     * @brief Provides a list of image.
     *
     * This signal can be emited several times.
     *
     * @param imageinfos a list of image
     */
    void images(QList<QueryImageinfo::Image> const & images);

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
           lhs.hasThumbUrl() == rhs.hasThumbUrl() &&
           (lhs.hasThumbUrl() /* && rhs.hasThumbUrl() */ ? lhs.thumbUrl() == rhs.thumbUrl() && lhs.thumbWidth() == rhs.thumbWidth() && lhs.thumbHeight() == rhs.thumbHeight(): true) &&
           lhs.hasSize() == rhs.hasSize() &&
           (lhs.hasSize() /* && rhs.hasSize() */ ? lhs.size() == rhs.size() && lhs.width() == rhs.width() && lhs.height() == rhs.height() : true) &&
           lhs.hasSha1() == rhs.hasSha1() &&
           (lhs.hasSha1() /* && rhs.hasSha1() */ ? lhs.sha1() == rhs.sha1() : true) &&
           lhs.hasMime() == rhs.hasMime() &&
           (lhs.hasMime() /* && rhs.hasMime() */ ? lhs.mime() == rhs.mime() : true) &&
           lhs.hasMetadata() == rhs.hasMetadata() &&
           (lhs.hasMetadata() /* && rhs.hasMetadata() */ ? lhs.metadata() == rhs.metadata() : true);
}

/**
 * @brief Returns true if lhs and rhs are equal, else false.
 * @param lhs left-hand side image info
 * @param rhs right-hand side image info
 * @return true if lhs and rhs are equal, else false
 */
inline bool operator==(QueryImageinfo::Image const & lhs, QueryImageinfo::Image const & rhs) {
    return lhs.namespaceId() == rhs.namespaceId() &&
           lhs.title() == rhs.title() &&
           lhs.titleNoNormalized() == rhs.titleNoNormalized() &&
           lhs.isNormalized() == rhs.isNormalized() &&
           lhs.isMissing() == rhs.isMissing() &&
           (!lhs.isMissing() /* && rhs.isMissing() */ ? lhs.imageRepository() == rhs.imageRepository() && lhs.imageinfos() == rhs.imageinfos() : true);
}

}

#endif
