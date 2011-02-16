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

    /**
     * @brief Constructs a page.
     */
    Page();

    /**
     * @brief Constructs a page from an other page.
     * @param other an other page
     */
    Page(const Page & other);

    /**
     * @brief Destructs a page.
     */
    ~Page();

    /**
     * @brief Assingning a page from an other page.
     * @param other an other page
     */
    Page & operator=(Page other);

    /**
     * @brief Set the pageId of the page.
     * @param id the page id of the page
     */
    void setPageId(unsigned int id);

    /**
     * @brief Return the page id of the page.
     * @return the page id of the page
     */
    unsigned int pageId() const;

    /**
     * @brief Set the title of the page.
     * @param title the title of the page
     */
    void setTitle(QString title);

    /**
     * @brief Return the title of the page.
     * @return the title of the page
     */
    QString pageTitle() const;

    /**
     * @brief Set the namespace of the page.
     * @param ns the namespace of the page
     */
    void setNs(unsigned int ns) const;

    /**
     * @brief Return the namespace of the page.
     * @return the namespace of the page
     */
    unsigned int pageNs() const;

    /**
     * @brief Set the last revision id of the page.
     * @param lastRevId the last revision id of the page
     */
    void setLastRevId(unsigned int lastRevId) const;

    /**
     * @brief Return the last revision id of the page.
     * @return the last revision id of the page
     */
    unsigned int pageLastRevId() const;

    /**
     * @brief Set the number of views of the page.
     * @param counter the number of views of the page
     */
    void setCounter(unsigned int counter) const;

    /**
     * @brief Return the number of views of the page.
     * @return the number of views of the page
     */
    unsigned int pageCounter() const;

    /**
     * @brief Set the page size.
     * @param length the page size
     */
    void setLength(unsigned int length) const;

    /**
     * @brief Return the page size.
     * @return the page size
     */
    unsigned int pageLength() const;

    /**
     * @brief Set the page token.
     * @param editToken the page token
     */
    void setEditToken(QString editToken);

    /**
     * @brief Return the page token.
     * @return the page token
     */
    QString pageEditToken() const;

    /**
     * @brief Set the page ID of the talk page for each non-talk page.
     * @param talkid the page ID of the talk page for each non-talk page
     */
    void setTalkid(unsigned int talkid) const;

    /**
     * @brief Return the page ID of the talk page for each non-talk page.
     * @return the page ID of the talk page for each non-talk page
     */
    unsigned int pageTalkid() const;

    /**
     * @brief Set the full url of the page.
     * @param fullurl the full url of the page
     */
    void setFullurl(QUrl fullurl);

    /**
     * @brief Return the full url of the page.
     * @return the full url of the page
     */
    QUrl pageFullurl() const;

    /**
     * @brief Set the edit url of the page.
     * @param editurl the edit url of the page
     */
    void setEditurl(QUrl editurl);

    /**
     * @brief Return the edit url of the page.
     * @return the edit url of the page
     */
    QUrl pageEditurl() const;

    /**
     * @brief Set the readability of the page.
     * @param readable the readability of the page
     */
    void setReadable(QString readable);

    /**
     * @brief Return the readability of the page.
     * @return the readability of the page
     */
    QString pageReadable() const;

    /**
     * @brief Set the text returned by EditFormPreloadText.
     * @param preload the text returned by EditFormPreloadText
     */
    void setPreload(QString preload);

    /**
     * @brief Return the text returned by EditFormPreloadText.
     * @return the text returned by EditFormPreloadText
     */
    QString pagePreload() const;

    /**
     * @brief Set the last touched timestamp.
     * @param touched the last touched timestamp
     */
    void setTouched(QDateTime touched);

    /**
     * @brief Return the last touched timestamp.
     * @return the last touched timestamp
     */
    QDateTime pageTouched() const;

    /**
     * @brief Set the timestamp when you obtained the edit token.
     * @param starttimestamp the timestamp when you obtained the edit token
     */
    void setStarttimestamp(QDateTime starttimestamp);

    /**
     * @brief Return the timestamp when you obtained the edit token.
     * @return the timestamp when you obtained the edit token
     */
    QDateTime pageStarttimestamp() const;

private:

    struct PagePrivate * d;

};
}

MEDIAWIKI_EXPORT bool operator==(const mediawiki::Page & lhs,const mediawiki::Page & rhs);

#endif // PAGE_H
