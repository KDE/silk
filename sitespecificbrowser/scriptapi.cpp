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

#include <qwebview.h>
#include <qwebframe.h>
#include <qfile.h>
#include <qtextstream.h>

#include <kdebug.h>
#include <kpassivepopup.h>
#include <knotification.h>

#include "view.h"
#include "scriptapi.h"

ScriptApi::ScriptApi( QObject *parent )
    : QObject( parent ),
      m_view(0),
      m_trusted(false)
{

}

void ScriptApi::setWebView( View *view )
{
    m_view = view;
    attachObject();
    connect( m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()) );
    installGlobals();
}

void ScriptApi::attachObject()
{
    m_view->page()->mainFrame()->addToJavaScriptWindowObject( QString("silk"), this );
}

void ScriptApi::installGlobals()
{
    QString script(
	"function GM_log(message) {\n" \
	"    window.silk.GM_log(message);\n" \
	"};\n"
	);

    m_view->page()->mainFrame()->evaluateJavaScript( script );
}

void ScriptApi::setTrusted( bool yes )
{
    m_trusted = yes;
}

void ScriptApi::setGreaseMonkeyName( const QString &name )
{
    m_gmName = name;
}

void ScriptApi::setGreaseMonkeyNameSpace( const QString &gmnamespace )
{
    m_gmNameSpace = gmnamespace;
}

void ScriptApi::GM_log( const QString &message )
{
    kDebug() << message;
}

QVariant ScriptApi::GM_getValue( const QString &key, const QVariant &defaultVal )
{
    return defaultVal;
}

void ScriptApi::GM_setValue( const QString &key, const QVariant &value )
{
}

void ScriptApi::showNotification( const QString &text )
{
    KPassivePopup::message( text, m_view );
//    KNotification::event( QString("script_message"), text );
}

bool ScriptApi::include( const QString &filename )
{
    if (!m_trusted)
        return false;

    QFile f( filename );
    if ( !f.open( QIODevice::ReadOnly ) ) {
        kDebug() << "Unable to open file " << filename;
        return false;
    }

    QTextStream ts( &f );
    m_view->evaluateScript( ts.readAll() );

    return true;
}

