#include <qdebug.h>
#include <qwebsettings.h>
#include <qwebframe.h>
#include <qsettings.h>
#include <qsignalmapper.h>
#include <qdir.h>

#include "page.h"
#include "view.h"

View::View( QWidget *parent )
    : QWebView(parent)
{
    m_mapper = new QSignalMapper( this );
    connect( m_mapper, SIGNAL( mapped(const QString &) ), SLOT(actionTriggered(const QString &)) );

    m_options = new WebAppOptions;
    setupApplication();

    m_page = new Page( this );
    connect( m_page->mainFrame(), SIGNAL( iconChanged() ), SLOT( iconLoaded() ) );
    setPage( m_page );

    QWebSettings::globalSettings()->setAttribute( QWebSettings::PluginsEnabled, true );
    QWebSettings::setIconDatabasePath( QDir::currentPath() );

    setWindowTitle( m_options->windowTitle );
    setUrl( m_options->startUrl );

    QIcon icon = QWebSettings::iconForUrl( m_options->startUrl );
    //QIcon icon = m_page->mainFrame()->icon();
    qDebug() << "Is icon null: " << icon.isNull();
    if ( !icon.isNull() ) {
        setWindowIcon( icon );
    }
}

WebAppOptions *View::options() const
{
    return m_options;
}

QList<QAction *> View::actions() const
{
    return m_options->actions;
}

void View::setupApplication()
{
    m_options->startUrl = QUrl("http://mail.google.com/");
    m_options->windowTitle = QString("GMail");
    m_options->allowedBases.append( QUrl("http://mail.google.com/") );

    // Setup script actions
    QAction *action;

    action = new QAction(this);
    action->setText( QString("Compose Mail") );
    m_mapper->setMapping( action, QString("window.location = 'http://mail.google.com/mail/?view=cm&fs=1&tf=1'") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );

    m_options->actions.append( action );

    save( QString("gmail.ini") ); // HACK
}

bool View::load( const QString &filename )
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

bool View::save( const QString &filename )
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

void View::actionTriggered( const QString &script )
{
    qDebug() << "actionTriggered: " << script;

    page()->mainFrame()->evaluateJavaScript( script );
}

void View::iconLoaded()
{
    qDebug() << "Got icon";
    qDebug() << m_page->mainFrame()->icon().isNull();
    setWindowIcon( m_page->mainFrame()->icon() );

//    QIcon icon = QWebSettings::iconForUrl( QUrl("http://mail.google.com/") );
//    qDebug() << icon;
}
