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
    qDebug() << "Is icon null: " << icon.isNull();
    if ( !icon.isNull() ) {
        setWindowIcon( icon );
    }
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
    action->setIcon( KIcon("mail-message-new") );
    m_mapper->setMapping( action, QString("window.location = 'http://mail.google.com/mail/?view=cm&fs=1&tf=1'") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );

/* doesn't work
    action = new QAction(this);
    action->setText( QString("Hide GBar") );
    m_mapper->setMapping( action, QString("document.getElementById('gbar').style.display = 'none'") );
    connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
    m_options->actions.append( action );
*/

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

KPluginInfo::List View::listWebApps()
{
    QString constraint;
    /*
    if (parentApp.isEmpty()) {
        constraint.append("(not exist [X-KDE-ParentApp] or [X-KDE-ParentApp] == '')");
    } else {
        constraint.append("[X-KDE-ParentApp] == '").append(parentApp).append("'");
    }
    //note: constraint guaranteed non-empty from here down

    if (category.isEmpty()) { //use all but the excluded categories
        KConfigGroup group(KGlobal::config(), "General");
        QStringList excluded = group.readEntry("ExcludeCategories", QStringList());
        foreach (const QString &category, excluded) {
            constraint.append(" and [X-KDE-PluginInfo-Category] != '").append(category).append("'");
        }
    } else { //specific category (this could be an excluded one - is that bad?)
        constraint.append(" and ");

        constraint.append("[X-KDE-PluginInfo-Category] == '").append(category).append("'");
        if (category == "Miscellaneous") {
            constraint.append(" or (not exist [X-KDE-PluginInfo-Category] or [X-KDE-PluginInfo-Category] == '')");
        }
    }
    */
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebApp");
/*
    //now we have to do some manual filtering because the constraint can't handle everything
    KConfigGroup constraintGroup(KGlobal::config(), "Constraints");
    foreach (const QString &key, constraintGroup.keyList()) {
        //kDebug() << "security constraint" << key;
        if (constraintGroup.readEntry(key, true)) {
            continue;
        }
        //ugh. a qlist of ksharedptr<kservice>
        QMutableListIterator<KService::Ptr> it(offers);
        while (it.hasNext()) {
            KService::Ptr p = it.next();
            QString prop = QString("X-Plasma-Requires-").append(key);
            QVariant req = p->property(prop, QVariant::String);
            //valid values: Required/Optional/Unused
            QString reqValue;
            if (req.isValid()) {
                reqValue = req.toString();
            } else {
                //TODO if it's a scripted language default to not-required because the bindings disable it
                //this isn't actually implemented in any bindings yet but should be possible for
                //anything but c++
            }

            if (!(reqValue == "Optional" || reqValue == "Unused")) {
            //if (reqValue == "Required") {
                it.remove();
            }
        }
    }
*/
    //kDebug() << "Applet::listAppletInfo constraint was '" << constraint
    //         << "' which got us " << offers.count() << " matches";
    return KPluginInfo::fromServices(offers);
}

bool View::loadWebApp(const QString &name)
{
    foreach (const KPluginInfo &info, listWebApps()) {
        QString comment = info.comment();

        if (comment.isEmpty()) {
            comment = i18n("No description available");
        }

        kDebug() << "Silk/WebApp:" << name << comment << info.author() << info.property("X-Silk-StartUrl") <<  info.property("X-Silk-StartUrl");
        if (info.pluginName() == name) {
            kDebug() << "Found plugin:" << name;
            m_options->startUrl = QUrl(info.property("X-Silk-StartUrl").toString());

            foreach (const QString &url, info.property("X-Silk-AllowedBases").toStringList()) {
                m_options->allowedBases << QUrl(url);
            }
            m_options->windowIcon = KIcon(info.icon());
            m_options->windowTitle = info.property("Name").toString();

	    return true;
        }
    }

    return false;
}

