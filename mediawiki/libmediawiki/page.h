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

    void setPageId(unsigned int id);

    /**
     * @brief
     */
    unsigned int pageId() const;

    void setTitle(QString title);

    /**
     * @brief
     */
    QString pageTitle() const;

    void setNs(unsigned int ns) const;

    /**
     * @brief
     */
    unsigned int pageNs() const;

    void setLastRevId(unsigned int lastRevId) const;

    /**
     * @brief
     */
    unsigned int pageLastRevId() const;

    void setCounter(unsigned int counter) const;

    /**
     * @brief
     */
    unsigned int pageCounter() const;

    void setLength(unsigned int length) const;

    /**
     * @brief
     */
    unsigned int pageLength() const;

    void setEditToken(QString editToken);

    /**
     * @brief
     */
    QString pageEditToken() const;

    void setTalkid(unsigned int talkid) const;

    /**
     * @brief
     */
    unsigned int pageTalkid() const;

    void setFullurl(QUrl fullurl);

    /**
     * @brief
     */
    QUrl pageFullurl() const;

    void setEditurl(QUrl editurl);

    /**
     * @brief
     */
    QUrl pageEditurl() const;

    void setReadable(QString readable);

    /**
     * @brief
     */
    QString pageReadable() const;

    void setPreload(QString preload);

    /**
     * @brief
     */
    QString pagePreload() const;

    void setTouched(QDateTime touched);

    /**
     * @brief
     */
    QDateTime pageTouched() const;

    void setStarttimestamp(QDateTime starttimestamp);

    /**
     * @brief
     */
    QDateTime pageStarttimestamp() const;

private:

    struct PagePrivate * d;

};
}

MEDIAWIKI_EXPORT bool operator==(const mediawiki::Page & lhs,const mediawiki::Page & rhs);

#endif // PAGE_H
