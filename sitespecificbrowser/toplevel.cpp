#include <qdebug.h>
#include <QtNetwork/QNetworkRequest>
#include <QtGui/QDesktopServices>
#include <qwebsettings.h>
#include <qwebframe.h>
#include <qsettings.h>
#include <qdir.h>

#include "toplevel.h"

Page::Page( TopLevel *toplevel )
    : QWebPage( toplevel ) 
{
    this->toplevel = toplevel;
}

bool Page::acceptNavigationRequest( QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type )
{
    if ( type == QWebPage::NavigationTypeLinkClicked ) {
	bool inScope = false;
	QUrl base;
	foreach( base, toplevel->options()->allowedBases ) {
	    if ( base.isParentOf( request.url() ) ) {
		inScope = true;
	    }
	}

	if ( !inScope ) {
	    QDesktopServices::openUrl( request.url() );
	    return false;
	}
    }

    return QWebPage::acceptNavigationRequest( frame, request, type );
}


TopLevel::TopLevel()
    : QWebView(0)
{
    m_options = new WebAppOptions;
#if 0
    load( QString("gmail.ini") );
#else
    m_options->startUrl = QUrl("http://mail.google.com/");
    m_options->windowTitle = QString("GMail");
    m_options->allowedBases.append( QUrl("http://mail.google.com/") );

    save( QString("gmail.ini") ); // HACK
#endif

    m_page = new Page( this );
    connect( m_page->mainFrame(), SIGNAL( iconChanged() ), SLOT( iconLoaded() ) );
    setPage( m_page );

    QWebSettings::globalSettings()->setAttribute( QWebSettings::PluginsEnabled, true );
    QWebSettings::setIconDatabasePath( QDir::currentPath() );

    setWindowTitle( m_options->windowTitle );
    setUrl( m_options->startUrl );

       QIcon icon = QWebSettings::iconForUrl( QUrl("http://mail.google.com/mail/") );
    //QIcon icon = m_page->mainFrame()->icon();
    qDebug() << "Is icon null: " << icon.isNull();
    if ( !icon.isNull() )
	setWindowIcon( icon );

}

WebAppOptions *TopLevel::options() const
{
    return m_options;
}

bool TopLevel::load( const QString &filename )
{
    // TODO
    QSettings settings( filename, QSettings::IniFormat );

    m_options->startUrl = QUrl( settings.value( QString("StartUrl") ).toString() );
    m_options->windowTitle = settings.value( QString("WindowTitle") ).toString();

    int size = settings.beginReadArray( QString("AllowedBases") );
    for ( int i = 0; i < size; ++i ) {
	settings.setArrayIndex( i );
	m_options->allowedBases.append( QUrl( settings.value( QString("Url") ).toString() ) );
    }
    settings.endArray();

    return true;
}

bool TopLevel::save( const QString &filename )
{
    QSettings settings( filename, QSettings::IniFormat );

    settings.setValue( QString("StartUrl"), m_options->startUrl.toString() );
    settings.setValue( QString("WindowTitle"), m_options->windowTitle );

    settings.beginWriteArray( QString("AllowedBases") );
    for ( int i = 0; i < m_options->allowedBases.size(); ++i ) {
	settings.setArrayIndex( i );
	settings.setValue( QString("Url"), m_options->allowedBases[i].toString() );
    }
    settings.endArray();

    settings.sync();

    return true;
}

void TopLevel::iconLoaded()
{
    qDebug() << "Got icon";
    qDebug() << m_page->mainFrame()->icon().isNull();
    setWindowIcon( m_page->mainFrame()->icon() );

//    QIcon icon = QWebSettings::iconForUrl( QUrl("http://mail.google.com/") );
//    qDebug() << icon;
}
