#include <qwebview.h>
#include <qwebframe.h>

#include <kdebug.h>

#include "scriptapi.h"

ScriptApi::ScriptApi( QObject *parent )
    : QObject( parent )
{

}

void ScriptApi::setWebView( QWebView *view )
{
    QWebPage *page = view->page();
    frame = page->mainFrame();

    attachObject();
    connect( frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()) );
}

void ScriptApi::attachObject()
{
    frame->addToJavaScriptWindowObject( QString("silk"), this );
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

