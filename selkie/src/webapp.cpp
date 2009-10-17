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

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>

#include <KAction>
#include <KActionCollection>
#include <KConfigDialog>
#include <KDebug>
#include <KDE/KLocale>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KToolBar>
#include <KStandardAction>
#include <KStandardDirs>
#include <KStatusBar>


WebApp::WebApp()
    : KMainWindow(),
      m_view(new View(this))
{
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

QIcon WebApp::icon()
{
    return m_view->options()->windowIcon;
}

void WebApp::startApplication()
{
    setWindowTitle( m_view->options()->windowTitle );
    m_view->setUrl( m_view->options()->startUrl );
    setWindowIcon(icon());

    QIcon icon = QWebSettings::iconForUrl( m_view->options()->startUrl );
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

        kDebug() << "Silk/WebApp:" << name << info.author() << info.property("X-Silk-StartUrl") <<  info.property("X-Silk-StartUrl");
        kDebug() << "Found plugin:" << name;
        m_view->options()->name = info.pluginName();
        m_view->options()->windowIcon = KIcon(info.icon());
        m_view->options()->windowTitle = info.property("Name").toString();

        QUrl startUrl = QUrl(info.property("X-Silk-StartUrl").toString());
        kDebug() << startUrl;
        QString dataUrl = "silk-webapp/" + info.pluginName() + "/";
        if (startUrl.isRelative()) {
            QString startFile = dataUrl + startUrl.toString();
            kDebug() << "StartUrl is relative, search KStandardDirs for" << dataUrl << startUrl << startFile;
            QUrl url = KGlobal::dirs()->findResource("data", startFile);
            kDebug() << "Found:" << url;
            //KGlobal::dirs()->findResource("data", startUrl);
            m_view->options()->startUrl = url;
        } else {
            m_view->options()->startUrl = QUrl(info.property("X-Silk-StartUrl").toString());

        }
        foreach (const QString &url, info.property("X-Silk-AllowedBases").toStringList()) {
            if (QUrl(url).isRelative()) {
                QStringList u = KGlobal::dirs()->findDirs("data", dataUrl + url);
                foreach (const QString &allowedUrl, u) {
                    // We need to append file:// as protocol, otherwise the
                    // parent matching with allowed bases won't work
                    m_view->options()->allowedBases << QUrl("file://" + allowedUrl);
                }
            } else {
                m_view->options()->allowedBases << QUrl(url);
            }
        }
        m_view->options()->styleSheets = info.property("X-Silk-StyleSheets").toStringList();
        kDebug() << "Stylesheets: ++" << m_view->options()->styleSheets;

        kDebug() << "AllowedBases:" << m_view->options()->allowedBases;
        QString comment = info.comment();

        if (comment.isEmpty()) {
            comment = i18n("No description available");
        }


        m_view->loadWebAppActions(this);
        return true;
    }

    return false;
}

#include "webapp.moc"
