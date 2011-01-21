/*
 *   Copyright 2010 by Ludovic Delfau <ludovicdelfau@gmail.com>
 *   Copyright 2010 by Paolo de Vathaire <paolo.devathaire@gmail.com>
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

#ifndef MEDIAWIKI_QUERYIMAGES_H
#define MEDIAWIKI_QUERYIMAGES_H

#include <KDE/KJob>

#include "mediawiki_export.h"
#include "image.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;

/**
 * @brief Query images job.
 *
 * Gets a list of all images used on pages.
 */
class MEDIAWIKI_EXPORT QueryImages : public KJob {

    Q_OBJECT

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
     * @brief Constructs a query images job.
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    explicit QueryImages(const MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destroys a query images job.
     */
    virtual ~QueryImages();

    /**
     * @brief Set the title.
     * @param title the title of the page
     */
    void setTitle(const QString & title);

    /**
     * @brief Set the limit.
     * @param limit number of images returned by one signal #pages()
     */
    void setLimit(unsigned int limit);

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
    void images(const QList<Image> & images);

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

private:

    struct QueryImagesPrivate * const d;

};

}

#endif
