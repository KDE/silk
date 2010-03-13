/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GREASEMONKEYSCRIPT_H
#define GREASEMONKEYSCRIPT_H

#include <qstring.h>
#include <qstringlist.h>

/**
 * Loads a script and parses the greasemonkey metadata.
 */
class GreaseMonkeyScript
{
public:
    GreaseMonkeyScript();

    bool load( const QString &filename );

    QString nameSpace() const;
    QString name() const;
    QString description() const;
    QStringList includeList() const;
    QStringList excludeList() const;

    void dump();

protected:
    bool extractMetaData( QTextStream *ts );

private:
    QString m_nameSpace;
    QString m_name;
    QString m_description;
    QStringList m_includeList;
    QStringList m_excludeList;
};

#endif // GREASEMONKEYSCRIPT_H

