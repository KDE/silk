/*
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

#include "webapp.h"
#include "view.h"
#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>

#include <KActionCollection>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KToolBar>
#include <kconfigdialog.h>
#include <kstatusbar.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KDE/KLocale>

WebApp::WebApp()
    : KMainWindow(),
      m_view(new View(this))
{
    m_actionCollection = new KActionCollection(this);
    setAcceptDrops(true);
    setCentralWidget(m_view);
}

WebApp::~WebApp()
{
}

QString WebApp::name()
{
    return m_view->options()->name;
}

void WebApp::startApplication()
{
    setWindowTitle( m_view->options()->windowTitle );
    m_view->setUrl( m_view->options()->startUrl );

    QIcon icon = QWebSettings::iconForUrl( m_view->options()->startUrl );
    //QIcon icon = m_page->mainFrame()->icon();
    kDebug() << "Is icon null: " << icon.isNull();
    if ( !icon.isNull() ) {
        setWindowIcon( icon );
    }
}


KPluginInfo::List WebApp::listWebApps(const QString &name)
{
    QString constraint;
    if (!name.isEmpty()) {
        constraint.append(QString("[X-KDE-PluginInfo-Name] == '%1'").arg(name));
    }
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebApp", constraint);
    return KPluginInfo::fromServices(offers);
}

bool WebApp::loadWebApp(const QString &name)
{
    foreach (const KPluginInfo &info, listWebApps(name)) {
        QString comment = info.comment();

        if (comment.isEmpty()) {
            comment = i18n("No description available");
        }

        kDebug() << "Silk/WebApp:" << name << comment << info.author() << info.property("X-Silk-StartUrl") <<  info.property("X-Silk-StartUrl");
        kDebug() << "Found plugin:" << name;
        m_view->options()->name = info.pluginName();
        m_view->options()->startUrl = QUrl(info.property("X-Silk-StartUrl").toString());

        foreach (const QString &url, info.property("X-Silk-AllowedBases").toStringList()) {
            m_view->options()->allowedBases << QUrl(url);
        }
        m_view->options()->windowIcon = KIcon(info.icon());
        m_view->options()->windowTitle = info.property("Name").toString();

        m_view->loadWebAppActions(m_actionCollection, this);
        return true;
    }

    return false;
}

#include "webapp.moc"
