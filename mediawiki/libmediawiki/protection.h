/*
 *   Copyright 2010 by Vincent Garcia <xavier.vincent.garcia@gmail.com>
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
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PROTECTION_H
#define PROTECTION_H
#include <QtCore/QString>
#include "mediawiki_export.h"

namespace mediawiki {

class MEDIAWIKI_EXPORT Protection {

public:

    Protection();

    ~Protection();

    Protection(const Protection & other);

    Protection & operator=(Protection other);

    void setType(const QString & type);

    QString Type() const;

    void setLevel(const QString & level);

    QString Level() const;

    void setExpiry(const QString & expiry);

    QString Expiry() const;

    void setSource(const QString & source);

    QString Source() const;

private:

    class ProtectionInfo * d;

};
}
#endif // PROTECTION_H


MEDIAWIKI_EXPORT bool operator==(const mediawiki::Protection & lhs, const mediawiki::Protection & rhs);
