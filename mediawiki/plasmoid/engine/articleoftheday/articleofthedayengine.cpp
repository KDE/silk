/*
 *   Copyright 2010 by Rémi Benoit <r3m1.benoit@gmail.com>
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

#include "articleofthedayengine.h"

ArticleOfTheDayEngine::ArticleOfTheDayEngine(QObject * parent, const QVariantList & args)
    : Plasma::DataEngine(parent,args)
{
    setMinimumPollingInterval(3600000);
    m_mediawiki["en.wikipedia.org"] = MediaWikiInfo(QUrl("http://en.wikipedia.org/w/api.php"), QString("Wikipedia:Today's_featured_article/"), QString(""));
}

void ArticleOfTheDayEngine::init() {
    Plasma::DataEngine::Data data;
    data["Wikipedia"] = QString("en.wikipedia.org");
    setData(QString("mediawiki"), data);
}

bool ArticleOfTheDayEngine::sourceRequestEvent(const QString & name) {
    return updateSourceEvent(name);
}

bool ArticleOfTheDayEngine::updateSourceEvent(const QString & source) {
    m_revisions.clear();

    if (sources().contains(source)) return true;

    QStringList sourceSplit = source.split(':');
    QDateTime date = QDateTime::fromString(sourceSplit[1],"yyyy-MM-dd");
    QLocale l(QLocale::English);

    if (sourceSplit.size() != 2 || !m_mediawiki.contains(sourceSplit[0])) return false;

    MediaWiki mediawiki(m_mediawiki[sourceSplit[0]].url, QString("articleofthedayengine"));

    const QString page(m_mediawiki[sourceSplit[0]].page + l.toString(date,"MMMM_d,_yyyy"));

    if (!searchText(mediawiki,page)) return false;

    Plasma::DataEngine::Data data;

    data["content"] = m_revision.content();

    setData(source, data);
    return true;
}

void ArticleOfTheDayEngine::result(const QList<Revision> & revisions)
{
    m_revisions.append(revisions);
}

bool ArticleOfTheDayEngine::searchText(MediaWiki & mediawiki, const QString & page)
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

K_EXPORT_PLASMA_DATAENGINE(articleoftheday, ArticleOfTheDayEngine)

#include "articleofthedayengine.moc"
