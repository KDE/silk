#include <qdebug.h>
#include <qwebsettings.h>
#include <qwebframe.h>
#include <qsettings.h>
#include <qsignalmapper.h>
#include <qdir.h>

#include <KDebug>
#include <KIcon>
#include <KPluginInfo>
#include <KServiceTypeTrader>

#include "page.h"
#include "view.h"
#include "scriptapi.h"

View::View( QWidget *parent )
    : QWebView(parent)
{
    m_mapper = new QSignalMapper( this );
    connect( m_mapper, SIGNAL( mapped(const QString &) ), SLOT(evaluateScript(const QString &)) );

    m_options = new WebAppOptions;

    m_page = new Page( this );
    connect( m_page->mainFrame(), SIGNAL( iconChanged() ), SLOT( iconLoaded() ) );
    setPage( m_page );

    m_scriptapi = new ScriptApi(this);
    m_scriptapi->setWebView(this);

    QWebSettings::globalSettings()->setAttribute( QWebSettings::PluginsEnabled, true );
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );
    QWebSettings::setIconDatabasePath( QDir::currentPath() );
}

WebAppOptions *View::options() const
{
    return m_options;
}

QList<QAction *> View::actions() const
{
    return m_options->actions;
}

void View::startApplication()
{
    setWindowTitle( m_options->windowTitle );
    setUrl( m_options->startUrl );

    QIcon icon = QWebSettings::iconForUrl( m_options->startUrl );
    //QIcon icon = m_page->mainFrame()->icon();
    kDebug() << "Is icon null: " << icon.isNull();
    if ( !icon.isNull() ) {
        setWindowIcon( icon );
    }
}

void View::setupApplication()
{
#if 0 // Hard coded app
    m_options->startUrl = QUrl("http://mail.google.com/");
    m_options->windowTitle = QString("GMail");
    m_options->allowedBases.append( QUrl("http://mail.google.com/") );
#endif

    // Setup script actions
    QAction *action;

    action = new QAction(this);
    action->setText( QString("Compose Mail") );
    action->setIcon( KIcon("mail-message-new") );
    m_mapper->setMapping( action, QString("window.location = 'http://mail.google.com/mail/?view=cm&fs=1&tf=1'") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );

    action = new QAction(this);
    action->setText( QString("Say Hello") );
    m_mapper->setMapping( action, QString("window.silk.GM_log('Hello!')") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );

    action = new QAction(this);
    action->setText( QString("Notify") );
    m_mapper->setMapping( action, QString("silk.showNotification('Pay Attention!')") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );

/* doesn't work
    action = new QAction(this);
    action->setText( QString("Hide GBar") );
    m_mapper->setMapping( action, QString("document.getElementById('gbar').style.display = 'none'") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );
*/
}

void View::evaluateScript( const QString &script )
{
    kDebug() << script;
    m_scriptapi->setTrusted( true );
    page()->mainFrame()->evaluateJavaScript( script );
    m_scriptapi->setTrusted( false );
}

void View::iconLoaded()
{
    kDebug() << "Got icon";
    kDebug() << m_page->mainFrame()->icon().isNull();
    setWindowIcon( m_page->mainFrame()->icon() );

//    QIcon icon = QWebSettings::iconForUrl( QUrl("http://mail.google.com/") );
//    kDebug() << icon;
}

KPluginInfo::List View::listWebApps(const QString &name)
{
    QString constraint;
    if (!name.isEmpty()) {
        constraint.append(QString("[X-KDE-PluginInfo-Name] == '%1'").arg(name));
    }
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebApp", constraint);
    return KPluginInfo::fromServices(offers);
}

bool View::loadWebApp(const QString &name)
{
    foreach (const KPluginInfo &info, listWebApps(name)) {
        QString comment = info.comment();

        if (comment.isEmpty()) {
            comment = i18n("No description available");
        }

        kDebug() << "Silk/WebApp:" << name << comment << info.author() << info.property("X-Silk-StartUrl") <<  info.property("X-Silk-StartUrl");
        kDebug() << "Found plugin:" << name;
        m_options->name = info.pluginName();
        m_options->startUrl = QUrl(info.property("X-Silk-StartUrl").toString());

        foreach (const QString &url, info.property("X-Silk-AllowedBases").toStringList()) {
            m_options->allowedBases << QUrl(url);
        }
        m_options->windowIcon = KIcon(info.icon());
        m_options->windowTitle = info.property("Name").toString();

        loadWebAppActions();
        return true;
    }

    return false;
}

bool View::loadWebAppActions()
{
    foreach (KPluginInfo info, WebAppAction::listWebAppActions(m_options->name))
    {
        kDebug() << "ACTION:" << info.name();
    }
    return true;
}