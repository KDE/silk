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
#include <kglobal.h>

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
	"};\n" \
	"function GM_getValue(key, default) {\n"			\
	"    return window.silk.GM_getValue(key, default);\n"		\
	"};\n"								\
	"function GM_setValue(key, value) {\n" \
	"    window.silk.GM_setValue(key, value);\n" \
	"};\n"
	);

    m_view->page()->mainFrame()->evaluateJavaScript( script );
}

KConfigGroup ScriptApi::config() const
{
    QString groupName("Silk-Script-Api-%1-%2-%3");
    groupName = groupName.arg( m_view->name() ).arg( m_gmNameSpace ).arg( m_gmName );

    return KGlobal::config()->group( groupName );
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
    return config().readEntry( key, defaultVal );
}

void ScriptApi::GM_setValue( const QString &key, const QVariant &value )
{
    config().writeEntry( key, value );
}

void ScriptApi::showNotification( const QString &text, QString icon )
{
    kDebug() << "notification:" << text << icon;
    int s = KIconLoader::SizeMedium;
    KNotification::event(QString("popup"),
                         text,
                         KIcon(icon).pixmap(QSize(s, s)));
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


bool ScriptApi::loadStyleSheet( const QString &filename )
{
    if ( !m_trusted ) {
        kDebug() << "Not trusted: Not including the stylesheet:" << filename;
        return false;
    }
    // TODO: validation of the filename
    //QString script("document.write('<link href=\"" + filename + "\" type=\"text/css\" rel=\"stylesheet\">');");


    QString script( "var fileref=document.createElement(\"link\");\n" \
                    "fileref.setAttribute(\"rel\", \"stylesheet\");\n" \
                    "fileref.setAttribute(\"type\", \"text/css\");\n" \
                    "fileref.setAttribute(\"href\", \"" + filename + "\");\n" \
                    "document.getElementsByTagName(\"head\")[0].appendChild(fileref);\n" \
                  );
    m_view->evaluateScript( script );
    kDebug() << "=============>>>>> included stylesheet" << script;

/*
    else if (filetype=="css"){ //if filename is an external CSS file
  var fileref=document.createElement("link")
  fileref.setAttribute("rel", "stylesheet")
  fileref.setAttribute("type", "text/css")
  fileref.setAttribute("href", filename)
 }
 if (typeof fileref!="undefined")
  document.getElementsByTagName("head")[0].appendChild(fileref)
*/
    return true;
}
