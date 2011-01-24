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

void Revision::setRevId(int id)
{
    d->revId=id;
}

int Revision::revId() const
{
    return d->revId;
}

void Revision::setParentId(int id)
{
    d->parentId=id;
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
    d->comment=com;
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


QString Revision::Rollback() const
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
           lhs.Rollback() == rhs.Rollback()&&
           lhs.revId() == rhs.revId();
}
