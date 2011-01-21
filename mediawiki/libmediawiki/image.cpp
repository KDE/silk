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

#include "image.h"

namespace mediawiki {

class ImagePrivate {

public:

    qint64 namespaceId;
    QString title;
    QVector<Imageinfo> imageinfos;

};

}

using namespace mediawiki;

Image::Image()
    : d(new ImagePrivate())
{
    d->namespaceId = -1;
}

Image::Image(const Image & other)
    : d(new ImagePrivate(*(other.d)))
{}

Image::~Image()
{
    delete d;
}

Image & Image::operator=(Image other)
{
    std::swap(d, other.d);
    return *this;
}

qint64 Image::namespaceId() const {
    return d->namespaceId;
}

void Image::setNamespaceId(qint64 namespaceId) {
    d->namespaceId = namespaceId;
}

QString Image::title() const {
    return d->title;
}

void Image::setTitle(const QString & title) {
    d->title = title;
}

const QVector<Imageinfo> & Image::imageinfos() const {
    return d->imageinfos;
}

QVector<Imageinfo> & Image::imageinfos() {
    return d->imageinfos;
}

void Image::setImageinfos(const QVector<Imageinfo> & imageinfos) {
    d->imageinfos = imageinfos;
}

bool operator==(const Image & lhs, const Image & rhs) {
    return lhs.namespaceId() == rhs.namespaceId() &&
           lhs.title() == rhs.title();
}

