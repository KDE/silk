#ifndef MEDIAWIKI_PAGE_H
#define MEDIAWIKI_PAGE_H

#include "mediawiki_export.h"
#include <QDateTime>
#include <QtCore/QUrl>

namespace mediawiki {

/**
 * @brief An image info.
 */
class MEDIAWIKI_EXPORT Page {

public:

    Page();

    ~Page();

    Page(const Page & other);

    /**
     * @brief Assingning an revision from an other revision .
     * @param other an other revision
     */
    Page & operator=(Page other);

    void Page::setPageId(unsigned int id);

    /**
     * @brief
     */
    unsigned int Page::pageId() const;

    void Page::setTitle(QString title);

    /**
     * @brief
     */
    QString Page::pageTitle() const;

    void Page::setNs(unsigned int ns) const;

    /**
     * @brief
     */
    unsigned int Page::pageNs() const;

    void Page::setLastRevId(unsigned int lastRevId) const;

    /**
     * @brief
     */
    unsigned int Page::pageLastRevId() const;

    void Page::setCounter(unsigned int counter) const;

    /**
     * @brief
     */
    unsigned int Page::pageCounter() const;

    void Page::setLength(unsigned int length) const;

    /**
     * @brief
     */
    unsigned int Page::pageLength() const;

    void Page::setEditToken(QString editToken);

    /**
     * @brief
     */
    QString Page::pageEditToken() const;

    void Page::setTalkid(unsigned int talkid) const;

    /**
     * @brief
     */
    unsigned int Page::pageTalkid() const;

    void Page::setFullurl(QUrl fullurl);

    /**
     * @brief
     */
    QUrl Page::pageFullurl() const;

    void Page::setEditurl(QUrl editurl);

    /**
     * @brief
     */
    QUrl Page::pageEditurl() const;

    void Page::setReadable(QString readable);

    /**
     * @brief
     */
    QString Page::pageReadable() const;

    void Page::setPreload(QString preload);

    /**
     * @brief
     */
    QString Page::pagePreload() const;

    void Page::setTouched(QDateTime touched);

    /**
     * @brief
     */
    QDateTime Page::pageTouched() const;

    void Page::setStarttimestamp(QDateTime starttimestamp);

    /**
     * @brief
     */
    QDateTime Page::pageStarttimestamp() const;

private:

    struct PagePrivate * d;

};
}

MEDIAWIKI_EXPORT bool operator==(const mediawiki::Page & lhs,const mediawiki::Page & rhs);

#endif // PAGE_H
