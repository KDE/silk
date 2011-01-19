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

#include <KDE/KIO/Job>

#include "mediawiki.h"

#include "pictureofthedayengine.h"

PictureOfTheDayEngine::PictureOfTheDayEngine(QObject * parent, const QVariantList & args)
    : Plasma::DataEngine(parent, args)
    , m_pages()
    , m_images()
    , m_mediawiki()
{
    setMinimumPollingInterval(3600000);
    m_mediawiki["commons.wikimedia.org"] = MediaWikiInfo(QUrl("http://commons.wikimedia.org/w/api.php"), QString("Template:Potd/"));
    m_mediawiki["en.wikipedia.org"] = MediaWikiInfo(QUrl("http://en.wikipedia.org/w/api.php"), QString("Template:POTD/"));
}

void PictureOfTheDayEngine::init() {
    Plasma::DataEngine::Data data;
    data["Wikimedia Commons"] = QString("commons.wikimedia.org");
    data["Wikipedia"] = QString("en.wikipedia.org");
    setData(QString("mediawiki"), data);
}

bool PictureOfTheDayEngine::sourceRequestEvent(const QString& name) {
    return updateSourceEvent(name);
}

bool PictureOfTheDayEngine::updateSourceEvent(const QString& source) {
    if (sources().contains(source)) return true;

    QStringList sourceSplit = source.split(':');
    if (sourceSplit.size() != 2 || !m_mediawiki.contains(sourceSplit[0])) return false;

    MediaWiki mediawiki(m_mediawiki[sourceSplit[0]].url, QString("pictureofthedayengine"));
    if (!searchImages(mediawiki, m_mediawiki[sourceSplit[0]].page + sourceSplit[1])) return false;
    if (!searchImageinfo(mediawiki)) return false;

    Plasma::DataEngine::Data data;
    data["url"] = m_imageinfo.thumbUrl();
    KIO::StoredTransferJob * image = KIO::storedGet(KUrl(m_imageinfo.thumbUrl()), KIO::NoReload, KIO::HideProgressInfo);
    image->exec();
    data["image"] = QImage::fromData(image->data());
    setData(source, data);
    m_pages.clear();
    m_images.clear();
    return true;
}

void PictureOfTheDayEngine::pages(const QList<QueryImages::Page> & pages) {
    m_pages.append(pages);
}

void PictureOfTheDayEngine::images(const QList<QueryImageinfo::Image> & images) {
    m_images.append(images);
}

bool PictureOfTheDayEngine::searchImages(const MediaWiki & mediawiki, const QString& page) {
    QueryImages * const queryimages(new QueryImages(mediawiki));
    queryimages->setTitle(page);
    connect(queryimages, SIGNAL(pages(const QList<QueryImages::Page> &)), this, SLOT(pages( const QList<QueryImages::Page>&)));
    if (!queryimages->exec() || m_pages.size() == 0) {
        return false;
    }
    m_page = m_pages[0];
    return !m_page.isMissing() && m_page.images().size() > 0;
}

bool PictureOfTheDayEngine::searchImageinfo(const MediaWiki & mediawiki) {
    QueryImageinfo * const queryimageinfo(new QueryImageinfo(mediawiki, m_page.images()[0].title()));
    queryimageinfo->paramLimit(1u, true);
    queryimageinfo->paramProperties(QueryImageinfo::URL);
    queryimageinfo->paramScale(400u, 300u);
    connect(queryimageinfo, SIGNAL(images(const QList<QueryImageinfo::Image> &)), this, SLOT(images(const QList<QueryImageinfo::Image> &)));
    if (!queryimageinfo->exec() || m_images.size() == 0) {
        return false;
    }
    QueryImageinfo::Image image(m_images[0]);
    if (image.isMissing() || image.imageinfos().size() == 0) {
        return false;
    }
    m_imageinfo = image.imageinfos()[0];
    return true;
}

K_EXPORT_PLASMA_DATAENGINE(pictureoftheday, PictureOfTheDayEngine)

#include "pictureofthedayengine.moc"
