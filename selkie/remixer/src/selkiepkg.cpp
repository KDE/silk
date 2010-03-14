/*
 * Copyright 2008 Aaron Seigo <aseigo@kde.org>
 * Copyright 2010 Sebastian Kügler <sebas@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include <QDir>
#include <QDBusInterface>

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
#include <KLocale>
#include <KService>
#include <KServiceTypeTrader>
#include <KShell>
#include <KStandardDirs>

#include "package.h"
//#include <Plasma/PackageStructure>
//#include <Plasma/Package>
//#include <Plasma/PackageMetadata>

static const char description[] = I18N_NOOP("Install, list, remove Selkie WebApp packages");
static const char version[] = "0.1";

void output(const QString &msg)
{
    std::cout << msg.toLocal8Bit().constData() << std::endl;
}

void runKbuildsycoca()
{
    QDBusInterface dbus("org.kde.kded", "/kbuildsycoca", "org.kde.kbuildsycoca");
    dbus.call(QDBus::NoBlock, "recreate");
}

QStringList packages(const QStringList& types)
{
    QStringList result;
    foreach (const QString& type, types) {
        KService::List services = KServiceTypeTrader::self()->query("Silk/" + type);
        foreach(const KService::Ptr &service, services) {
            result << service->property("X-KDE-PluginInfo-Name", QVariant::String).toString();
        }
    }
    return result;
}

void listPackages(const QStringList& types)
{
    QStringList list = packages(types);
    list.sort();
    foreach(const QString& package, list) {
        output(package);
    }
}

void showPackage(const QString& package)
{
    Package p(0, package);
    p.show();
}

void installPackage(const QString& package)
{
    Package p(0, package);
    p.show();
    p.install();
    runKbuildsycoca();
}

void exportPackage(const QString& package, const QString &exportFile)
{
    Package p(0, package);
    p.show();
    if (p.isValid()) {
        Package::exportPackage(KUrl(package), KUrl(exportFile));
        kDebug() << "File exported:" << exportFile;
    } else {
        kDebug() << "package invalid" << package;
    }
    //p.install();
    //runKbuildsycoca();
}

int main(int argc, char **argv)
{
    KAboutData aboutData("selkiepkg", 0, ki18n("Selkie Package Manager"),
                         version, ki18n(description), KAboutData::License_GPL,
                         ki18n("(C) 2010, Sebastian Kügler"));
    aboutData.addAuthor( ki18n("Sebastian Kügler"),
                         ki18n("Original author"),
                        "sebas@kde.org" );

    KComponentData componentData(aboutData);

    KCmdLineArgs::init( argc, argv, &aboutData );

    KCmdLineOptions options;
    options.add("g");
    options.add("global", ki18n("For install or remove, operates on packages installed for all users."));
    options.add("i");
    options.add("install <path>", ki18nc("Do not translate <path>", "Install the package at <path>"));
    options.add("s");
    options.add("show <path>", ki18nc("Do not translate <path>", "Install the package at <path>"));
    options.add("u");
    options.add("upgrade <path>", ki18nc("Do not translate <path>", "Upgrade the package at <path>"));
    options.add("p");
    options.add("package <path>", ki18nc("Do not translate <path>", "Create a package at <path>"));
    options.add("l");
    options.add("list", ki18n("List installed packages"));
    options.add("r");
    options.add("remove <name>", ki18nc("Do not translate <name>", "Remove the package named <name>"));
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    QString servicePrefix;
    QStringList pluginTypes;
    //Plasma::PackageStructure *installer = 0;
    QString package;
    QString exportFile;
    QString packageFile;

    if (args->isSet("remove")) {
        package = args->getOption("remove");
    } else if (args->isSet("show")) {
        package = args->getOption("show");
    } else if (args->isSet("upgrade")) {
        package = args->getOption("upgrade");
    } else if (args->isSet("package")) {
        package = args->getOption("install");
        exportFile = args->getOption("package");
    } else if (args->isSet("install")) {
        package = args->getOption("install");
        installPackage(package);
    }
    if (!QDir::isAbsolutePath(package)) {
        packageFile = QDir(QDir::currentPath() + '/' + package).absolutePath();
    } else {
        packageFile = package;
    }
    pluginTypes << "WebApp";
    //pluginTypes << "WebApp/Action";


    if (args->isSet("list")) {
        listPackages(pluginTypes);
    } else if (args->isSet("show")) {
        showPackage(packageFile);
    } else if (args->isSet("package"))  {
        exportPackage(packageFile, exportFile);
    } else if (args->isSet("install")) {
        installPackage(packageFile);
    } else if (args->isSet("upgrade")) {
        installPackage(packageFile);
    } else {
        runKbuildsycoca();
    }
    //delete installer;
    return 0;
}

