

#ifndef MEDIAWIKI_REVISION_H
#define MEDIAWIKI_REVISION_H

#include "mediawiki_export.h"
#include <QDateTime>

namespace mediawiki {

/**
 * @brief An image info.
 */
class MEDIAWIKI_EXPORT Revision {

public:
    Revision();
    /**
     * @brief Destructs an image info.
     */
    ~Revision();

    /**
     * @brief Construct by copy.
     */
    Revision(const Revision & other);


    /**
     * @brief Assingning an revision from an other revision .
     * @param other an other revision
     */
    Revision & operator=(Revision other);
    /**
     * @param Revision Id
     */
    void setRevId(int id);

    /**
     * @brief
     */
    int revId() const;

    /**
     * @brief
     * @param
     */
    void setParentId(int id);

    /**
     * @brief
     */
    int parentId() const;

    /**
     * @brief
     * @param
     */
    void setSize(int size);

    /**
     * @brief
     */
    int size() const;

    /**
     * @brief
     * @param
     */
    void setMinor(const QString &  minor);

    /**
     * @brief
     * @param
     */
    QString minor() const;


    /**
     * @brief
     */
    QDateTime timestamp() const;

    /**
     * @brief
     * @param
     */
    void setTimestamp(const QDateTime & timestamp);

    /**
     * @brief
     * @param
     */
    QString user() const;

    /**
     * @brief
     * @param
     */
    void setUser(const QString & user);

    /**
     * @brief
     */
    QString content() const;

    /**
     * @brief
     * @param
     */
    void setContent(const QString & content);


    /**
     * @brief
     */
    QString comment() const;

    /**
     * @brief
     * @param
     */
    void setComment(const QString &  com);

    /**
     * @brief
     * @param
     */
    void setParseTree(const QString &  parseTree);


    /**
     * @brief
     */
    QString parseTree() const;

    /**
     * @brief
     * @param
     */
    void setRollback(const QString &  rollback);

    /**
     * @brief
     */
    QString rollback() const;

private:

    struct RevisionPrivate * d;

};
}


MEDIAWIKI_EXPORT bool operator==(const mediawiki::Revision & lhs,const mediawiki::Revision & rhs);

#endif
