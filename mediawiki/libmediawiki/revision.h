/*
 *   Copyright 2011 Robin Bussenot <bussenot.robin@gmail.com>
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

    /**
     * @brief Constructs a revision.
     */
    Revision();

    /**
     * @brief Destructs a revision.
     */
    ~Revision();

    /**
     * @brief Constructs a revision from an other revision.
     * @param other an other revision
     */
    Revision(const Revision & other);

    /**
     * @brief Assingning a revision from an other revision.
     * @param other an other revision
     */
    Revision & operator=(Revision other);

    /**
    * @brief Set the revision ID.
    * @param revisionId the revision ID
    **/
    void setRevisionId(int revisionId);

    /**
     * @brief Get the revision ID.
     * @return the revision ID
     */
    int revisionId() const;

    /**
     * @brief Set the parent ID.
     * @param parentId the parent ID
     */
    void setParentId(int parentId);

    /**
     * @brief Get the parent ID.
     * @return the parent ID
     */
    int parentId() const;

    /**
     * @brief Set the size of the revision text in bytes.
     * @param size the size of the revision text in bytes
     */
    void setSize(int size);

    /**
     * @brief Get the size of the revision text in bytes.
     * @return the size of the revision text in bytes
     */
    int size() const;

    /**
     * @brief Whether the revision was a minor edit
     * @param minor
     */
    //FIXME: Must be a bool because in xml when minor="" is set means that a minor version.
    void setMinor(const QString &  minor);

    /**
     * @brief Get Tags if the revision is minor
     */
    QString minor() const;

    /**
     * @brief Get the date and time of the revision.
     * @return the date and time of the revision
     */
    QDateTime timestamp() const;

    /**
     * @brief Set the date and time of the revision
     * @param timestamp the date and time of the revision
     */
    void setTimestamp(const QDateTime & timestamp);

    /**
     * @brief Get the user who made the revision.
     * @return the user who made the revision
     */
    QString user() const;

    /**
     * @brief Set the user who made the revision.
     * @param user the user who made the revision
     */
    void setUser(const QString & user);

    /**
     * @brief The revision content.
     */
    QString content() const;

    /**
     * @brief Set the revision content.
     * @param content the revision content
     */
    void setContent(const QString & content);

    /**
     * @brief Get the edit comment.
     * @return the edit comment
     */
    QString comment() const;

    /**
     * @brief Set the edit comment.
     * @param comment the edit comment
     */
    void setComment(const QString & comment);

    /**
     * @brief Set the parse tree of the revision content.
     * @param parseTree the parse tree of the revision content
     */
    void setParseTree(const QString & parseTree);

    /**
     * @brief Get the parse tree of the revision content.
     * @return the parse tree of the revision content
     */
    QString parseTree() const;

    /**
     * @brief Set the rollback token.
     * @param rollback the rollback token
     */
    void setRollback(const QString &  rollback);

    /**
     * @brief Get the rollback token.
     * @return the rollback token
     */
    QString rollback() const;

private:

    struct RevisionPrivate * d;

};
}


MEDIAWIKI_EXPORT bool operator==(const mediawiki::Revision & lhs,const mediawiki::Revision & rhs);

#endif
