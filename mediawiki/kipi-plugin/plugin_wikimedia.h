/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to WikiMedia web service
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef PLUGIN_WIKIMEDIA_H
#define PLUGIN_WIKIMEDIA_H

// Qt includes

#include <QVariant>

// KDE includes

#include <kdeversion.h>

// LibKIPI includes

#include <libkipi/plugin.h>

// LibMediaWiki includes

#include <libmediawiki/login.h>
#include <libmediawiki/mediawiki.h>

class KAction;

namespace KIPIWikiMediaPlugin
{
    class WMWindow;
    class WmLogin;
    class WikiMediaJob;
}

class KJob;

class Plugin_WikiMedia
    : public KIPI::Plugin
{
Q_OBJECT

public:

    Plugin_WikiMedia(QObject *parent, const QVariantList &args);
    ~Plugin_WikiMedia();

    KIPI::Category category(KAction* action) const;
    void setup(QWidget*);
    int runLWindow();
    void runMWindow();

public Q_SLOTS:
    void slotExport();

private:

    KAction                       * m_actionExport;
    KIPIWikiMediaPlugin::WmLogin  * m_dlgLoginExport;
    KIPIWikiMediaPlugin::WMWindow * m_dlgExport;
    KIPIWikiMediaPlugin::WikiMediaJob * m_uploadJob;
    QString                         m_login;
    QString                         m_pass;
    mediawiki::MediaWiki          * m_mediawiki;
    QUrl                            m_wiki;
};

#endif // PLUGIN_WIKIMEDIA_H
