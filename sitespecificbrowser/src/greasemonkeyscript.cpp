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

#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

#include <kdebug.h>

#include "greasemonkeyscript.h"

GreaseMonkeyScript::GreaseMonkeyScript()
{
}

bool GreaseMonkeyScript::load( const QString &filename )
{
    QFile f( filename );
    if ( !f.open(QIODevice::ReadOnly) ) {
        kDebug() << "Unable to load file " << filename;
        return false;
    }

    bool ok = false;
    QTextStream ts( &f );

    QString line = ts.readLine();
    do {
        if ( line == QString("// ==UserScript==") ) {
            ok = extractMetaData( &ts );
            break;
        }
        line = ts.readLine();
    } while( !line.isNull() );

    return ok;
}

// The stream should be positioned just after the opening line
// of the UserScript comment.
bool GreaseMonkeyScript::extractMetaData( QTextStream *ts )
{
    bool ok = false;

    // Note the trimmed() calls below should not be needed. There's either
    // a QRegExp bug or I'm doing something daft.
    QRegExp name( "//\\s*@name\\s+(.*)$" );
    QRegExp nameSpace( "//\\s*@namespace\\s+(.*)$" );
    QRegExp description( "//\\s*@description\\s+(.*)$" );
    QRegExp include( "//\\s*@include\\s+(.*)$" );
    QRegExp exclude( "//\\s*@exclude\\s+(.*)$" );

    QString line = ts->readLine();
    do {
        if ( line == QString("// ==/UserScript==") ) {
            ok = true;
            break;
        }
        else if ( name.exactMatch(line) ) {
            m_name = name.cap(1).trimmed();
        }
        else if ( nameSpace.exactMatch(line) ) {
            m_nameSpace = nameSpace.cap(1).trimmed();
        }
        else if ( description.exactMatch(line) ) {
            m_description = description.cap(1).trimmed();
        }
        else if ( include.exactMatch(line) ) {
            m_includeList += include.cap(1).trimmed();
        }
        else if ( exclude.exactMatch(line) ) {
            m_excludeList += exclude.cap(1).trimmed();
        }
        line = ts->readLine();
    } while( !line.isNull() );

    return ok;
}

void GreaseMonkeyScript::dump()
{
    kDebug() << "Name:" << m_name;
    kDebug() << "Namespace:" << m_nameSpace;
    kDebug() << "Description:" << m_description;
    kDebug() << "Include:" << m_includeList;
    kDebug() << "Exclude:" << m_excludeList;
}

QString GreaseMonkeyScript::nameSpace() const
{
    return m_nameSpace;
}

QString GreaseMonkeyScript::name() const
{
    return m_name;
}

QString GreaseMonkeyScript::description() const
{
    return m_description;
}

QStringList GreaseMonkeyScript::includeList() const
{
    return m_includeList;
}

QStringList GreaseMonkeyScript::excludeList() const
{
    return m_excludeList;
}
