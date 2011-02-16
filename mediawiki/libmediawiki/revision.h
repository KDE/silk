

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
    * @brief Set the Revision ID
    * @param Id Revision ID
    **/
    void setRevId(int id);

    /**
     * @brief get the Revision ID
     */
    int revId() const;

    /**
     * @briefSet the parent ID
     * @param ID ID of parent
     */
    void setParentId(int id);

    /**
     * @brief get parent ID
     */
    int parentId() const;

    /**
     * @brief Set the size of the revision text in bytes
     * @param size size of revision text
     */
    void setSize(int size);

    /**
     * @brief get the size of the revision text
     */
    int size() const;

    /**
     * @brief Whether the revision was a minor edit
     * @param minor
     */
    void setMinor(const QString &  minor);

    /**
     * @brief Get Minor  // Missing doc
     */
    QString minor() const;


    /**
     * @brief  Get the date and time the revision was made
     */
    QDateTime timestamp() const;

    /**
     * @brief Set the date and time the revision was made
     * @param timestamp
     */
    void setTimestamp(const QDateTime & timestamp);

    /**
     * @brief Get the user who made the revision
     */
    QString user() const;

    /**
     * @brief Set the user who made the revision
     * @param user revision owner
     */
    void setUser(const QString & user);

    /**
     * @brief The revision content. If set, the maximum limit will be 10 times as low
     */
    QString content() const;

    /**
     * @brief Set the revision content
     * @param content
     */
    void setContent(const QString & content);


    /**
     * @brief Get the edit comment
     */
    QString comment() const;

    /**
     * @brief Set the edit comment
     * @param Comment the edit comment
     */
    void setComment(const QString &  com);

    /**
     *  @brief Set parse tree for revision content
     * @param parseTree
     */
    void setParseTree(const QString &  parseTree);


    /**
     * @brief Get parseTree
     */
    QString parseTree() const;

    /**
     * @brief Set rollback token
     * @param rollback token needed to rollback edits from a page
     */
    void setRollback(const QString &  rollback);

    /**
     * @brief get rollback token
     */
    QString rollback() const;

private:

    struct RevisionPrivate * d;

};
}


MEDIAWIKI_EXPORT bool operator==(const mediawiki::Revision & lhs,const mediawiki::Revision & rhs);

#endif
