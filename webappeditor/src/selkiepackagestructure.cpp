/*
 * SelkiePackageStructure.cpp
 *
 * Copyright (C) 2010 Sebastian Kügler <sebas@kde.org>
 */
#include "selkiepackagestructure.h"

SelkiePackageStructure::SelkiePackageStructure(QObject* parent, QString name)
    : Plasma::PackageStructure(parent, name)
{
    /*
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


#include "selkiepackagestructure.moc"
