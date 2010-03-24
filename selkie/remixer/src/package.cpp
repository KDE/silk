/*
 * Package.cpp
 *
 * Copyright (C) 2010 Sebastian Kügler <sebas@kde.org>
 */

#include <QDir>
#include <QFile>
#include <KZip>

#include <kdebug.h>
//#include <kio/jobclasses.h>
#include <KConfigGroup>
#include <KDesktopFile>
//#include <KIO/Job>
//#include <KIO/ListJob>
#include <KTempDir>
#include <KStandardDirs>

#include "package.h"

Package::Package(QString path, QObject* parent)
    : QObject(parent)
{
    /*
    A package roughly looks like this:

    examplepackage/
    |-- actions
    |   |-- silk-webapp-silk-commitlog.desktop
    |   |-- silk-webapp-silk-sidebar.desktop
    |   |-- silk-webapp-silk-sourcetree.desktop
    |   `-- silk-webapp-silk-urltrigger.desktop
    |-- plugin.desktop
    |-- scripts
    |   `-- togglesidebar.js
    `-- webapp.desktop


    */
    //m_pluginName = QString("silk");
    KUrl _dir(path);

    if (path.isEmpty()) {
        kDebug() << "Empty package structure";
    } else if (path.endsWith(".selkie")) {
        kDebug() << "Package file:" << path;
        //QString unpackPath = "/tmp/unpacked/";

        KTempDir tmp;
        tmp.setAutoRemove(false);
        kDebug() << "TempDir:" << tmp.name();
        QString unpackPath = tmp.name();
        importPackage(path, unpackPath);
        m_root = KUrl(unpackPath);
        readDir();
    } else if (_dir.isValid()) {
        kDebug() << "Reading dir";
        m_root = _dir;
        readDir();
    }
}

Package::~Package()
{
}

void Package::show()
{
    kDebug() << "Package: " << m_root.path();
    kDebug() << "---------------------------";
    kDebug() << "Valid:   " << isValid();
    kDebug() << "Metadata:" << m_appFile << m_pluginFile;
    kDebug() << "Actions: " << m_actionFiles;
    kDebug() << "Scripts: " << m_scriptFiles;

}

KUrl Package::root()
{
    return m_root;
}

void Package::readDir()
{
    m_appPath = KStandardDirs::locateLocal("apps", "");
    m_pluginPath = KStandardDirs::locateLocal("services", "");


    if (m_root.isValid()) {
        QString rpath = m_root.path();
        if (!rpath.endsWith('/')) {
            rpath = rpath + '/';
        }
        m_appFile = rpath + "webapp.desktop";
        m_pluginFile = rpath + "plugin.desktop";
        m_pluginName = pluginName();
        m_dataPath = KStandardDirs::locateLocal("data", "silk/webapps/" + m_pluginName);

        //kDebug() << "Package path:" << rpath;
        QDir _root(rpath);
        QDir _actions(rpath + "actions/");
        m_actionFiles = _actions.entryList(QStringList("*.desktop"));

        QDir _scripts(rpath + "scripts/");
        m_scriptFiles = _scripts.entryList(QStringList("*.js"));
    }
}

void Package::ls(const QStringList &list)
{
    foreach (const QString &s, list) {
        kDebug() << " -- " << s;
    }
}

QString Package::pluginName()
{
    KDesktopFile desktopFile(m_pluginFile);
    KConfigGroup group = desktopFile.group("Desktop Entry");

    kDebug() << group.readEntry("Name", QString()) << group.readEntry("X-KDE-PluginInfo-Name", QString());
    QString _plugin = group.readEntry("X-KDE-PluginInfo-Name", QString());
    if (_plugin.isEmpty()) {
        kWarning() << "Could not read plugin name";
    }
    return _plugin;
}

bool Package::isValid()
{
    QString rpath = m_root.path();
    if (!rpath.endsWith('/')) {
        rpath = rpath + '/';
    }

    bool valid = true;
    QString error;

    if (!m_root.isValid()) {
        valid = false;
        error.append("m_root is invalid" + rpath + "\n");
    }
    if (!QFile(m_appFile).exists()) {
        valid = false;
        error.append("m_appFile is invalid" + m_appFile + "\n");
    }
    if (!QDir(rpath + "actions/").exists()) {
        valid = false;
        error.append("actions/ dir does not exist" + rpath + "actions/" + "\n");
    }

    if (m_pluginName.isEmpty()) {
        valid = false;
        error.append("Plugin name is empty. \n");
    }
    if (!error.isEmpty()) {
        kWarning() << "EE:" << error;
    }
    return valid;
}

void Package::install()
{
    /*
    bool KStandardDirs::makeDir (   const QString &     dir,
        int     mode = 0755
        )
    */
    //QStringList installationPath = KGlobal::dirs()->findDirs("apps", "Internet");
    //kDebug() << "Installation Path:" << installationPath;
    //kDebug() << "App Path:" << m_appPath;
    //kDebug() << "Plugin Path:" << m_pluginPath;
    //kDebug() << "Packageroot Path:" << m_dataPath;
    if (!isValid()) {
        kWarning() << "Selkie plasmoid invalid, not installing";
        return;
    }

    // Install app
    QString appdest = m_appPath + "silk-webapp-" + m_pluginName + ".desktop";
    if (install(m_appFile, appdest)) {
        //kDebug() << "appinstall Success.";
    } else {
        //return false;
    }

    // Install plugin
    QString plugindest = m_pluginPath + "silk/webapps/" + m_pluginName + \
                        "/silk-webapp-" + m_pluginName + ".desktop";
    kDebug() << "creating dir????" << m_pluginPath + "silk/webapps/" + m_pluginName;
    if (!QDir(m_pluginPath + "silk/webapps/" + m_pluginName).exists()) {
        //kDebug() << "creating dir" << m_pluginPath + "silk/webapps/" + m_pluginName;
        KStandardDirs::makeDir(m_pluginPath + "silk/webapps/" + m_pluginName);
    }
    if (install(m_pluginFile, plugindest)) {
        //kDebug() << "=========== :) plugininstall success." << plugindest;
    } else {
        //return false;
    }

    // Install actions
    //kDebug() << "installing actions" << m_actionFiles;
    QString actionPath = m_root.path() + "actions/";
    foreach (const QString &af, m_actionFiles) {
        //kDebug() << "installing plugin to " << m_pluginPath + af;
        install(actionPath + af, m_pluginPath + af);
    }

    // Install datafiles
    //kDebug() << "---> installing scripts" << m_scriptFiles;
    QString scriptPath = m_root.path() + "scripts/";
    foreach (const QString &af, m_scriptFiles) {
        if (!QDir(m_dataPath).exists()) {
            //kDebug() << "creating dir" << m_dataPath;
            KStandardDirs::makeDir(m_dataPath);
        }
        if (!install(scriptPath + af, m_dataPath + "/" + af)) {
            //kDebug() << "installation error" << scriptPath + af << m_dataPath + "/" + af;
        };
    }


    //return true;
}

bool Package::install(const QString &source, const QString &destination)
{
    //kDebug() << "Installing ... " << source << destination;
    return QFile::copy(source, destination);
}

bool Package::exportPackage(const KUrl &exportUrl, const KUrl &targetFile)
{
    kDebug() << "Exporting" << exportUrl << "to" << targetFile;
    bool ret = true;
    KZip selkiepackage(targetFile.path());
    if (!selkiepackage.open(QIODevice::WriteOnly)) {
        kWarning() << "Can't open file for writing" << targetFile.path();
        return false;
    }
    ret = selkiepackage.addLocalDirectory(exportUrl.path(), ".");
    selkiepackage.close();
    return ret;
}

bool Package::importPackage(const KUrl &importFile, const KUrl &targetUrl)
{
    kWarning() << "Importing" << importFile.path() << "to" << targetUrl.path();
    bool ret = true;
    KZip selkiepackage(importFile.path());
    if (!selkiepackage.open(QIODevice::ReadOnly)) {
        kWarning() << "Can't open file for reading" << importFile.path();
        return false;
    }
    selkiepackage.directory()->copyTo(targetUrl.path());
    selkiepackage.close();
    return ret;
}

#include "package.moc"
