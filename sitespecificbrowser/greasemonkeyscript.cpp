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

    QRegExp name( "//\\s*@name\\s+(.*)$" );
    QRegExp nameSpace( "//\\s*@namespace\\s+(.*)$" );
    QRegExp description( "//\\s*@description\\s+(.*)$" );
    QRegExp include( "//\\s*@include\\s+(.*)$" );
    QRegExp exclude( "//\\s*@exclude\\s+(.*)$" );

    QString line = ts->readLine();
    do {
	if ( line == QString("// ==/UserScript==") ) {
	    break;
	}
	else if ( name.exactMatch(line) ) {
 	    m_name = name.cap(1);
	}
	else if ( nameSpace.exactMatch(line) ) {
 	    m_nameSpace = nameSpace.cap(1);
	}
	else if ( description.exactMatch(line) ) {
 	    m_description = description.cap(1);
	}
	else if ( include.exactMatch(line) ) {
 	    m_includeList += include.cap(1);
	}
	else if ( exclude.exactMatch(line) ) {
 	    m_excludeList += exclude.cap(1);
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
