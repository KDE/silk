/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-12-26
 * Description : a kipi plugin to import/export images to WikiMedia web service
 *
 * Copyright (C) 2005-2008 by Vardhman Jain <vardhman at gmail dot com>
 * Copyright (C) 2008-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2008-2009 by Luka Renko <lure at kubuntu dot org>
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
