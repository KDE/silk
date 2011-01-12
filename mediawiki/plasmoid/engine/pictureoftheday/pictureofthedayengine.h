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

#ifndef PICTUREOFTHEDAYENGINE_H
#define PICTUREOFTHEDAYENGINE_H

#include <Plasma/DataEngine>

#include "queryimageinfo.h"
#include "queryimages.h"

using namespace mediawiki;

//PATTERN: mediawiki:yyyy-MM-jj
class PictureOfTheDayEngine : public Plasma::DataEngine {

    Q_OBJECT

public:

    PictureOfTheDayEngine(QObject* parent, const QVariantList & args);

protected:

    virtual void init();

    virtual bool sourceRequestEvent(const QString& name);

    virtual bool updateSourceEvent(const QString& source);

private slots:

    void pages(const QList<QueryImages::Page> & pages);

    void images(const QList<QueryImageinfo::Image> & images);

private:

    struct MediaWikiInfo {
        MediaWikiInfo() {}
        MediaWikiInfo(const QUrl & url, const QString & page) : url(url), page(page) {}
        QUrl url;
        QString page;
    };

    bool searchImages(const MediaWiki & mediawiki, const QString & date);

    bool searchImageinfo(const MediaWiki & mediawiki);

    QList<QueryImages::Page> m_pages;

    QueryImages::Page m_page;

    QList<QueryImageinfo::Image> m_images;

    QueryImageinfo::Imageinfo m_imageinfo;

    QMap<QString, MediaWikiInfo> m_mediawiki;

};

#endif
