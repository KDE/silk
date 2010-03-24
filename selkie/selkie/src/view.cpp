/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 * Copyright 2009 Omat Holding B.V. <info@omat.nl>
 * Copyright 2009 Sebastian K?gler <sebas@kde.org>
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
#include <QGraphicsWebView>
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
#include <KGlobal>
#include <KIcon>
#include <KMainWindow>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KStandardDirs>
#include <KToolBar>
#include <kio/accessmanager.h>

#include "page.h"
#include "view.h"
#include "webapp.h"
#include "scriptapi.h"

View::View( KMainWindow *win, QGraphicsItem *parent)
    : QGraphicsWebView(parent),
    m_win(win)
{
    m_showProgressBar = false;
    m_mapper = new QSignalMapper( this );
    connect( m_mapper, SIGNAL( mapped(const QString &) ), SLOT(evaluateScript(const QString &)) );

    m_options = new WebAppOptions;
    m_actionCollection = new KActionCollection(this);

    m_page = new Page( this );
    connect( m_page->mainFrame(), SIGNAL( iconChanged() ), SLOT( iconLoaded() ) );
    connect( this, SIGNAL( urlChanged(const QUrl &) ), this, SLOT( updateActions() ) );
    connect( this, SIGNAL( loadFinished(bool) ), this, SLOT( loadStyleSheets() ) );
    connect( this, SIGNAL( loadFinished(bool) ), this, SLOT( triggerUrlActions() ) );
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

    connect( this, SIGNAL( loadStarted() ), m_progressTimer, SLOT( start() ) );
    connect( this, SIGNAL( loadProgress( int ) ), this, SLOT( updateProgress( int ) ) );
    connect( this, SIGNAL( loadFinished(bool) ), m_progressTimer, SLOT( stop() ) );

    connect( m_progressTimer, SIGNAL( timeout() ), this, SLOT( toggleProgressBar() ) );
    connect( this, SIGNAL( loadFinished(bool) ), this, SLOT( startTimer() ) );

    connect( this->page(), SIGNAL( printRequested ( QWebFrame*) ),
             SLOT( slotPrint( QWebFrame* ) ) );
}

View::~View()
{
    delete m_options;
}

void View::updateProgress(int progress)
{
    m_progress = (qreal)(progress / 100.0);
}

void View::slotPrint( QWebFrame* frame )
{
    Q_UNUSED( frame )
    //FIXME: should probably move to the mainwindow
    /*
    QPrintPreviewDialog dlg( 0 );
    connect( &dlg, SIGNAL( paintRequested( QPrinter * ) ),
             frame, SLOT( print( QPrinter * ) ) );
    dlg.exec();
    */
}

void View::showProgressBar()
{
    kDebug() << "hiding";
    m_showProgressBar = true;
    update();
}

void View::hideProgressBar()
{
    kDebug() << "hiding";
    m_showProgressBar = false;
    update();
}

void View::toggleProgressBar()
{
    kDebug() << "toggle to " << !m_showProgressBar;
    m_showProgressBar = !m_showProgressBar;
    update();
}

void View::startTimer()
{
    kDebug() << "Starting timer for two seconds";
    m_progress = 1;
    m_progressTimer->setInterval( 2000 );
    m_progressTimer->setSingleShot( true );
    m_progressTimer->start();
    update();
    //connect( m_progressTimer, SIGNAL( timeout() ), this, SLOT( toggleProgressBar() ) );
}

void View::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 )
{
    QGraphicsWebView::paint(painter, option, widget);
    if (!m_showProgressBar) {
        return;
    }
    //kDebug() << m_progress;
    painter->save();
    QColor b = QColor("#223F5B");
    b.setAlphaF(1-(.5*m_progress));

    //qreal o = .5;
    //painter->setOpacity(o);
    painter->setBrush(b);
    painter->translate(.5, .5);

    QColor p("#FFFFFF");
    p.setAlphaF(.4);
    QPen pen(p);
    pen.setWidth(1);
    painter->setPen(p);
    int margin = 2;
    int w = (geometry().width()-(margin*2)) * m_progress;
    int h = 8;
    QRectF progress_rect(margin, margin, w, h);
    painter->drawRoundedRect(progress_rect, margin, margin);
    painter->restore();
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
    //kDebug() << script;
    m_scriptapi->setTrusted( true );
    page()->mainFrame()->evaluateJavaScript( script );
    m_scriptapi->setTrusted( false );
}

void View::loadStyleSheets()
{
    // FIXME: loading CSS doesn't work yet as I don't really know how to effectively set a
    // stylesheet, probably setUserStyle... or something in webkit
    foreach (const QString &css, m_options->styleSheets) {
        QString scriptfile = "silk-webapp/" + m_options->name + "/data/" + css;
        scriptfile = KGlobal::dirs()->findResource("data", scriptfile);
        kDebug() << " ___ Found CSS:" << scriptfile << css;

        m_scriptapi->setTrusted( true );
        m_scriptapi->loadStyleSheet(scriptfile);
        m_scriptapi->setTrusted( false );
    }
}

void View::iconLoaded()
{
    //kDebug() << "Got icon";
    kDebug() << m_page->mainFrame()->icon().isNull();
    //FIXME: setWindowIcon( m_page->mainFrame()->icon() );
}

bool View::addAction(WebAppAction *action)
{
    //kDebug() << "Searching for Actions ..." << m_options->name;
    //kDebug() << "New Action:" << info.name();
    m_mapper->setMapping(action, action->options()->script);
    connect(action, SIGNAL(triggered()), m_mapper, SLOT(map()));
    m_options->actions.append( action );
    if (m_actionCollection) {
        m_actionCollection->addAction(action->name(), action);
    }
    return true;
}

bool View::match(QStringList wildcards, QStringList urls)
{
    if (!wildcards.isEmpty()) {
        //kDebug() << "+++++++ Wildcards" << wildcards;
        foreach(QString w, wildcards) {
            // Pattern matching
            bool inverted = false;
            if (w.startsWith('!')) {
                w.remove(0, 1);
                inverted = true;
                //kDebug() << "inverting wildcard" << w;
            }
            QRegExp rx(w);
            rx.setPatternSyntax(QRegExp::Wildcard);
            if (!inverted && rx.exactMatch(url().toString())) {
                //kDebug() << "showing (match) ..." << wildcards << url() << !inverted;
                return true;
            } else if (inverted && !rx.exactMatch(url().toString())) {
                //kDebug() << "showing (no match, inverted) ..." << wildcards << url() << !inverted;
                return true;
            } else {
                //kDebug() << "wildcard" << w << "not matching";
            }
        }
    }

    // No pattern matched, let's see if we match a URL
    if (urls.isEmpty() && !wildcards.isEmpty()) {
        // neither wildcard nor url is specified. Let's show the action
        // as there are no restrictions specified
        //kDebug() << "not showing:" << wildcards << url();
        return false;
    } else if (urls.isEmpty()) {
        // no wildcards, no URLs to pay attention to
        return false;
    }

    foreach(QString u, urls) {
        // Does the current URL start with the shown one?
        if (url().toString().startsWith(u)) {
            return true;
        }
    }

    // No URL matched, no wildcard matched, but filters do apply
    return false;
}

void View::updateActions()
{
    resetToolbarActions();
    //triggerUrlActions();
}

void View::resetToolbarActions()
{
    //KMainWindow* win = qobject_cast<KMainWindow*>(parent());
    if (!m_win) {
        kWarning() << "Our parent is not a KMainWindow, be afraid";
        return;
    }

    m_win->toolBar()->clear();
    foreach (QAction *action, m_actionCollection->actions()) {
        WebAppAction *wa_action = qobject_cast<WebAppAction*>(action);
        if (wa_action) {
            if (match(wa_action->options()->showOnWildcard, wa_action->options()->showOnUrl)) {
                //kDebug() << "Showing" << wa_action->options()->name;
                m_win->toolBar()->addAction(wa_action);
            }
        }
    }
}

void View::triggerUrlActions()
{
    QAction *action;
    foreach (action, actions()) {
        WebAppAction *wa = qobject_cast<WebAppAction*>(action);
        if (wa) {
            if (match(wa->options()->triggerOnWildcard, wa->options()->triggerOnUrl)) {
                kDebug() << "Triggering" << wa->options()->name;
                wa->trigger();
            }
        }
    }
}
