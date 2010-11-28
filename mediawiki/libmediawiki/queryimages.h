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

#include <KDE/KJob>

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Query images job.
 *
 * Gets a list of all images used on pages.
 */
class QueryImages : public KJob {

    Q_OBJECT

public:

    /**
     * @brief A image
     */
    struct Image {

    public:

        /**
         * @brief Constructs a image.
         * @param namespaceId the namespace id of the image
         * @param title the title of the image
         */
        Image(unsigned int namespaceId, QString const & title)
            : m_namespaceId(namespaceId)
            , m_title(title)
        {}

        /**
         * @brief Returns the namespace id of the image.
         * @return the namespace id of the image
         */
        inline unsigned int namespaceId() const { return m_namespaceId; }

        /**
         * @brief Returns the title of the imagee.
         * @return the title of the image
         */
        inline QString title() const { return m_title; }

    private:

        unsigned int m_namespaceId;

        QString m_title;

    };

    /**
     * @brief A page.
     */
    struct Page {

    public:

        /**
         * @brief Construct a page.
         * @param pageId the page id
         * @param namespaceId the namespace id of the page
         * @param title the title of the page
         * @param titleNoNormalized the title of the page no normalized
         * @param images images of page
         * @param isMissing true if the page is missing, else false
         */
        Page(unsigned int pageId,
             unsigned int namespaceId,
             QString const & title,
             QString const & titleNoNormalized,
             QList<Image> const & images,
             bool isMissing
            )
            : m_pageId(pageId)
            , m_namespaceId(namespaceId)
            , m_title(title)
            , m_titleNoNormalized(titleNoNormalized)
            , m_images(images)
            , m_isMissing(isMissing)
        {}

        /**
         * @brief Returns the page id.
         * @return the page id
         * @pre !#isMissing()
         */
        inline unsigned int pageId() const { Q_ASSERT(!isMissing()); return m_pageId; }

        /**
         * @brief Returns the namespace id of the page.
         * @return the namespace id of the page
         */
        inline unsigned int namespaceId() const { return m_namespaceId; }

        /**
         * @brief Returns the title of the page.
         * @return the title of the page
         */
        inline QString title() const { return m_title; }

        /**
         * @brief Returns the title no normalized of the page.
         * @return the title of the page no normalized
         * @post #isNormalized() ? return != #title() : return == #title()
         */
        inline QString titleNoNormalized() const {
            Q_ASSERT(isNormalized() ? m_titleNoNormalized != title() : m_titleNoNormalized == title());
            return m_titleNoNormalized;
        }

        /**
         * @brief Returns images of page.
         * @return images of page
         */
        inline QList<Image> images() const { return m_images; }

        /**
         * @brief Returns true if the page is missing, else false.
         * @return true if the page is missing, else false
         */
        inline bool isMissing() const { return m_isMissing; }

        /**
         * @brief Returns true if the page is normalized, else false.
         * @return true if the page is normalized, else false
         */
        inline bool isNormalized() const { return m_title != m_titleNoNormalized; }

    private:

        unsigned int m_pageId;

        unsigned int m_namespaceId;

        QString m_title;

        QString m_titleNoNormalized;

        QList<Image> m_images;

        bool m_isMissing;

    };

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
     * @brief Constructs a query images job.
     * @param mediawiki the mediawiki concerned by the job
     * @param title the title of the page
     * @param limit number of images returned by one signal #pages()
     * @param parent the QObject parent
     */
    explicit QueryImages(MediaWiki const & mediawiki, QString const & title, unsigned int limit = 10, QObject * parent = 0);

    /**
     * @brief Destroys a query images job.
     */
    virtual ~QueryImages();

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

signals:

    /**
     * @brief Provides a list of all images used on pages
     *
     * This signal can be emited several times.
     *
     * @param pages list of all images used on pages
     */
    void pages(QList<QueryImages::Page> const & pages);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct QueryImagesPrivate * const d;

};

}
