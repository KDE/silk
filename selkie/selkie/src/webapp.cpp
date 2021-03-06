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
    return options()->name;
}

QIcon WebApp::icon()
{
    return options()->windowIcon;
}

void WebApp::startApplication()
{
    setWindowTitle( options()->windowTitle );
    m_widget->view()->setUrl( options()->startUrl );
    setWindowIcon(icon());

    QIcon icon = QWebSettings::iconForUrl( options()->startUrl );
    //kDebug() << "Is icon null: " << icon.isNull();
    if (!icon.isNull()) {
        setWindowIcon(icon);
    }
}

bool WebApp::finishLoading(WebAppOptions myoptions)
{
    options()->name = myoptions.name;
    options()->windowIcon = myoptions.windowIcon;
    options()->windowTitle = myoptions.windowTitle;
    options()->packageRoot = myoptions.packageRoot;
    QUrl startUrl = myoptions.startUrl;

    //kDebug() << startUrl;
    QString dataUrl = "silk/webapps/" + options()->name + "/";
    if (startUrl.isRelative()) {
        QString startFile = dataUrl + startUrl.toString();
        //kDebug() << "StartUrl is relative, search KStandardDirs for" << dataUrl << startUrl << startFile;
        QUrl url = KGlobal::dirs()->findResource("data", startFile);
        //kDebug() << "Found:" << url;
        //KGlobal::dirs()->findResource("data", startUrl);
        options()->startUrl = url;
    } else {
        options()->startUrl = QUrl(startUrl);
    }
    foreach (const QString &url, myoptions.allowedBasesRaw) {
        if (QUrl(url).isRelative()) {
            QStringList u = KGlobal::dirs()->findDirs("data", dataUrl + url);
            foreach (const QString &allowedUrl, u) {
                // We need to append file:// as protocol, otherwise the
                // parent matching with allowed bases won't work
                options()->allowedBases << QUrl("file://" + allowedUrl);
            }
        } else {
            options()->allowedBases << QUrl(url);
        }
    }
    options()->styleSheets = myoptions.styleSheets;
    //kDebug() << "Stylesheets: ++" << options()->styleSheets;
    //kDebug() << "AllowedBases:" << options()->allowedBases;

    if (myoptions.comment.isEmpty()) {
        options()->comment = i18n("No description available");
    } else {
        options()->comment = myoptions.comment;
    }

    if (loadWebAppActions()) {
        kDebug() << "some action failed to load";
    }
    return true;
}

bool WebApp::loadWebAppFromPackage(const QString &path)
{
    kDebug() << "loading path" << path;
    Package p(path);
    //p.show();
    kDebug() << "Package imported to" << p.root();
    QUrl root = p.root();
    QString pluginFile = root.path() + "metadata.desktop";

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

    dump(options);
    return finishLoading(options);
}

bool WebApp::loadInstalledWebApp(const QString &name)
{
    QString p = Package::findPackage(name);
    if (p.isEmpty()) {
        return false;
    }

    m_package = new Package(name);
    // already validated, as listPackage only returns valid packages
    //output(QString("  %1\t%2 (%3)").arg(package, p.metadata()->name, p.metadata()->comment));
    WebAppOptions options;
    options.name = m_package->metadata()->pluginName;
    options.comment = m_package->metadata()->comment;
    options.windowIcon = KIcon(m_package->metadata()->icon);
    options.windowTitle = m_package->metadata()->name;
    options.packageRoot = m_package->metadata()->root;
    options.startUrl = m_package->metadata()->startUrl;
    foreach (const QString &base, m_package->metadata()->allowedBases) {
        options.allowedBases << base;
        options.allowedBasesRaw << base;
    }
    foreach (const QString &css, m_package->metadata()->styleSheets) {
        options.styleSheets << css;
    }
    //options.styleSheets = m_package->metadata()->styleSheets;
    dump(options);
    finishLoading(options);
    return true;
}

bool WebApp::loadWebAppActions()
{
    // FIXME: Port to dirlisting mechanism
    //kDebug() << "Searching for Actions ..." << m_options->name;
    bool failed = false;
    foreach (const QString &a, WebAppAction::listWebAppActions(options()->packageRoot)) {
        kDebug() << "NEW ACTION:" << a;
        WebAppAction *action = new WebAppAction(this);
        action->setPackageRoot(options()->packageRoot.path());
        action->load(a);
        if (!(m_widget->view()->addAction(action))) {
            kDebug() << "Loading failed:" << a;
            failed = true;
        }
    }
    return failed;
}

WebAppOptions* WebApp::options()
{
    return m_widget->view()->options();
}

void WebApp::dump(const WebAppOptions options)
{
    kDebug() << "//------------- WebApp --------------";
    kDebug() << "plugin" << options.name;
    kDebug() << "title" << options.windowTitle;
    kDebug() << "icon" << options.windowIcon;
    kDebug() << "comment" << options.comment;
    kDebug() << "startUrl" << options.startUrl;
    kDebug() << "allowedBases" << options.allowedBases;
    kDebug() << "allowedBasesRaw" << options.allowedBasesRaw;
    kDebug() << "styleSheets" << options.styleSheets;
    kDebug() << "packageRoot" << options.packageRoot;
    kDebug() << "//------------- /WebApp --------------";
}

#include "webapp.moc"
