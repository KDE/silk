/*
 * Copyright 2009-2010 Sebastian Kügler <sebas@kde.org>
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
#include "webappwidget.h"
#include "../../remixer/src/package.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QWebFrame>

#include <KAction>
#include <KActionCollection>
#include <KConfigDialog>
#include <KDebug>
#include <KDesktopFile>
#include <KDE/KLocale>
#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KTempDir>
#include <KToolBar>
#include <KStandardAction>
#include <KStandardDirs>
#include <KStatusBar>


WebApp::WebApp()
    : KMainWindow()
{
    setAcceptDrops(true);
    m_widget = new WebAppWidget(this);
    setCentralWidget(m_widget);
}

WebApp::~WebApp()
{
}

QString WebApp::name()
{
    return m_widget->view()->options()->name;
}

QIcon WebApp::icon()
{
    return m_widget->view()->options()->windowIcon;
}

void WebApp::startApplication()
{
    setWindowTitle( m_widget->view()->options()->windowTitle );
    m_widget->view()->setUrl( m_widget->view()->options()->startUrl );
    setWindowIcon(icon());

    QIcon icon = QWebSettings::iconForUrl( m_widget->view()->options()->startUrl );
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
        WebAppOptions options;
        kDebug() << "Silk/WebApp:" << name << info.author() << info.property("X-Silk-StartUrl") <<  info.property("X-Silk-StartUrl");
        //kDebug() << "Found plugin:" << name;
        options.comment = info.comment();
        options.name = info.pluginName();
        options.windowIcon = KIcon(info.icon());
        options.windowTitle = info.property("Name").toString();
        options.startUrl = QUrl(info.property("X-Silk-StartUrl").toString());
        foreach (const QString &url, info.property("X-Silk-AllowedBases").toStringList()) {
            options.allowedBasesRaw << url;
        }
        options.styleSheets = info.property("X-Silk-StyleSheets").toStringList();
        return finishLoading(options);
        //return true;
    }
    return false;
}

bool WebApp::finishLoading(WebAppOptions options)
{
    m_widget->view()->options()->name = options.name;
    m_widget->view()->options()->windowIcon = options.windowIcon;
    m_widget->view()->options()->windowTitle = options.windowTitle;
    QUrl startUrl = options.startUrl;

    //kDebug() << startUrl;
    QString dataUrl = "silk/webapps/" + options.name + "/";
    if (startUrl.isRelative()) {
        QString startFile = dataUrl + startUrl.toString();
        //kDebug() << "StartUrl is relative, search KStandardDirs for" << dataUrl << startUrl << startFile;
        QUrl url = KGlobal::dirs()->findResource("data", startFile);
        //kDebug() << "Found:" << url;
        //KGlobal::dirs()->findResource("data", startUrl);
        m_widget->view()->options()->startUrl = url;
    } else {
        m_widget->view()->options()->startUrl = QUrl(startUrl);
    }
    foreach (const QString &url, options.allowedBasesRaw) {
        if (QUrl(url).isRelative()) {
            QStringList u = KGlobal::dirs()->findDirs("data", dataUrl + url);
            foreach (const QString &allowedUrl, u) {
                // We need to append file:// as protocol, otherwise the
                // parent matching with allowed bases won't work
                m_widget->view()->options()->allowedBases << QUrl("file://" + allowedUrl);
            }
        } else {
            m_widget->view()->options()->allowedBases << QUrl(url);
        }
    }
    m_widget->view()->options()->styleSheets = options.styleSheets;
    kDebug() << "Stylesheets: ++" << m_widget->view()->options()->styleSheets;

    //kDebug() << "AllowedBases:" << m_widget->view()->options()->allowedBases;

    if (options.comment.isEmpty()) {
        m_widget->view()->options()->comment = i18n("No description available");
    } else {
        m_widget->view()->options()->comment = options.comment;
    }


    m_widget->view()->loadWebAppActions(this);
    return true;
    

    //return false;
}

bool WebApp::loadWebAppFromPackage(const QString &path)
{
    kDebug() << "loading path" << path;
/*
    KTempDir tmp;
    tmp.setAutoRemove(false);
    kDebug() << "TempDir:" << tmp.name();
*/
    Package p(path);
    //p.show();
    kDebug() << "Package imported to" << p.root();
    QUrl root = p.root();
    QString pluginFile = root.path() + "plugin.desktop";

    kDebug() << "Plugin File:" << pluginFile;
    KDesktopFile* dfile = new KDesktopFile(pluginFile);
    KConfigGroup cfg = dfile->desktopGroup();
    if (!cfg.isValid()) {
        kWarning() << "EE: Invalid KConfigGroup in plugin .desktop file.";
        return false;
    }
    WebAppOptions options;
    options.name = cfg.readEntry("X-KDE-PluginInfo-Name", QString());
    options.comment = cfg.readEntry("Comment", QString());
    options.windowIcon = KIcon(cfg.readEntry("Icon", QString("edit-delete-page")));
    options.windowTitle = cfg.readEntry("Name", QString());
    options.startUrl = cfg.readEntry("X-Silk-StartUrl", QString());
    options.packageRoot = p.root();
    foreach (const QString &url, cfg.readEntry("X-Silk-AllowedBases", QStringList())) {
        options.allowedBasesRaw << url;
    }
    options.styleSheets = cfg.readEntry("X-Silk-StyleSheets", QStringList());
    kDebug() << "//------------- WebApp --------------";
    kDebug() << "plugin" << options.name;
    kDebug() << "title" << options.windowTitle;
    kDebug() << "icon" << options.windowIcon;
    kDebug() << "comment" << options.comment;
    kDebug() << "startUrl" << options.startUrl;
    kDebug() << "packageRoot" << options.packageRoot;
    kDebug() << "//------------- /WebApp --------------";

    return finishLoading(options);

    //importPackage(const KUrl &importFile, const KUrl &targetUrl);

    //Package::importPackage(KUrl(path), KUrl(tmp.name()));

    return false;

    return true;

}
#include "webapp.moc"
