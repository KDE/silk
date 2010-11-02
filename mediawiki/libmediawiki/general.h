/*
 *   Copyright 2010 by Manuel Campomanes <campomanes.manuel@gmail.com>
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

#ifndef GENERAL_H
#define GENERAL_H

#include <kdemacros.h>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

class QNetworkReply;
class MediaWiki;


class KDE_EXPORT General : public QObject {

    Q_OBJECT

public:

    explicit General(MediaWiki const & mediawiki, QObject * parent = 0);

    virtual ~General();

    QString getMainpage();
    QString getBase();
    QString getSitename();
    QString getGenerator();
    QString getPhpversion();
    QString getPhpsapi();
    QString getDbtype();
    QString getDbversion();
    QString getRev();
    QString getCase();
    QString getRights();
    QString getLang();
    QString getFallback8bitencoding();
    QString getWriteapi();
    QString getTimezone();
    QString getTimeoffset();
    QString getArticlepath();
    QString getScriptpath();
    QString getScript();
    QString getVariantarticlepath();
    QString getServer();
    QString getWikiid();
    QString getTime();


private slots:

    void onFinished(QNetworkReply * reply);
signals:
    void finished(bool);

private:

    struct GeneralPrivate * const d;

};

#endif // USERGROUPS_H
