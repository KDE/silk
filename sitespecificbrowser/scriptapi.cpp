#include <qwebview.h>
#include <qwebframe.h>
#include <qfile.h>
#include <qtextstream.h>

#include <kdebug.h>

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
}

void ScriptApi::attachObject()
{
    m_view->page()->mainFrame()->addToJavaScriptWindowObject( QString("silk"), this );
}

void ScriptApi::setTrusted( bool yes )
{
    m_trusted = yes;
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

