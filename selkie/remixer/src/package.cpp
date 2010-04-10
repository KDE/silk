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
#include <QUrlInfo>

#include "package.h"

Package::Package(QString path, QObject* parent)
    : QObject(parent),
    m_metadata(new MetaData)
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
    //m_metadata->pluginName = QString("silk");
    QString installedPath = findPackage(path);
    if (!installedPath.isEmpty()) {
        m_root = installedPath;
        m_metadataFile = m_root.path() + "/" + "metadata.desktop";
        readMetadata();
        readDir();
        kDebug() << "installed, but valid?" << isValid();
        return;
    }

    QDir _d(path);
    if (_d.isRelative()) {
        path = QDir::currentPath() + "/" + path;
    }
    
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
        m_metadataFile = unpackPath + "/" + "metadata.desktop";
        readMetadata();
        readDir();
    } else if (_dir.isValid()) {
        //kDebug() << "Reading dir" << _dir;
        m_root = _dir;
        m_metadataFile = m_root.path() + "/" + "metadata.desktop";
        readMetadata();
        readDir();
    }
    //show();
}

Package::~Package()
{
    // TODO: kill tempdir if necessary
}

QStringList Package::listPackages()
{
    QStringList packages;
    QStringList packagePaths = KGlobal::dirs()->findDirs("data", "silk/webapps/");
    foreach (const QString &path, packagePaths) {
        QDir kstdpath(path);
        QStringList selkies = kstdpath.entryList( QDir::Files | QDir::Dirs);
        foreach (const QString &selkie, selkies) {
            QString _p = path + selkie;
            //kDebug() << "Check?" << _p << selkie;
            Package* package = new Package(_p);
            if (package->isValid()) {
                kDebug() << "*** Found Valid Package" << package->metadata()->name << _p;
                packages << selkie;
            } else {
                //kDebug() << "--- Invalid Package:" << _p;
            }
            delete package;
        }
    }
    return packages;
}

QString Package::findPackage(const QString &package)
{
    QStringList packagePaths = KGlobal::dirs()->findDirs("data", "silk/webapps/" + package);
    //foreach (const QString &_stddir, installationPaths) 
    kDebug() << "Found:" << packagePaths;
    if (packagePaths.count()) {
        return packagePaths[0];
    } else {
        return QString();
    }
}

void Package::show()
{
    kDebug() << "Package: " << m_root.path();
    kDebug() << "---------------------------";
    kDebug() << "Valid:   " << isValid();
    kDebug() << "Metadata:" << m_appFile << m_metadataFile;
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
    //m_pluginPath = KStandardDirs::locateLocal("services", "");

    //kDebug() << "Root:"
    if (m_root.isValid()) {
        QString rpath = m_root.path();
        if (!rpath.endsWith('/')) {
            rpath = rpath + '/';
        }
        m_appFile = rpath + "webapp.desktop";
        m_metadataFile = rpath + "metadata.desktop";
        m_metadata->pluginName = pluginName();
        m_dataPath = KStandardDirs::locateLocal("data", "silk/webapps/" + m_metadata->pluginName);

        //kDebug() << "Package path:" << rpath;
        QDir _root(rpath);
        QDir _actions(rpath + "actions/");
        m_actionFiles = _actions.entryList(QStringList("*.desktop"));

        QDir _scripts(rpath + "scripts/");
        m_scriptFiles = _scripts.entryList(QStringList("*.js"));

        QDir _data(rpath + "data/");
        m_dataFiles = _data.entryList(QStringList("*"));
    }
}

void Package::ls(const QStringList &list)
{
    foreach (const QString &s, list) {
        kDebug() << " -- " << s;
    }
}

void Package::readMetadata()
{
    //kDebug() << "Reading metadata:" << m_metadata->pluginName;
    KDesktopFile desktopFile(m_metadataFile);
    KConfigGroup group = desktopFile.group("Desktop Entry");

    //kDebug() << group.readEntry("Name", QString()) << group.readEntry("X-KDE-PluginInfo-Name", QString());
    //QString _plugin = group.readEntry("X-KDE-PluginInfo-Name", QString());

    /*

    X-KDE-PluginInfo-Author=Sebastian Kügler
    X-KDE-PluginInfo-Email=sebas@kde.org
    X-KDE-PluginInfo-Name=silk
    X-KDE-PluginInfo-Version=0.01
    X-KDE-PluginInfo-Website=http://kde.org
    X-KDE-PluginInfo-Category=Development
    X-KDE-PluginInfo-License=GPL

    X-Silk-StartUrl=http://gitorious.org/project-silk/
    X-Silk-AllowedBases=http://gitorious.org/,https://secure.gitorious.org/

    */

    m_metadata->pluginName = group.readEntry("X-KDE-PluginInfo-Name", QString());
    m_metadata->name = group.readEntry("Name", QString());
    m_metadata->comment = group.readEntry("Comment", QString());
    m_metadata->author = group.readEntry("X-KDE-PluginInfo-Author", QString());
    m_metadata->email = group.readEntry("X-KDE-PluginInfo-Email", QString());
    m_metadata->version = group.readEntry("X-KDE-PluginInfo-Version", QString());
    m_metadata->website = group.readEntry("X-KDE-PluginInfo-Website", QString());
    m_metadata->category = group.readEntry("X-KDE-PluginInfo-Category", QString());
    m_metadata->license = group.readEntry("X-KDE-PluginInfo-License", QString());
    m_metadata->startUrl = group.readEntry("X-Silk-StartUrl", QString());
    m_metadata->allowedBases = group.readEntry("X-Silk-StartAllowedBases", QStringList());

}

MetaData* Package::metadata()
{
    return m_metadata;
}

QString Package::pluginName()
{
    return m_metadata->pluginName;
    /*
    kDebug() << "Plugin name:" << m_metadata->pluginName;
    KDesktopFile desktopFile(m_metadataFile);
    KConfigGroup group = desktopFile.group("Desktop Entry");

    kDebug() << group.readEntry("Name", QString()) << group.readEntry("X-KDE-PluginInfo-Name", QString());
    QString _plugin = group.readEntry("X-KDE-PluginInfo-Name", QString());
    if (_plugin.isEmpty()) {
        kWarning() << "Could not read plugin name";
    }
    return _plugin;
    */
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

    if (m_metadata->pluginName.isEmpty()) {
        valid = false;
        error.append("Plugin name is empty. \n");
    }
    if (!error.isEmpty()) {
        //kWarning() << "EE:" << error;
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
    QStringList installationPaths = KGlobal::dirs()->findDirs("data", "");
    QString installationPath;
    /*
    foreach (const QString &_path, installationPaths) {
        QFile t(_path+"bla");
        if (t.isWritable()) {
            installationPath = _path;
            continue;
        } else {
            kDebug() << "nope ..." << t.fileName();
        }
    }
    */
    if (installationPaths.count()) {
        // TODO: we take the first, assuming it's the local one, need to check wether that
        // assumption is safe
        installationPath = installationPaths[0] + "silk/webapps/" + m_metadata->pluginName + "/" ;
    }
    if (installationPath.isEmpty()) {
        kDebug() << "Empty installation path. Bugger.";
        return;
    }
    if (!installationPath.endsWith('/')) {
        installationPath.append('/');
    }
    //kDebug() << "Installation Path:" << installationPath << installationPaths;
    kDebug() << "App Path:" << m_appPath;
    //kDebug() << "Plugin Path:" << m_pluginPath;
    kDebug() << "Packageroot Path:" << m_dataPath;
    if (!isValid()) {
        kWarning() << "Selkie plasmoid invalid, not installing";
        return;
    }
    // TODO: check wether it's already installed, possibly upgrade?


    if (!QDir(installationPath).exists()) {
        kDebug() << "creating data dir" << m_dataPath;
        KStandardDirs::makeDir(m_dataPath);
    }

    if (!QDir(m_appPath).exists()) {
        kDebug() << "creating app dir" << m_appPath;
        KStandardDirs::makeDir(m_appPath);
    }

    kDebug() << "============== Metadata:";
    // Install app
    QString appdest = m_appPath + "silk-webapp-" + m_metadata->pluginName + ".desktop";
    if (install(m_appFile, appdest) && install(m_appFile, installationPath + "webapp.desktop")) {
        kDebug() << "appinstall Success." << appdest;
    } else {
        //return false;
    }
    QString _src = m_root.path() + "/metadata.desktop";
    QString _dest = installationPath + "metadata.desktop";
    kDebug() << "src dest" << _src << _dest;
    if (!install(_src, _dest)) {
        kDebug() << "EE: Error installing metadata" << _src << _dest;
    }

    /*
    // Install plugin
    QString plugindest = m_appPath + "silk/webapps/" + m_metadata->pluginName + \
                        "/silk-webapp-" + m_metadata->pluginName + ".desktop";
    kDebug() << "creating dir????" << m_pluginPath + "silk/webapps/" + m_metadata->pluginName;
    KStandardDirs::makeDir(installationPath + "/actions/");
    if (!QDir(m_pluginPath + "silk/webapps/" + m_metadata->pluginName).exists()) {
        //kDebug() << "creating dir" << m_pluginPath + "silk/webapps/" + m_metadata->pluginName;
        // = /home/sebas/.kde4/share/kde4/services/silk/webapps/gmail
        //KStandardDirs::makeDir(m_pluginPath + "silk/webapps/" + m_metadata->pluginName);
    }
    
    //if (install(m_metadataFile, plugindest)) {
    //    kDebug() << "=========== :) plugininstall success." << plugindest;
    //} else {
        //return false;
    //}
    */
    kDebug() << "============== Actions:";
    // Install actions
    KStandardDirs::makeDir(installationPath + "actions/");
    //kDebug() << "installing actions" << m_actionFiles;
    QString actionPath = m_root.path() + "/actions/";
    foreach (const QString &af, m_actionFiles) {
        QString _src = actionPath + af;
        QString _dest = installationPath + "actions/" + af;
        //kDebug() << "installing plugin to " << _ipath;
        if (!install(_src, _dest)) {
            kDebug() << "EE: action installation error" << _src << _dest;
        };
    }

    kDebug() << "============== Scripts:";
    // Install datafiles
    //kDebug() << "---> installing scripts" << m_scriptFiles;
    KStandardDirs::makeDir(installationPath + "scripts/");
    QString scriptPath = m_root.path() + "/scripts/";
    foreach (const QString &af, m_scriptFiles) {
        QString _src = scriptPath + af;
        QString _dest = installationPath + "scripts/" + af;
        if (!install(_src, _dest)) {
            kDebug() << "EE: script installation error" << scriptPath + af << m_dataPath + "/" + af;
        };
    }

    kDebug() << "============== Data:";
    if (!QDir(m_root.path() + "/data/").exists()) {
        kDebug() << "No data to install, but that's okay.";
        return;
    }
    // Install datafiles
    //kDebug() << "---> installing scripts" << m_scriptFiles;
    KStandardDirs::makeDir(installationPath + "data/");
    QString dataPath = m_root.path() + "/scripts/";
    // FIXME: needs recursive installation
    foreach (const QString &df, m_dataFiles) {
        QString _src = dataPath + df;
        QString _dest = installationPath + "data/" + df;
        if (!install(_src, _dest)) {
            kDebug() << "EE: data installation error" << scriptPath + df << m_dataPath + "/" + df;
        };
    }

    //kDebug() << "=============== Metadata:";
    //QString _src, _dest;
    //return true;
}

bool Package::install(const QString &source, const QString &destination)
{
    kDebug() << "Installing ... " << source << destination;
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
