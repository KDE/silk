#ifndef USERGROUP_H
#define USERGROUP_H

#include <QtCore/QString>
#include <QtCore/QList>

#include "mediawiki_export.h"

namespace mediawiki {

/**
 * @brief A user group
 */
class MEDIAWIKI_EXPORT UserGroup
{

public:

    /**
     * Constructs a user group.
     */
    UserGroup();

    /**
     * @brief Constructs a user group from an other user group.
     * @param other an other user group
     */
    UserGroup(const UserGroup & other);

    /**
     * @brief Destructs a user group .
     */
    ~UserGroup();

    /**
     * @brief Assingning a user group from an other user group.
     * @param other an other user group
     */
    UserGroup & operator=(UserGroup other);

    /**
     * @brief Returns the name of the user group.
     * @return the name of the user group
     */
    QString name() const;

    /**
     * @brief Set the name of the user group.
     * @param name the name of the user group
     */
    void setName(const QString & name);

    /**
     * @brief Returns rights of the user group.
     * @return rights of the user group
     */
    const QList<QString> & rights() const;

    /**
     * @brief Returns rights of the user group.
     * @return rights of the user group
     */
    QList<QString> & rights();

    /**
     * @brief Set rights of the user group.
     * @param rights rights of the user group
     */
     void setRights(const QList<QString> & rights);

    /**
     * @brief Returns the numbers of users in the user group.
     * @return the numbers of users in the user group
     */
    qint64 number() const;

    /**
     * @brief Set the number of users in the user group.
     * @param number the number of users in the user group
     */
    void setNumber(qint64 number) ;

private:

    struct UserGroupPrivate * d;

};

}

/**
 * @brief Returns true if lhs and rhs are equal, else false.
 * @param lhs left-hand side user group
 * @param rhs right-hand side user group
 * @return true if lhs and rhs are equal, else false
 */
MEDIAWIKI_EXPORT bool operator==(const mediawiki::UserGroup & lhs, const mediawiki::UserGroup & rhs);

#endif // USERGROUP_H
