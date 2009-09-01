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
#include <qprintpreviewdialog.h>
#include <qtimer.h>

#include <KActionCollection>
#include <KDebug>
#include <KIcon>
#include <KMainWindow>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KToolBar>
#include <kio/accessmanager.h>

#include "page.h"
#include "view.h"
#include "webapp.h"
#include "scriptapi.h"

View::View( QWidget *parent )
    : QWebView(parent)
{
    m_mapper = new QSignalMapper( this );
    connect( m_mapper, SIGNAL( mapped(const QString &) ), SLOT(evaluateScript(const QString &)) );

    m_options = new WebAppOptions;
    m_actionCollection = new KActionCollection(this);

    m_page = new Page( this );
    connect( m_page->mainFrame(), SIGNAL( iconChanged() ), SLOT( iconLoaded() ) );
    connect( m_page, SIGNAL( urlChanged() ), this, SLOT( resetToolbarActions() ) );
    setPage( m_page );

    m_scriptapi = new ScriptApi(this);
    m_scriptapi->setWebView(this);

    QWebSettings::globalSettings()->setAttribute( QWebSettings::PluginsEnabled, true );
    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );
    QWebSettings::setIconDatabasePath( QDir::currentPath() );

#ifndef NO_KIO
    KIO::AccessManager *access = new KIO::AccessManager( this );
    m_page->setNetworkAccessManager(access);
#endif

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

    connect( this->page(), SIGNAL( printRequested ( QWebFrame*) ),
             SLOT( slotPrint( QWebFrame* ) ) );
}

View::~View()
{
    delete m_options;
}

void View::slotPrint( QWebFrame* frame )
{
    QPointer<QPrintPreviewDialog> dlg = new QPrintPreviewDialog( this );
    connect( dlg, SIGNAL( paintRequested( QPrinter * ) ),
             frame, SLOT( print( QPrinter * ) ) );
    dlg->exec();
    delete dlg;
}

WebAppOptions *View::options() const
{
    return m_options;
}

QList<QAction *> View::actions() const
{
    return m_options->actions;
}

QString View::name() const
{
    return m_options->name;
}

QString View::plugin() const
{
    return m_options->plugin;
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
}

bool View::loadWebAppActions(WebApp *parent)
{
    kDebug() << "Searching for Actions ..." << m_options->name;
    foreach (KPluginInfo info, WebAppAction::listWebAppActions(m_options->name)) {
        kDebug() << "New Action:" << info.name();
        WebAppAction *action = new WebAppAction(parent);
        action->load(info);
        m_mapper->setMapping(action, action->options()->script);
        connect( action, SIGNAL(triggered()), m_mapper, SLOT(map()) );
        m_options->actions.append( action );
        if (m_actionCollection) {
            m_actionCollection->addAction(action->name(), action);
        }
    }
    return true;
}

bool View::actionShown(WebAppAction *action)
{
    QStringList urls = action->options()->showOnUrl;
    if (urls.isEmpty()) {
        return true;
    }
    //if (url().isEmpty() || wa_action->options()->showOnUrl.isEmpty()
    //    || m_page->url().toString().startsWith(wa_action->options()->showOnUrl)) {
    foreach(QString u, urls) {
        // Does the current URL start with the shown one?
        if (m_page->url().toString().startsWith(u)) {
            return true;
        }
    }
    kDebug() << "NOT SHOWING:" << urls << m_page->url();
    return false;
}

void View::resetToolbarActions()
{
    KMainWindow* win = static_cast<KMainWindow*>(parent());
    if (win) {
        win->toolBar()->clear();
    }
    foreach (QAction *action, m_actionCollection->actions()) {
        WebAppAction *wa_action = static_cast<WebAppAction*>(action);
        if (wa_action) {
            // This is a bit tricky since we don't know about the URL on startup. So if it's empty,
            // ignore this setting and just put them all in.
            // TODO: Will probably need some improvement.
            if (actionShown(wa_action)) {
                win->toolBar()->addAction(wa_action);
            }
        }
    }
}
