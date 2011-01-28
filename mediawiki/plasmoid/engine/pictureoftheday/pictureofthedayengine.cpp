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
{
    setMinimumPollingInterval(3600000);
    m_mediawiki["commons.wikimedia.org"] = MediaWikiInfo(QUrl("http://commons.wikimedia.org/w/api.php"), QString("Template:Potd/"), QString("_(en)"));
    m_mediawiki["en.wikipedia.org"] = MediaWikiInfo(QUrl("http://en.wikipedia.org/w/api.php"), QString("Template:POTD/"), QString(""));
}

void PictureOfTheDayEngine::init() {
    Plasma::DataEngine::Data data;
    data["Wikimedia Commons"] = QString("commons.wikimedia.org");
    data["Wikipedia"] = QString("en.wikipedia.org");
    setData(QString("mediawiki"), data);
}

bool PictureOfTheDayEngine::sourceRequestEvent(const QString & name) {
    return updateSourceEvent(name);
}

bool PictureOfTheDayEngine::updateSourceEvent(const QString & source) {
    m_images.clear();
    m_imageinfos.clear();
    m_revisions.clear();

    if (sources().contains(source)) return true;

    QStringList sourceSplit = source.split(':');
    if (sourceSplit.size() != 2 || !m_mediawiki.contains(sourceSplit[0])) return false;

    MediaWiki mediawiki(m_mediawiki[sourceSplit[0]].url, QString("pictureofthedayengine"));
    const QString page(m_mediawiki[sourceSplit[0]].page + sourceSplit[1] + m_mediawiki[sourceSplit[0]].lang);
    if (!searchImages(mediawiki, page)) return false;
    if (!searchImageinfo(mediawiki)) return false;
    if (!searchText(mediawiki, page)) return false;

    Plasma::DataEngine::Data data;
    data["url"] = m_imageinfo.thumbUrl();
    KIO::StoredTransferJob * imageJob = KIO::storedGet(KUrl(m_imageinfo.thumbUrl()), KIO::NoReload, KIO::HideProgressInfo);
    imageJob->exec();
    data["image"] = QImage::fromData(imageJob->data());
    //FIXME: pointeur de fonction ou classe
    QString content;
    if (sourceSplit[0] == QString("en.wikipedia.org")) {
        content = m_revision.content().remove(QRegExp("^.*\\[\\[Image:[^\\]]*\\]\\]\\s*\\|\\s*|\\s*<small>.*$"));
    }
    else if (sourceSplit[0] == QString("commons.wikimedia.org")) {
        content = m_revision.content().remove(QRegExp("^.*<span[^>]*>\\s*|\\s*</span>.*$"));
    }
    data["content"] = content.replace(QRegExp("\\[\\[((\\]?[^\\]\\|])*\\]?\\|)?|\\]\\]"), "").replace(QRegExp("'''((('('?))?[^'])*)'''"), "<b>\\1</b>").replace(QRegExp("''(('?[^'])*)''"), "<i>\\1</i>");
    setData(source, data);
    return true;
}

void PictureOfTheDayEngine::result(const QList<Image> & images) {
    m_images.append(images);
}

void PictureOfTheDayEngine::result(const QList<Imageinfo> & imageinfos) {
    m_imageinfos.append(imageinfos);
}

void PictureOfTheDayEngine::result(const QList<Revision> & revisions)
{
    m_revisions.append(revisions);
}

bool PictureOfTheDayEngine::searchImages(const MediaWiki & mediawiki, const QString & page) {
    QueryImages * const queryimages(new QueryImages(mediawiki));
    queryimages->setTitle(page);
    connect(queryimages, SIGNAL(images(const QList<Image> &)), this, SLOT(result( const QList<Image> &)));
    if (!queryimages->exec() || m_images.size() == 0) {
        return false;
    }
    m_image = m_images[0]; //FIXME: faire ça dans l'update et dc un return avec la condition
    return true;
}

bool PictureOfTheDayEngine::searchImageinfo(MediaWiki & mediawiki) {
    QueryImageinfo * const queryimageinfo(new QueryImageinfo(mediawiki));
    queryimageinfo->setTitle(m_image.title());
    queryimageinfo->setLimit(1u);
    queryimageinfo->setOnlyOneSignal(true);
    queryimageinfo->setProperties(QueryImageinfo::Url);
    queryimageinfo->setWidthScale(280u);
    queryimageinfo->setHeightScale(210u);
    connect(queryimageinfo, SIGNAL(result(const QList<Imageinfo> &)), this, SLOT(result(const QList<Imageinfo> &)));
    if (!queryimageinfo->exec() || m_imageinfos.size() == 0) {
        return false;
    }
    m_imageinfo = m_imageinfos[0];
    return true;
}

bool PictureOfTheDayEngine::searchText(MediaWiki & mediawiki, const QString & page)
{
    QueryRevision * const queryrevision(new QueryRevision(mediawiki));
    queryrevision->setPageName(page);
    queryrevision->setProp(CONTENT);
    queryrevision->setExpandTemplates(true);
    queryrevision->setLimit(1);
    connect(queryrevision, SIGNAL(revision(const QList<Revision> &)), this, SLOT(result(const QList<Revision> &)));
    if (!queryrevision->exec() || m_revisions.size() == 0) {
        return false;
    }
    m_revision = m_revisions[0];
    return true;
}


K_EXPORT_PLASMA_DATAENGINE(pictureoftheday, PictureOfTheDayEngine)

#include "pictureofthedayengine.moc"
