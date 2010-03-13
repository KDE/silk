/*
 * SelkiePackageStructure.cpp
 *
 * Copyright (C) 2010 Sebastian Kügler <sebas@kde.org>
 */

#include <KZip>

#include "selkiepackagestructure.h"

SelkiePackageStructure::SelkiePackageStructure(QObject* parent, QString name)
    : Plasma::PackageStructure(parent, name)
{
    /*
    // stolen from http://api.kde.org/4.x-api/kdelibs-apidocs/plasma/html/classPlasma_1_1PackageStructure.html#a7f8ac5c04add42761625092881f67be1
    PackageStructure structure;

    structure.addDirectoryDefinition("images", "pics/", i18n("Images"));
    QStringList mimetypes;
    mimetypes << "image/svg" << "image/png" << "image/jpeg";
    structure.setMimetypes("images", mimetypes);

    structure.addDirectoryDefinition("scripts", "code/", i18n("Executable Scripts"));
    mimetypes.clear();
    mimetypes << "text/\*";
    structure.setMimetypes("scripts", mimetypes);

    structure.addFileDefinition("mainscript", "code/main.js", i18n("Main Script File"));
    structure.setRequired("mainscript", true);
    */
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
    addDirectoryDefinition("scripts", "scripts/", i18n("Scripts"));
    QStringList mimetypes;
    mimetypes << "application/javascript";
    setMimetypes("scripts", mimetypes);

    mimetypes.clear();

    addDirectoryDefinition("actions", "actions/", i18n("Actions"));
    mimetypes << "application/x-desktop";
    setMimetypes("actions", mimetypes);

    addFileDefinition("plugin", "plugin.desktop", i18n("Plugin .desktop file"));
    setRequired("plugin", true);
    addFileDefinition("webapp", "webapp.desktop", i18n("Webapp .desktop file"));
    setRequired("webapp", true);
}

SelkiePackageStructure::~SelkiePackageStructure()
{
}


bool SelkiePackageStructure::exportPackage(const KUrl &exportUrl, const KUrl &targetFile)
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

bool SelkiePackageStructure::importPackage(const KUrl &importFile, const KUrl &targetUrl)
{
    kWarning() << "Importing" << importFile.path();
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

#include "selkiepackagestructure.moc"
