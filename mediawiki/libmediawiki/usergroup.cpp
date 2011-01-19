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

#include "usergroup.h"

namespace mediawiki {

struct UserGroupPrivate
{
    QString name;

    QList<QString> rights;

    unsigned int number;
};

}

using namespace mediawiki;

UserGroup::UserGroup()
    : d(new UserGroupPrivate())
{
    d->number = -1;
}

UserGroup::UserGroup(const UserGroup & other)
    : d(new UserGroupPrivate(*(other.d)))
{}

UserGroup::~UserGroup()
{
    delete d;
}

UserGroup & UserGroup::operator=(UserGroup other)
{
    std::swap(d, other.d);
    return *this;
}

QString UserGroup::name() const
{
    return d->name;
}

void UserGroup::setName(const QString & name)
{
    d->name = name;
}

const QList<QString> & UserGroup::rights() const
{
    return d->rights;
}

QList<QString> & UserGroup::rights()
{
    return d->rights;
}

void UserGroup::setRights(const QList<QString> & rights)
{
    d->rights = rights;
}

qint64 UserGroup::number() const
{
    return d->number;
}

void UserGroup::setNumber(qint64 number)
{
    d->number = number;
}

bool operator==(const UserGroup & lhs, const UserGroup & rhs)
{
    return lhs.number() == rhs.number() &&
           lhs.rights() == rhs.rights() &&
           lhs.name() == rhs.name() ;
}