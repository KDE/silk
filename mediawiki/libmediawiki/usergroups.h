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

#ifndef USERGROUPS_H
#define USERGROUPS_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace mediawiki {

class MediaWiki;

class UserGroups : public QObject {

    Q_OBJECT

public:

    struct Result {

    public:

        QString name;

        QList<QString> rights;

    };

    explicit UserGroups(MediaWiki const & mediawiki, QObject * parent = 0);

    virtual ~UserGroups();
    
signals:

    void finished(QList<UserGroups::Result> const & usergroups);

private slots:

    void processReply();

private:

    struct UserGroupsPrivate * const d;

    UserGroups(UserGroups const &);

    UserGroups & operator=(UserGroups const &);

};

}

#endif // USERGROUPS_H

