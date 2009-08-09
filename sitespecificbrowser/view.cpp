/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 * Copyright 2009 Omat Holding B.V. <info@omat.nl>
 * Copyright 2009 Sebastian Kügler <sebas@kde.org>
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

#include <qdebug.h>
#include <qwebsettings.h>
#include <qwebframe.h>
#include <qsettings.h>
#include <qsignalmapper.h>
#include <qdir.h>
#include <qprogressbar.h>
#include <qtimer.h>

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

    m_progressTimer = new QTimer( this );
    m_progressTimer->setInterval( 500 );
    m_progressTimer->setSingleShot( true );

    m_progressBar = new QProgressBar( this );
    m_progressBar->show(); // let's show it at startup.

    connect( this, SIGNAL( loadStarted() ), m_progressTimer, SLOT( start() ) );
    connect( this, SIGNAL( loadProgress( int ) ), m_progressBar, SLOT( setValue( int ) ) );
    connect( this, SIGNAL( loadFinished( bool ) ), m_progressBar, SLOT( hide() ) );
    connect( this, SIGNAL( loadFinished( bool ) ), m_progressTimer, SLOT( stop() ) );
    connect( m_progressTimer, SIGNAL( timeout() ), m_progressBar, SLOT( show() ) );
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
    kDebug() << "Searching for Actions ..." << m_options->name;
    foreach (KPluginInfo info, WebAppAction::listWebAppActions(m_options->name)) {
        kDebug() << "ACTION:" << info.name();
        WebAppAction *action = new WebAppAction(this);
        action->load(info);
        m_mapper->setMapping(action, action->options()->script);
        connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
        m_options->actions.append( action );

    }
    return true;
}
