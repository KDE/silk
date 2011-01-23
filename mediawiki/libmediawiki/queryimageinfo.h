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

#include <QtCore/QList>

#include "imageinfo.h"
#include "job.h"
#include "mediawiki_export.h"

class QNetworkReply;

namespace mediawiki {

class MediaWiki;
class QueryImageinfoPrivate;

/**
 * @brief Query image info job.
 *
 * Gets image information for an image.
 */
class MEDIAWIKI_EXPORT QueryImageinfo : public Job {

    Q_OBJECT
    Q_DECLARE_PRIVATE(QueryImageinfo)

public:

    /**
     * @brief Constructs a query image info job.
     *
     * You can set parameters of the query after.
     *
     * By default, a single image info is returned.
     *
     * @param mediawiki the mediawiki concerned by the job
     * @param parent the QObject parent
     */
    QueryImageinfo(MediaWiki & mediawiki, QObject * parent = 0);

    /**
     * @brief Destructs a query image info job.
     */
    virtual ~QueryImageinfo();

    /**
     * @brief Set the title of the image requested.
     * @param title the title of the image requested
     */
    void setTitle(const QString & title);

    /**
     * @brief Type used for property.
     */
    typedef unsigned char property_type;

    /**
     * @brief Property.
     */
    enum Property {
        Timestamp    = 0x01,
        User         = 0x02,
        Comment      = 0x04,
        Url          = 0x08,
        Size         = 0x10,
        Sha1         = 0x20,
        Mime         = 0x40,
        Metadata     = 0x80
    };
    Q_DECLARE_FLAGS(Properties, Property)

    /**
     * @brief Set which properties to get.
     * @param properties properties to get
     */
    void setProperties(Properties properties);

    /**
     * @brief Set how many image info to return per signal.
     * @param limit how many image info to return per signal
     */
    void setLimit(unsigned int limit);

    /**
     * @brief If true stop the request to the first signal.
     * @param onlyOneSignal if true stop the request to the first signal
     */
    void setOnlyOneSignal(bool onlyOneSignal);

    /**
     * @brief Set timestamp to start listing from.
     * @param start timestamp to start listing from
     */
    void setBeginTimestamp(const QDateTime & begin);

    /**
     * @brief Set timestamp to stop listing at.
     * @param end timestamp to stop listing at
     */
    void setEndTimestamp(const QDateTime & end);

    /**
     * @brief Set width scale.
     *
     * Only for the first image info. The property URL must be set.
     *
     * @param width width scale
     */
    void setWidthScale(unsigned int width);

    /**
     * @brief Set height scale.
     *
     * Only for the first image info. The property URL must be set.
     *
     * @param height height scale
     */
    void setHeightScale(unsigned int height);

    /**
     * @brief Starts the job asynchronously.
     */
    virtual void start();

private slots:

    void doWorkSendRequest();

    void doWorkProcessReply(QNetworkReply * reply);

signals:

    /**
     * @brief Provides a list of imageinfos.
     * @param imageinfos a list of imageinfos
     */
    void result(const QList<Imageinfo> & imageinfos);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QueryImageinfo::Properties)

}

#endif
