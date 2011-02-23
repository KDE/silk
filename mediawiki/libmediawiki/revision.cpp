/*
 *   Copyright 2011 Robin Bussenot bussenot.robin@gmail.com
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

#include "revision.h"
#include <QDateTime>

namespace mediawiki {

    struct RevisionPrivate {

        int revId;
        int parentId;
        int size;
        QString minor;
        QString user;
        QDateTime timestamp;
        QString comment;
        QString content;
        QString parseTree;
        QString rollback;
    };
}
using namespace mediawiki;

Revision::Revision()
    :d(new RevisionPrivate())
{
    d->revId=-1;
    d->parentId=-1;
    d->size=-1;
}
Revision::~Revision()
{
    delete d;
}
Revision::Revision( const Revision & other)
        : d(new RevisionPrivate(*(other.d)))
{
}

Revision & Revision::operator=(Revision other)
{
    std::swap(d, other.d);
    return *this;
}

void Revision::setRevisionId(int revisionId)
{
    d->revId=revisionId;
}

int Revision::revisionId() const
{
    return d->revId;
}

void Revision::setParentId(int parentId)
{
    d->parentId=parentId;
}

int Revision::parentId() const
{
    return d->parentId;
}

void Revision::setSize(int size)
{
    d->size=size;
}

int Revision::size() const
{
    return d->size;
}

void Revision::setMinor(const QString &  minor)
{
    d->minor=minor;
}
QString Revision::minor() const
{
    return d->minor;
}

QDateTime Revision::timestamp() const
{
    return d->timestamp;
}

void Revision::setTimestamp(const QDateTime & timestamp)
{
    d->timestamp = timestamp;
}

QString Revision::user() const
{
    return d->user;
}

void Revision::setUser(const QString & user)
{
    d->user = user;
}

void Revision::setComment(const QString &  com)
{
    d->comment = com;
}

QString Revision::comment() const
{
    return d->comment;
}

QString Revision::content() const
{
    return d->content;
}

void Revision::setContent(const QString &  content)
{
    d->content=content;
}

void Revision::setParseTree(const QString &  parseTree)
{
    d->parseTree=parseTree;
}


QString Revision::parseTree() const
{
    return d->parseTree;
}

void Revision::setRollback(const QString &  rollback)
{
    d->parseTree=rollback;
}


QString Revision::rollback() const
{
    return d->rollback;
}


bool operator==(const mediawiki::Revision & lhs, const mediawiki::Revision & rhs)
{
    return lhs.timestamp() == rhs.timestamp() &&
           lhs.user() == rhs.user() &&
           lhs.comment() == rhs.comment() &&
           lhs.content()== rhs.content()&&
           lhs.size() == rhs.size() &&
           lhs.minor()==rhs.minor()&&
           lhs.parseTree() == rhs.parseTree()&&
           lhs.parentId() == rhs.parentId()&&
           lhs.rollback() == rhs.rollback()&&
           lhs.revisionId() == rhs.revisionId();
}
