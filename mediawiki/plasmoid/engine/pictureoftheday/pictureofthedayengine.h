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
#include "queryrevision.h"

using namespace mediawiki;

//PATTERN: mediawiki:yyyy-MM-jj
class PictureOfTheDayEngine : public Plasma::DataEngine {

    Q_OBJECT

public:

    PictureOfTheDayEngine(QObject * parent, const QVariantList & args);

protected:

    virtual void init();

    virtual bool sourceRequestEvent(const QString & name);

    virtual bool updateSourceEvent(const QString & source);

private slots:

    void result(const QList<Image> & images);

    void result(const QList<Imageinfo> & imageinfos);

    void result(const QList<Revision> & revisions);

private:

    struct MediaWikiInfo {
        MediaWikiInfo() {}
        MediaWikiInfo(const QUrl & url, const QString & page, const QString & lang, const QString & regex)
            : url(url), page(page), lang(lang), regex(regex) {}
        QUrl url;
        QString page;
        QString lang;
        QString regex;
    };

    bool searchImages(const MediaWiki & mediawiki, const QString & page);

    bool searchImageinfo(MediaWiki & mediawiki, Image & image);

    bool searchText(MediaWiki & mediawiki, const QString & page);

    QList<Image> m_images;
    QList<Imageinfo> m_imageinfos;
    QList<Revision> m_revisions;
    QMap<QString, MediaWikiInfo> m_mediawiki;

};

#endif
