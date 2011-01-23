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

#ifndef MEDIAWIKI_IMAGE_H
#define MEDIAWIKI_IMAGE_H

#include <QtCore/QString>

#include "mediawiki_export.h"

namespace mediawiki {

/**
 * @brief A image.
 */
class MEDIAWIKI_EXPORT Image {

public:

    /**
     * @brief Constructs a image.
     */
    Image();

    /**
     * @brief Constructs an image an other image.
     * @param other an other image
     */
    Image(const Image & other);

    /**
     * @brief Destructs an image.
     */
    ~Image();

    /**
     * @brief Assingning an image from an other image.
     * @param other an other image
     */
    Image & operator=(Image other);

    /**
     * @brief Returns the namespace id of the image.
     * @return the namespace id of the image
     */
    qint64 namespaceId() const;

    /**
     * @brief Set the namespace id.
     * @param namespaceId the namespace id of the image
     */
    void setNamespaceId(qint64 namespaceId);

    /**
     * @brief Returns the title of the image.
     * @return the title of the image
     */
    QString title() const;

    /**
     * @brief Set the title.
     * @param title the title of the image
     */
    void setTitle(const QString & title);

private:

    class ImagePrivate * d;

};

}

/**
 * @brief Returns true if lhs and rhs are equal, else false.
 * @param lhs left-hand side image info
 * @param rhs right-hand side image info
 * @return true if lhs and rhs are equal, else false
 */
MEDIAWIKI_EXPORT bool operator==(const mediawiki::Image & lhs, const mediawiki::Image & rhs);

#endif
