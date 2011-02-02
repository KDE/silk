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

#ifndef ARTICLEOFTHEDAYENGINE_H
#define ARTICLEOFTHEDAYENGINE_H

#include <Plasma/DataEngine>

#include "queryrevision.h"

using namespace mediawiki;

//PATTERN: mediawiki:yyyy-MM-jj
class ArticleOfTheDayEngine : public Plasma::DataEngine {

    Q_OBJECT

public:

    ArticleOfTheDayEngine(QObject * parent, const QVariantList & args);

protected:

    virtual void init();

    virtual bool sourceRequestEvent(const QString & name);

    virtual bool updateSourceEvent(const QString & source);

private slots:

    void result(const QList<Revision> & revisions);

private:

    struct MediaWikiInfo {
        MediaWikiInfo() {}
        MediaWikiInfo(const QUrl & url, const QString & page, const QString & lang) : url(url), page(page), lang(lang) {}
        QUrl url;
        QString page;
        QString lang;
    };

    bool searchText(MediaWiki & mediawiki, const QString & page);

    QList<Revision> m_revisions;
    Revision m_revision;
    QMap<QString, MediaWikiInfo> m_mediawiki;

};

#endif // ARTICLEOFTHEDAYENGINE_H
