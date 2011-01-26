#include "page.h"
#include <QtCore/QUrl>

namespace mediawiki {

    struct PagePrivate {

        unsigned int m_pageid;
        QString m_title;
        unsigned int m_ns;
        unsigned int m_lastrevid;
        unsigned int m_counter;
        unsigned int m_length;
        QString m_edittoken ;
        unsigned int m_talkid ;
        QUrl m_fullurl;
        QUrl m_editurl;
        QString m_readable;
        QString m_preload;
        QDateTime m_touched;
        QDateTime m_starttimestamp;
    };
}
using namespace mediawiki;

Page::Page()
    :d(new PagePrivate())
{

}

Page::~Page()
{
    delete d;
}


Page::Page( const Page & other)
        : d(new PagePrivate(*(other.d)))
{
}

Page & Page::operator=(Page other)
{
    std::swap(d, other.d);
    return *this;
}

void Page::setPageId(unsigned int id)
{
    d->m_pageid=id;
}

unsigned int Page::pageId() const
{
    return d->m_pageid;
}

void Page::setTitle(QString title)
{
    d->m_title=title;
}

QString Page::pageTitle() const
{
    return d->m_title;
}

void Page::setNs(unsigned int ns) const
{
    d->m_ns=ns;
}

unsigned int Page::pageNs() const
{
    return d->m_ns;
}

void Page::setLastRevId(unsigned int lastRevId) const
{
    d->m_lastrevid=lastRevId;
}

unsigned int Page::pageLastRevId() const
{
    return d->m_lastrevid;
}

void Page::setCounter(unsigned int counter) const
{
    d->m_counter=counter;
}

unsigned int Page::pageCounter() const
{
    return d->m_counter;
}

void Page::setLength(unsigned int length) const
{
     d->m_length=length;
}

unsigned int Page::pageLength() const
{
     return d->m_length;
}

void Page::setEditToken(QString editToken)
{
    d->m_edittoken=editToken;
}

QString Page::pageEditToken() const
{
    return d->m_edittoken;
}

void Page::setTalkid(unsigned int talkid) const
{
     d->m_talkid=talkid;
}

unsigned int Page::pageTalkid() const
{
     return d->m_talkid;
}

void Page::setFullurl(QUrl fullurl)
{
    d->m_fullurl=fullurl;
}

QUrl Page::pageFullurl() const
{
    return d->m_fullurl;
}

void Page::setEditurl(QUrl editurl)
{
    d->m_editurl=editurl;
}

QUrl Page::pageEditurl() const
{
    return d->m_editurl;
}

void Page::setReadable(QString readable)
{
    d->m_readable=readable;
}

QString Page::pageReadable() const
{
    return d->m_readable;
}

void Page::setPreload(QString preload)
{
    d->m_preload=preload;
}

QString Page::pagePreload() const
{
    return d->m_preload;
}

void Page::setTouched(QDateTime touched)
{
    d->m_touched=touched;
}

QDateTime Page::pageTouched() const
{
    return d->m_touched;
}

void Page::setStarttimestamp(QDateTime starttimestamp)
{
    d->m_starttimestamp=starttimestamp;
}

QDateTime Page::pageStarttimestamp() const
{
    return d->m_starttimestamp;
}

bool operator==(const mediawiki::Page & lhs,const mediawiki::Page & rhs)
{
    return lhs.pageId() == rhs.pageId() &&
           lhs.pageTitle() == rhs.pageTitle() &&
           lhs.pageNs() == rhs.pageNs() &&
           lhs.pageLastRevId() == rhs.pageLastRevId() &&
           lhs.pageCounter() == rhs.pageCounter() &&
           lhs.pageLength() == rhs.pageLength() &&
           lhs.pageEditToken() == rhs.pageEditToken() &&
           lhs.pageTalkid() == rhs.pageTalkid() &&
           lhs.pageFullurl() == rhs.pageFullurl() &&
           lhs.pageEditurl() == rhs.pageEditurl() &&
           lhs.pageReadable() == rhs.pageReadable() &&
           lhs.pagePreload() == rhs.pagePreload() &&
           lhs.pageTouched() == rhs.pageTouched() &&
           lhs.pageStarttimestamp() == rhs.pageStarttimestamp();
}
