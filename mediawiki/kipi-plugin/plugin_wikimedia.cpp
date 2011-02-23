/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-12-26
 * Description : a kipi plugin to export images to WikiMedia web service
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

#include "plugin_wikimedia.h"

// C ANSI includes

extern "C"
{
#include <unistd.h>
}

// KDE includes

#include <kdebug.h>
#include <KConfig>
#include <KApplication>
#include <KAction>
#include <KActionCollection>
#include <KGenericFactory>
#include <KLibLoader>
#include <KStandardDirs>
#include <kwindowsystem.h>

// LibKIPI includes

#include <libkipi/interface.h>

// LibMediaWiki includes

#include <libmediawiki/login.h>
#include <libmediawiki/mediawiki.h>

// Local includes

#include "wmwindow.h"
#include "wmlogin.h"
#include "wikimediajob.h"

K_PLUGIN_FACTORY( WikiMediaFactory, registerPlugin<Plugin_WikiMedia>(); )
K_EXPORT_PLUGIN ( WikiMediaFactory("kipiplugin_wikimedia") )

Plugin_WikiMedia::Plugin_WikiMedia(QObject* parent, const QVariantList& /*args*/)
               : KIPI::Plugin(WikiMediaFactory::componentData(),
                              parent, "WikiMedia Export")
{
    kDebug(AREA_CODE_LOADING) << "Plugin_WikiMedia plugin loaded";
}

void Plugin_WikiMedia::setup(QWidget* widget)
{
    m_dlgExport = 0;

    KIPI::Plugin::setup(widget);

    KIconLoader::global()->addAppDir("kipiplugin_wikimedia");

    m_actionExport = actionCollection()->addAction("wikimediaexport");
    m_actionExport->setText(i18n("Export to &WikiMedia..."));
    m_actionExport->setIcon(KIcon("wikimedia"));

    connect(m_actionExport, SIGNAL( triggered(bool) ),
            this, SLOT( slotExport()) );

    addAction(m_actionExport);

    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    if (!interface)
    {
        kError() << "Kipi interface is null!";
        m_actionExport->setEnabled(false);
        return;
    }

    m_actionExport->setEnabled(true);
}

Plugin_WikiMedia::~Plugin_WikiMedia()
{
}
int Plugin_WikiMedia::runLWindow()
{
    m_dlgLoginExport = new KIPIWikiMediaPlugin::WmLogin(kapp->activeWindow(), i18n("Login"), QString(), QString());

        QString username_edit, password_edit;
        QUrl wiki;

        if (!m_dlgLoginExport)
        {
            kDebug() << " Out of memory error " ;
        }

        if (m_dlgLoginExport->exec() == QDialog::Accepted)
        {
            username_edit = m_dlgLoginExport->username();
            password_edit = m_dlgLoginExport->password();
            wiki = m_dlgLoginExport->wiki();
            delete m_dlgLoginExport;
        }
        else
        {
            delete m_dlgLoginExport;
            //Return something which say authentication needed.
            return -1;
        }
        mediawiki::MediaWiki mediawiki(wiki);
        mediawiki::Login login(mediawiki, username_edit, password_edit);
        login.exec();
        qDebug()<< login.error();
        return login.error();
}

void Plugin_WikiMedia::runMWindow()
{

    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    if (!interface)
    {
        kError() << "Kipi interface is null!";
        return;
    }

    KStandardDirs dir;
    QString tmp = dir.saveLocation("tmp", QString("kipi-wm-") + QString::number(getpid()) + QString("/"));

    if (!m_dlgExport)
    {
        // We clean it up in the close button
        m_dlgExport = new KIPIWikiMediaPlugin::WMWindow(interface, tmp, kapp->activeWindow());
    }
    else
    {
        if (m_dlgExport->isMinimized())
            KWindowSystem::unminimizeWindow(m_dlgExport->winId());

        KWindowSystem::activateWindow(m_dlgExport->winId());
    }

    m_dlgExport->reactivate();
}

void Plugin_WikiMedia::slotExport()
{
    if(!runLWindow())
        runMWindow();
}


KIPI::Category Plugin_WikiMedia::category( KAction* action ) const
{
    if (action == m_actionExport)
        return KIPI::ExportPlugin;

    kWarning() << "Unrecognized action for plugin category identification";
    return KIPI::ExportPlugin;
}

KJob* Plugin_WikiMedia::exportFiles(const QString& album)
{
    KIPI::Interface* interface = dynamic_cast<KIPI::Interface*>(parent());
    return new KIPIWikiMediaPlugin::WikiMediaJob(interface, album);
}
