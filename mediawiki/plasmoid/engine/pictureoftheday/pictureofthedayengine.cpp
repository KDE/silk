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

#include "mediawiki.h"

#include "pictureofthedayengine.h"

PictureOfTheDayEngine::PictureOfTheDayEngine(QObject * parent, QVariantList const & args)
    : Plasma::DataEngine(parent, args)
    , m_pages()
    , m_images()
    , m_mediawiki()
{
    setMinimumPollingInterval(3600000);
    m_mediawiki["wikimediacommons"] = QUrl("http://commons.wikimedia.org/w/api.php");
}

void PictureOfTheDayEngine::init() {
    Plasma::DataEngine::Data data;
    data["Wikimedia Commons"] = QString("wikimediacommons");
    setData(QString("mediawiki"), data);
}

bool PictureOfTheDayEngine::sourceRequestEvent(QString const & name) {
    return updateSourceEvent(name);
}

bool PictureOfTheDayEngine::updateSourceEvent(QString const & source) {
    QStringList sourceSplit = source.split(':');
    if (sourceSplit.size() != 2 || !m_mediawiki.contains(sourceSplit[0])) {
        return false;
    }
    MediaWiki mediawiki(m_mediawiki[sourceSplit[0]], QString("pictureofthedayengine"));
    QueryImages * const queryimages(new QueryImages(mediawiki, QString("Template:Potd/") + sourceSplit[1]));
    connect(queryimages, SIGNAL(pages(QList<QueryImages::Page> const &)), this, SLOT(pages(QList<QueryImages::Page> const &)));
    if (!queryimages->exec()) {
        return false;
    }
    if (m_pages.size() == 0) {
        return false;
    }
    QueryImages::Page page(m_pages[0]);
    if (page.isMissing() || page.images().size() == 0) {
        return false;
    }
    QueryImageinfo * const queryimageinfo(new QueryImageinfo(mediawiki, page.images()[0].title()));
    queryimageinfo->paramLimit(1u, true);
    queryimageinfo->paramProperties(QueryImageinfo::URL);
    connect(queryimageinfo, SIGNAL(images(QList<QueryImageinfo::Image> const &)), this, SLOT(images(QList<QueryImageinfo::Image> const &)));
    if (!queryimageinfo->exec()) {
        return false;
    }
    if (m_images.size() == 0) {
        return false;
    }
    QueryImageinfo::Image image(m_images[0]);
    if (image.isMissing() || image.imageinfos().size() == 0) {
        return false;
    }
    QueryImageinfo::Imageinfo imageinfo(image.imageinfos()[0]);
    Plasma::DataEngine::Data data;
    data["url"] = imageinfo.url();
    setData(source, data);
    m_pages.clear();
    m_images.clear();
    return true;
}

void PictureOfTheDayEngine::pages(QList<QueryImages::Page> const & pages) {
    m_pages.append(pages);
}

void PictureOfTheDayEngine::images(QList<QueryImageinfo::Image> const & images) {
    m_images.append(images);
}

K_EXPORT_PLASMA_DATAENGINE(pictureoftheday, PictureOfTheDayEngine)

#include "pictureofthedayengine.moc"
